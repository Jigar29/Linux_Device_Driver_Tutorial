make clean 
echo "[DONE] Binaries has been cleared..."
make all 
echo "[DONE] Building Binaries."

sudo -S <<<"miltonjug29" insmod solid_chardev_driver.ko
echo "[DONE] Charcter device driver inserted in the kernel..."

sudo -S <<<"miltonjug29" mknod /dev/chardevice c 240 0 
echo "[DONE] Device file created"

sudo -S <<<"miltonjug29" chmod 777 /dev/chardevice 
echo "[DONE] Permission of the device file Changed..."

sudo -S <<<"miltonjug29" insmod usb_block_driver.ko
echo "[DONE] USB driver inserted in the kernel..."

./user_application
echo "Running the user application now"