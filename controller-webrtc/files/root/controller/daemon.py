import sys

import config
import webui

if config.port:
    import serial
    dev = serial.Serial(config.port, config.baudrate)
else:
    dev = None

def send_command(cmd):
    global dev
    if config.debug_daemon:
        print("<<<", cmd)
    if dev:
        dev.write(bytes(cmd + "\0", "ascii"))

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

webui.motor_callback = handle_motor
webui.start_server()
