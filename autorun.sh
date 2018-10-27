
make clean 
echo "[DONE] Binaries has been cleared..."
make all 
echo "[DONE] Building Binaries."

sudo -S <<<"miltonjug29" insmod solid_chardev_driver.ko
echo "[DONE] Module inserted in the kernel..."

sudo -S <<<"miltonjug29" mknod /dev/chardevice c 240 0 
echo "[DONE] Device file created"

sudo -S <<<"miltonjug29" chmod 777 /dev/chardevice 
echo "[DONE] Permission of the device file Changed..."

"/home/jigar/Desktop/linux_dev_driver/user_application" ./user_application

echo "Running the user application now"
