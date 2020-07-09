Linuxscripts

These files are created to simplify installation of remotehaptics hapticside on Ubuntu 16.04 (20.04 seemed problematic).
You need the proprietary OpenHapticsAE_Linux_v3_0.zip 

1. Install dependend packages
sh ubuntudep.sh

2. Install openhaptics
sh install_oh.sh

3. Build chai. Start with making the "glue" library hdPhantom
cd
wget https://chai3d.org/download/chai3d-3.2.0-multiplatform.zip
unzip chai3d-3.2.0-multiplatform.zip
cd chai3d-3.2.0/extras/hdPhantom
sudo sh linux-installation.sh
cd ../../
cmake .
make -j3

4. Build hapticside
cd ../hapitcside
qmake
make
sudo make install

5. Make it start on boot
sudo cp start_hapticside /usr/bin
sudo cp rc.local /etc/rc.local

6. Install dhcp server and configure network to be 192.168.200.1 with client 192.168.200.2
sudo apt install isc-dhcp-server
sudo cp dhcpd.conf /etc/dhcp/dhcpd.conf
sudo cp interfaces /etc/network/interfaces

7. Disable x window autostart and configure for PHANToMConfiguration to start
sudo systemctl disable lightdm.service
echo LANG=en_us PHANToMConfiguration > .xinitrc

8. Make the calibration console app
sh make_calib.sh

9. If we are lucky now, we can reboot :)
sudo reboot

