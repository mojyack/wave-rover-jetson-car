const ws = new WebSocket("ws://" + location.host + "/ws");

// wheel control
let pointer_ids = {left : null, right : null};

const brake_area = {
    left : [ 0.0, 0.2 ],
    right : [ 0.8, 1.0 ]
}

function draw_controll_bar(elm, y, brake = false) {
    ctx = elm.getContext('2d')
    ctx.clearRect(0, 0, elm.width, elm.height);

    ctx.fillStyle = "#A0A0A0"
    ctx.fillRect(0, y - 5, elm.width, 10);

    const ba      = brake_area[elm.id];
    ctx.fillStyle = brake ? "#FF0000FF" : "#FF000080"
    ctx.fillRect(ba[0] * elm.width, 0, ba[1] * elm.width, elm.height);
}

const clamp = (n, min, max) => Math.min(Math.max(n, min), max)

function calc_local_pos(elm, ev) {
    const cr = elm.getBoundingClientRect();
    const x  = clamp(ev.clientX - cr.x, 0, elm.width);
    const y  = clamp(ev.clientY - cr.y, 0, elm.height);
    return [ x, y ];
}

function down(elm, ev) {
    const [x, y] = calc_local_pos(elm, ev);
    const sym    = elm.id[0].toUpperCase()

    const ba = brake_area[elm.id];
    if(x >= ba[0] * elm.width && x < ba[1] * elm.width) {
        ws.send(sym + "B");
        draw_controll_bar(elm, elm.height / 2, true);
    } else {
        const v = (y - elm.height * 0.5) / elm.height * 2 * -100;
        ws.send(sym + v.toString());
        draw_controll_bar(elm, y);
    }
}

function up(elm, ev) {
    const y   = elm.height / 2;
    const sym = elm.id[0].toUpperCase()
    ws.send(sym + "C");
    draw_controll_bar(elm, y);
}

function init_canvas(name) {
    const canvas = document.getElementById(name);

    canvas.addEventListener('pointerdown', ev => {
        ev.preventDefault();
        canvas.setPointerCapture(ev.pointerId);
        pointer_ids[canvas.id] = ev.pointerId;
        down(canvas, ev);
    });
    canvas.addEventListener('pointermove', ev => {
        if(pointer_ids[canvas.id] === ev.pointerId) {
            down(canvas, ev);
        }
    });
    canvas.addEventListener('pointerup', ev => {
        if(pointer_ids[canvas.id] === ev.pointerId) {
            up(canvas, ev);
            pointer_ids[canvas.id] = null;
            canvas.releasePointerCapture(ev.pointerId);
        }
    });
    canvas.addEventListener("pointercancel", () => {
        handle_up(canvas);
        pointer_ids[canvas.id] = null;
    });
}

function fit_canvas(canvas) {
    canvas.setAttribute("width", canvas.clientWidth);
    canvas.setAttribute("height", canvas.clientHeight);
}

function fit_canvases() {
    fit_canvas(document.getElementById("left"));
    fit_canvas(document.getElementById("right"));
}

document.addEventListener("DOMContentLoaded", () => {
    init_canvas("left");
    init_canvas("right");
    window.addEventListener("resize", fit_canvases);
    window.addEventListener("pageshow", fit_canvases);
    fit_canvases();
});

// monitor webrtc
const pc  = new RTCPeerConnection();
const mon = document.getElementById("monitor");

pc.ontrack = e => {
    mon.srcObject = e.streams[0];
};

pc.onicecandidate = e => {
    if(e.candidate) {
        ws.send(JSON.stringify({
            type : "ice",
            candidate : e.candidate
        }));
    }
};

ws.onmessage = async e => {
    const msg = JSON.parse(e.data);

    if(msg.type === "offer") {
        await pc.setRemoteDescription(msg.sdp);
        const answer = await pc.createAnswer();

        await pc.setLocalDescription(answer);
        ws.send(JSON.stringify({
            type : "answer",
            sdp : pc.localDescription
        }));
    }

    if(msg.type === "ice") {
        await pc.addIceCandidate(msg.candidate);
    }
};
