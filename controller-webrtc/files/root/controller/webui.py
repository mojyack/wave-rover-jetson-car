import asyncio
import json
import gi
from aiohttp import web

gi.require_version("Gst", "1.0")
gi.require_version("GstWebRTC", "1.0")
from gi.repository import Gst, GstWebRTC, GstSdp

import config

Gst.init(None)

pipeline = None
webrtc = None
viewer_ws = None
loop = None

# gstreamer helpers


def create_pipeline():
    global pipeline, webrtc

    pipeline = Gst.parse_launch(config.monitor_pipeline)
    webrtc = pipeline.get_by_name("wb")

    webrtc.connect("on-negotiation-needed", on_negotiation_needed)
    webrtc.connect("on-ice-candidate", on_ice_candidate)

    pipeline.set_state(Gst.State.PLAYING)
    print("pipeline started")


def destroy_pipeline():
    global pipeline, webrtc

    if not pipeline:
        return

    pipeline.set_state(Gst.State.NULL)
    pipeline = None
    webrtc = None
    print("pipeline stopped")


# signaling helpers


def send_to_viewer(msg):
    if config.debug_webui:
        print("to viewer: ", msg)
    if viewer_ws:
        loop.call_soon_threadsafe(asyncio.create_task, viewer_ws.send_json(msg))


def on_ice_candidate(_, mlineindex, candidate):
    send_to_viewer(
        {
            "type": "ice",
            "candidate": {"sdpMLineIndex": mlineindex, "candidate": candidate},
        }
    )


def on_offer_created(promise, _):
    reply = promise.get_reply()
    offer = reply.get_value("offer")

    webrtc.emit("set-local-description", offer, None)

    send_to_viewer(
        {"type": "offer", "sdp": {"type": "offer", "sdp": offer.sdp.as_text()}}
    )


def on_negotiation_needed(_):
    promise = Gst.Promise.new_with_change_func(on_offer_created, None)
    webrtc.emit("create-offer", None, promise)


# http endpoints
routes = web.RouteTableDef()


@routes.get("/")
async def index(request):
    return web.FileResponse("index.html")


@routes.get("/script.js")
async def js(request):
    return web.FileResponse("script.js")


@routes.get("/style.css")
async def css(request):
    return web.FileResponse("style.css")


@routes.get("/ws")
async def ws_handler(request):
    global viewer_ws, loop

    loop = asyncio.get_running_loop()

    ws = web.WebSocketResponse()
    await ws.prepare(request)

    viewer_ws = ws
    print("viewer connected")

    create_pipeline()

    try:
        async for msg in ws:
            if msg.type != web.WSMsgType.TEXT:
                print("unhandled ws type", msg.type)
                continue

            if msg.data[0] == "L" or msg.data[0] == "R":
                parse_motor_command(msg.data)
                continue

            data = json.loads(msg.data)
            if config.debug_webui:
                print("from viewer:", data)

            if data["type"] == "answer":
                _, sdpmsg = GstSdp.SDPMessage.new_from_text(data["sdp"]["sdp"])
                answer = GstWebRTC.WebRTCSessionDescription.new(
                    GstWebRTC.WebRTCSDPType.ANSWER, sdpmsg
                )
                webrtc.emit("set-remote-description", answer, None)

            elif data["type"] == "ice":
                c = data["candidate"]
                webrtc.emit("add-ice-candidate", c["sdpMLineIndex"], c["candidate"])

    finally:
        viewer_ws = None
        destroy_pipeline()
        print("viewer disconnected")

    return ws


# application
motor_callback = None


class MotorCommand:
    SetSpeed = 0
    Brake = 1
    Coast = 2


def parse_motor_command(text):
    match text[0]:
        case "L" | "R":
            match text[1]:
                case "B":
                    motor_callback(text[0].lower(), MotorCommand.Brake)
                case "C":
                    motor_callback(text[0].lower(), MotorCommand.Coast)
                case _:
                    motor_callback(
                        text[0].lower(), MotorCommand.SetSpeed, float(text[1:])
                    )
        case _:
            print("invalid command", text)


def start_server():
    app = web.Application()
    app.add_routes(routes)
    web.run_app(app, port=8000)
