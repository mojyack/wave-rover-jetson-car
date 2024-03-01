port = "/dev/ttyUSB0"
baudrate = 115200

strenc = "ascii"

# camera
# camera_command = "gst-launch-1.0 v4l2src ! 'image/jpeg,width=800,height=600,framerate=30/1' ! multifilesink location=plain-snapshot.jpg"
camera_command = "gst-launch-1.0 nvarguscamerasrc sensor-id=0 ! 'video/x-raw(memory:NVMM),width=800,height=600,framerate=15/1,format=NV12' ! nvjpegenc ! multifilesink location=snapshot.jpg"
# camera_command = "/usr/bin/true"

# debug
debug_daemon = True
debug_webui = False
fakedev = False
