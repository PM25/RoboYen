cd /home/py/RoboYen

python3 joystick.py &
echo $! > pid.txt

# python3 arm.py &
# echo $! >> pid.txt

python3 webcam.py &
echo $! >> pid.txt
