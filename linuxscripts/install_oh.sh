unzip OpenHapticsAE_Linux_v3_0.zip
cd OpenHapticsAE_Linux_v3_0/OpenHaptics-AE 3.0/64-bit
sudo dpkg -i openhaptics-ae_3.0-2_amd64.deb
cd ../../PHANTOM\ Device\ Drivers/64-bit
sudo dpkg -i phantomdevicedrivers_4.3-3_amd64.deb

sudo ln -s /usr/lib/x86_64-linux-gnu/libraw1394.so.11 /usr/lib/libraw1394.so.8
sudo ln -s /usr/lib64/libPHANToMIO.so.4.3 /usr/lib/libPHANToMIO.so.4
sudo ln -s /usr/lib64/libHD.so.3.0.0 /usr/lib/libHD.so.3.0
sudo ln -s /usr/lib64/libHL.so.3.0.0 /usr/lib/libHL.so.3.0
sudo ln -s /usr/lib64/libHD.so.3.0.0 /usr/lib/libHD.so

cd raw1394spoof
make
sudo mkdir /var/lib/dummy1394
sudo cp raw1394.ko /var/lib/dummy1394/
sudo ln /dev/fw0 /dev/raw1394
sudo chmod 0777 /dev/raw1394

