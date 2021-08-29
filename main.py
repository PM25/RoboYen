import sys
import time

from roboyen import RoboYen
from components import Camera


def main():
    print("\n[Start]")
    cam = Camera()
    cam.streaming()

    bot = RoboYen()
    bot.start()


if __name__ == "__main__":
    main()
