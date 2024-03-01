# run controller daemon and webui on jetson
## notes
### permissions
except for access to the serial port, this software should be able to run as an normal user.
make sure that you have the correct permissions to the serial port.

in order to use nvenc, you muse be in the video group.  
if you are not already in that, run the following command:
```
sudo usermod -aG video $(id -un)
# reboot system to take effect
```
### bug of jetson's uart
some jetson nano models have a malfunctional uart pin(ref: [1]).  
it is safe to use a normal usb port for serial.

[1] https://forums.developer.nvidia.com/t/unreliable-serial-communcation-via-the-uart-tx-rx-gpio-pins/158249/30
### python path
if you built python manually, please ensure that you use proper python.  
e.g. `export PATH=$HOME/.local/bin:$PATH`

## install dependencies
```
python3 -m pip install pyserial
```
## start server
```
cd jetson/controller
# see src/config.py for configurations
python3 src/daemon.py
```
## open controller
open `http://$IP_ADDR_OF_JETSON:8000/www` with a web browser.  
you should see a controller ui.
