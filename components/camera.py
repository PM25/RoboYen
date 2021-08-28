import time
from pathlib import Path
import subprocess


class Camera:
    def __init__(self, width=320, height=240, framerate=10):
        self.device = self.select_camera_device()
        self.width = int(width)
        self.height = int(height)
        self.framerate = int(framerate)

    def select_camera_device(self):
        print("[Camera]")
        devices = [str(f) for f in Path("/dev").glob("video*")]

        assert len(devices) > 0

        if len(devices) > 1:
            print()
            print("=" * 5, "Available Devices for Camera", "=" * 5)
            for idx, device in enumerate(devices):
                print(f"{idx}:\t{device.path}\t{device.name}")
            print("=" * 30)
            device_idx = eval(input(f"Select Device >> "))

        elif len(devices) == 1:
            device_idx = 0

        device = devices[device_idx].path
        print(f"*Selected device for Camera: {device}")

        return device

    def streaming(self):
        process = subprocess.call(
            [
                "./gst-rtsp-server-1.14.1/examples/test-launch",
                f"( v4l2src device={self.device} ! image/jpeg,width={self.width},height={self.height},framerate={self.framerate}/1 ! jpegparse ! rtpjpegpay name=pay0 pt=96 )",
            ]
        )
