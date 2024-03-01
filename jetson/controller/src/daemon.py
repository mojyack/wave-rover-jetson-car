import sys

import config

if not config.fakedev:
    import serial

import camera
cam = camera.Camera(config.camera_command)

dev = serial.Serial(config.port, config.baudrate) if not config.fakedev else None

import webui

def send_command(cmd):
    global dev
    if config.debug_daemon:
        print("<<<", cmd)
    if not config.fakedev:
        dev.write(bytes(cmd + "\0", config.strenc))

def handle_motor(sym, command, value=None):
    match command:
        case webui.MotorCommand.SetSpeed:
            if value > 0:
                send_command(f"{sym}f")
            else:
                send_command(f"{sym}r")
                value *= -1
            value /= 100
            send_command(f"{sym}s{value}")
        case webui.MotorCommand.Brake:
            send_command(f"{sym}b")
            send_command(f"{sym}s0")
        case webui.MotorCommand.Coast:
            send_command(f"{sym}c")
            send_command(f"{sym}s0")

cam.start()
webui.start_server(handle_motor)
cam.stop()
