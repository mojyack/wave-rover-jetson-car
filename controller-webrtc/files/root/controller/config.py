# port: path to tty device file or None to disable seiral port
port = "/dev/ttyS0"
# baudrate: used if port != None
baudrate = 115200

# gstreamer pipeline to stream monitor video
# example for rpi02w
monitor_pipeline = """
libcamerasrc !
video/x-raw,width=800,height=600,framerate=30/1 !
v4l2h264enc extra-controls="controls,repeat_sequence_header=1" !
video/x-h264,level=(string)4 !
h264parse config-interval=1 !
rtph264pay pt=96 config-interval=1 !
webrtcbin name=wb
"""

# debug
debug_daemon = False
debug_webui = False

if False:
    port = None
    monitor_pipeline = """
    videotestsrc !
    video/x-raw,width=800,height=600,framerate=30/1 !
    x264enc tune=zerolatency speed-preset=ultrafast key-int-max=30 !
    h264parse config-interval=1 !
    rtph264pay pt=96 config-interval=1 !
    webrtcbin name=wb
    """
