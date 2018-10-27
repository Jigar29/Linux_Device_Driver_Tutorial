#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/semaphore.h>
#include <linux/errno.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>              // This is used to access the user-space memory from the kernel space, since 
                                        // both the memory sections are different, accessing the user-space is a little bit 
                                        // difficult. While accessing kernel-space memory from the user-space is restricted and 
                                        // the system calls are used only for accessing various hardware devices through kernel 

// Module description
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jigar Agrawal");
MODULE_DESCRIPTION("Character device driver.");
MODULE_VERSION("0.0"); 

//Defining the local memory for reading and sriting operations
static char device_buffer[100] = ""; 

//Declaring the semaphor to be used for the device 
struct semaphore device_semaphore = {0}; 

// Defines 
#define DEVICE_NAME "chardevice"

// Major number for the device, the major number is the number kernel require when the 
// device is loaded into the the memory 
int major_num = -1;

//Dev_t variable , whcih holds the major and minor number assigned by the kernel 
dev_t device_registration = {0}; 

//Declaring the variable which receives the return values of the functions 
int return_val = -1; 

//Character device structure 
struct cdev *char_device; 

// Local File operation functions 
// Everything in the linux is file. So are the operations. 
int openTheDevice (struct inode *, struct file *); 
ssize_t readFromDevice (struct file *, char __user *, size_t, loff_t *);
ssize_t writeToDevice (struct file *, const char __user *, size_t, loff_t *);
int closeTheDevice (struct inode *, struct file *);

// File operation structure that the device is going to use during the system calls from user-applications 
struct file_operations device_file_operations =
{
    .owner = THIS_MODULE,
    .open = openTheDevice,
    .read = readFromDevice,
    .write = writeToDevice,
    .release = closeTheDevice
}; 

static int __init module_initialization(void)
{
    //Dynamically requesting the available major and minor numbers from the kernel 
    return_val = alloc_chrdev_region(&device_registration, 0, 1, DEVICE_NAME); 

    if(return_val<0)
    {
        printk(KERN_ALERT"The device was not registered to the kernel, please try again later..."); 
        return return_val; 
    }
    
    major_num = MAJOR(device_registration); 

    printk(KERN_ALERT "The device was successfuly loaded with the Major no: %d", major_num); 
    // Since we still have to create the device file in the kernel directory 
    // Due to the dynamic number allocation it is not possible to create the file beforehand 
    printk(KERN_ALERT"Please use 'mknod /dev/chardevice c %d %d'",major_num, MINOR(device_registration));
    //Now as we have obtained the device numbers from the kernel, let's create the devide strcture
    //there are 2 ways to create the structue for the character device 
    // 1. Using cdev_alloc - if you dont have available cdev_data structure 
    // 2. cdev_init - incase you already have available cdev data structure 
    char_device = cdev_alloc(); 
    char_device->owner = THIS_MODULE;
    char_device->ops = &device_file_operations; 

    // Now let us inform the kernel that we have a ready charcater device ready to be installed to the kernel 
    // int cdev_add(struct cdev *, dev_t, unsigned);
    return_val = cdev_add(char_device, device_registration, 1); 
    if(return_val < 0)
    {
        printk(KERN_ALERT"static inline void sema_init(struct semaphore *sem, int val)The Device can not be added to the kernel at this point.. Please try again later"); 
        printk(KERN_ALERT"Unregistering the device numbers from the kernel table..."); 
        // extern void unregister_chrdev_region(dev_t, unsigned);
        unregister_chrdev_region(device_registration, 1); 
        return return_val; 
    }
    printk(KERN_ALERT"The device has been added to the kernel Successfully."); 

    // Initializing the semaphore for the file operation to avoid data curruption from multiple processes or 
    // threads writting to this file at the same time 
    // static inline void sema_init(struct semaphore *sem, int val); 
    sema_init(&device_semaphore, 1); 
    return return_val; 
}

// Module deinitialization function. 
// When the module gets unloaded from the kernel, this function gets called.. 
static void module_deinitialization (void)
{
    printk(KERN_ALERT"De-initializing the kernel module..."); 

    // Detaching the driver module from the kernel. 
    // void cdev_del(struct cdev *);
    cdev_del(char_device); 

    // Freeing the device number allocated for the device from the kernel 
    unregister_chrdev_region(device_registration, 1); 
}

int openTheDevice (struct inode *node, struct file *fptr)
{
    // Decrementing the semaphor by 1 to make it 0 and preventing this file to get executed by
    // multiple threads 
    // extern int __must_check down_interruptible(struct semaphore *sem);
    if(down_interruptible(&device_semaphore) != 0)
    {
        printk(KERN_ALERT"The device is being operated by some other thread, please let the device become free");
        return -1; 
    }
    printk(KERN_ALERT"The device is opened and ready to be used"); 
    return 0; 
}

ssize_t readFromDevice (struct file *fptr, char __user *dest_buffer, size_t bytes_to_read, loff_t *cur_pos)
{
    // Reading the given number of bytes from the device
    // Static __always_inline unsigned long copy_to_user(void __user *to, const void *from, unsigned long n)

    return_val = copy_to_user(dest_buffer, device_buffer, bytes_to_read);

    //Some error checks 
    if(!return_val)
    {
        printk(KERN_ALERT"All the data read successfully %s", device_buffer);
        cur_pos += bytes_to_read;  
        return bytes_to_read; 
    }
    return 0; 
}
ssize_t writeToDevice (struct file *fptr, const char __user *source_buffer, size_t bytes_to_write, loff_t *cur_pos)
{
    // Writting the number of bytes to the device 
    // static inline unsigned long copy_from_user(void *to, const void __user *from, unsigned long n)

    printk(KERN_ALERT"Came into the write function");
    
    return_val = copy_from_user(device_buffer, source_buffer, bytes_to_write); 

    if(!return_val)
    {
        printk(KERN_ALERT"The char received is %s", device_buffer); 
        cur_pos += bytes_to_write; 
        return bytes_to_write; 
    } 
    return 0;  
}
int closeTheDevice (struct inode *node, struct file *fptr)
{
    printk(KERN_ALERT"Closing the device.."); 
    // Let us release the semaphore for other threads 
    up(&device_semaphore); 
    return 0; 
}

module_init(module_initialization); 
module_exit(module_deinitialization); 