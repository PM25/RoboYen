import sys
import time


def safe_execute(func, max_try=1000, time_interval=1):
    for _ in range(int(max_try)):
        try:
            print("=> Program Start")
            func()

        except (KeyboardInterrupt, SystemExit) as e:
            print("ok, ok, terminating now...")
            break

        except (OSError) as e:
            print("*Can't detect any devices")

        except:
            print("=" * 15)
            print("*Unexpected error:", sys.exc_info()[0])
            raise

        time.sleep(time_interval)
