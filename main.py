import sys
import time

from roboyen import RoboYen
from components import Camera
from utils.tools import safe_execute


def main():
    print("\n[Start]")
    cam = Camera()
    cam.streaming()

    bot = RoboYen()
    bot.start()


if __name__ == "__main__":
    safe_execute(main)
