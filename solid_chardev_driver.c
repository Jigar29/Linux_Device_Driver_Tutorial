#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/uaccess.h> 

// Module description 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jigar Agrawal"); 
MODULE_DESCRIPTION("Character device driver.");
MODULE_VERSION("0.0"); 

// Defines 
#define DEVICE_NAME "char_device"

// Major number for the device, the major number is the number kernel require when the 
// device is loaded into the the memory 
int major_num = -1;

//Dev_t variable , whcih holds the major and minor number assigned by the kernel 
dev_t device_registration = (dev_t *) 0; 

//Declaring the variable which receives the return values of the functions 
int return_val = -1; 

// Local File operation functions 
// Everything in the linux is file. So as the operations. 
/*int openTheDevice (struct inode *, struct file *); 
ssize_t readFromDevice (struct file *, char __user *, size_t, loff_t *);
ssize_t writeToDevice (struct file *, const char __user *, size_t, loff_t *);
int closeTheDevice (struct inode *, struct file *);*/

static int __init module_initialization(void)
{
    // Registering the character devie to the kernel 
    // This function can be found in fs.h 

    return_val = alloc_chrdev_region(&device_registration, 0, 1, DEVICE_NAME); 

    if(return_val<0)
    {
        printk(KERN_ALERT"The device was not registered to the kernel, please try again later..."); 
        return return_val; 
    }
    
    major_num = MAJOR(device_registration); 

    printk(KERN_ALERT "The device was successfuly loaded with the Major no: %d", major_num); 
    return major_num; 
}

static void module_deinitialization (void)
{
    printk(KERN_ALERT"De-initializing the kernel module..."); 
}

module_init(module_initialization); 
module_exit(module_deinitialization); 