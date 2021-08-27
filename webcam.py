from pathlib import Path
import subprocess

def get_video_device():
    fnames = [str(f) for f in Path("/dev").glob('video*')]
    
    assert len(fnames) > 0
    
    if(len(fnames) == 1):
        device_idx = 0
    else:
        device_idx = input('select device >>')

    return fnames[device_idx]


def main():
    video_device = get_video_device()
    process = subprocess.call(
            ['./gst-rtsp-server-1.14.1/examples/test-launch', 
                f'( v4l2src device={video_device} ! image/jpeg,width=320,height=240,framerate=10/1 ! jpegparse ! rtpjpegpay name=pay0 pt=96 )'
                ])


if __name__ == "__main__":
    while True:
        try:
            print("=> Trying to execute main()")
            main()
        
        except(KeyboardInterrupt, SystemExit) as e :
            print('ok, ok, terminating now...')
            break
        
        except:
            print("*Failed to execute main()")
        
        sleep(1)
