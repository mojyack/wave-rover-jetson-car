import os
import functools
from http import HTTPStatus
from http.server import ThreadingHTTPServer
from http.server import SimpleHTTPRequestHandler

import config

class MotorCommand:
    SetSpeed = 0
    Brake = 1
    Coast = 2

class Server(SimpleHTTPRequestHandler):
    def __init__(
        self, handle_motor, finish_loop, *args, **kwargs
    ):
        self.handle_motor = handle_motor
        self.finish_loop = finish_loop
        self.protocol_version = "HTTP/1.1"
        super().__init__(*args, **kwargs)

    def log_request(self, code="-", size="-"):
        pass

    def send_snapshot(self, path):
        try:
            file = open(path, "rb")
        except OSError:
            self.send_error(HTTPStatus.NOT_FOUND, "File not found")
            return None

        try:
            fs = os.fstat(file.fileno())
            self.send_response(HTTPStatus.OK)
            self.send_header("Content-type", "image/jpeg")
            self.send_header("Content-Length", str(fs[6]))
            self.send_header("Cache-Control", "no-cache, no-store")
            self.end_headers()
            self.copyfile(file, self.wfile)
        except:
            print("failed to send snapshot")

    def do_GET(self):
        if config.debug_webui:
            print("GET: ", self.path)
        if "snapshot.jpg" in self.path:
            path = self.path
            param = path.find("?")
            if param != -1:
                path = path[:param]
            self.send_snapshot(os.getcwd() + path)
        else:
            super().do_GET()

    def do_POST(self):
        ctype = self.headers["content-type"]
        clen = int(self.headers["content-length"])
        text = self.rfile.read(clen).decode("utf-8")

        if config.debug_webui:
            print("POST:", text)

        match text[0]:
            case "L" | "R":
                match text[1]:
                    case "B":
                        self.handle_motor(text[0].lower(), MotorCommand.Brake)
                    case "C":
                        self.handle_motor(text[0].lower(), MotorCommand.Coast)
                    case _:
                        self.handle_motor(text[0].lower(), MotorCommand.SetSpeed, float(text[1:]))
            case "Q":
                self.finish_loop[0] = 1
            case _:
                print("unknown post")

        self.send_response(HTTPStatus.NO_CONTENT)
        self.end_headers()


def start_server(handle_motor):
    finish_loop = [0]
    server = ThreadingHTTPServer(
        ("0.0.0.0", 8000),
        functools.partial(Server, handle_motor, finish_loop),
    )
    try:
        while finish_loop[0] == 0:
            server.handle_request()
    except KeyboardInterrupt:
        pass


if __name__ == "__main__":

    def handle(s, c, v):
        pass

    start_server(handle)
