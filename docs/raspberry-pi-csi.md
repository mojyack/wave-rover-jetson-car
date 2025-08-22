- ensure kernel configs `CONFIG_VIDEO_BCM2835_UNICAM_LEGACY` and `CONFIG_VIDEO_BCM2835_UNICAM` are set
- install `libcamera-tools`
- edit /boot/config.txt as:
```
camera_auto_detect=0
dtoverlay=imx219
enable_uart=1
# enable kms for rich model
[pi4]
dtoverlay=vc4-kms-v3d
# need for hardware encoding
[pi02]
gpu_mem=256
```
- edit config.py as:
    - `port = "/dev/ttyS0"`
    - `camera_command = "gst-launch-1.0 --no-position libcamerasrc ! 'video/x-raw,width=800,height=600,framerate=30/1' ! jpegenc ! multifilesink location=snapshot.jpg"`

# note
- hardware h.264 encoding
`gst-launch-1.0 libcamerasrc ! 'video/x-raw,width=X,height=X,framerate=X' ! v4l2h264enc ! 'video/x-h264,level=(string)4' ! ...`
