# This is fixing and installing the console calibration application from OpenHaptics
sudo patch -d/ -p0 < calib.patch
cd /usr/share/3DTouch/examples/HD/console/Calibration
sudo make
sudo cp Calibration /usr/bin/calib

