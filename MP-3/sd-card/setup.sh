echo "1-1:1.0" > /sys/bus/usb/drivers/usbhid/unbind
mv /lib/modules/3.6.0 /lib/modules/4.19.0-xilinx-54621-gd80ed71
insmod /mnt/launcher_driver.ko
cd /lib
ln -s ld-linux.so.3 ld-linux-armhf.so.3
