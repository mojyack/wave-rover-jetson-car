import threading

import serial

port = "/dev/ttyUSB0"
baudrate = 115200
strenc = "ascii"

def reader_main(dev):
    while True:
        data = dev.read()
        try:
            text = data.decode(strenc)
        except UnicodeDecodeError:
            text = "?"
        print(text, end="")


def main():
    dev = serial.Serial(port, baudrate)
    reader = threading.Thread(target=reader_main, args=(dev,), daemon=True)
    reader.start()
    try:
        while True:
            data = input()
            dev.write(bytes(data + "\0", strenc))
    except KeyboardInterrupt:
        pass


if __name__ == "__main__":
    main()
