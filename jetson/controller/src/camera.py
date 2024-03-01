import os
import subprocess
import signal


class Camera:
    def __init__(self, camera_command):
        self.camera_command = camera_command

    def start(self):
        self.process = subprocess.Popen(
            f"cd /tmp && {self.camera_command}", shell=True
        )

    def stop(self):
        self.process.send_signal(signal.SIGINT)
        self.process.wait()
