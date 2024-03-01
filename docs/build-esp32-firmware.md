# build firmware of wave rover
```
cd wave-rover
```

## prepare environment
### download arduino-cli
ref: https://arduino.github.io/arduino-cli/0.35/installation/
```
curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh
```
### download pyserial
```
python3 -m pip install -t lib pyserial
```
### edit config
```
sed "s|@WORKDIR@|$PWD|g" config.yaml.in > config.yaml
```
### download toolchain for esp32
```
bin/arduino-cli --config-file config.yaml core install esp32:esp32
```
### install libraries
```
bin/arduino-cli --config-file config.yaml lib install "Adafruit SSD1306"
bin/arduino-cli --config-file config.yaml lib install "INA219_WE"
```

## build and flush the firmware
### connect wave rover to the host machine
if you want to use usb-c port of wave rover, remember to use the port at the center(No.9 of [1]).  
the following instructions assume that /dev/ttyUSB0 is connected to the wave rover. to use another port, edit `env` file.

before flushing, ensure that you have rw permissions of the serial port.  
you can obtain that with `chmod o+rw /dev/ttyUSB0`.

[1] https://www.waveshare.com/w/upload/f/f0/General_Driver_for_Robot02.jpg
### load commands
```
source ./env
```
### build
```
amake firmware
```
### flush
```
aup firmware
```
