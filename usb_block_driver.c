#include <linux/module.h> 
#include <linux/init.h> 
#include <linux/moduleparam.h> 
#include <linux/fs.h> 
#include <linux/semaphore.h> 
#include <linux/usb.h> 

// Module and license declaration 
MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Jigar Agrawal"); 
MODULE_VERSION("0.0"); 
MODULE_DESCRIPTION("This module is the implementatin of the USB block driver");

// Semaphore declaration 
struct semaphore device_semaphore; 

// Return variable declaration 
int return_val = -1; 

// local usb driver functions 
int usb_drive_attach_function (struct usb_interface *intf, const struct usb_device_id *id);
void usb_drive_dettach_function (struct usb_interface *intf);

// USB_ID table for the pendrives 
static struct usb_device_id pendrive_table[] = 
{
  {USB_DEVICE(0x0781, 0x55a5)}, {}
}; 
MODULE_DEVICE_TABLE(usb, pendrive_table);

// USB driver structure declaration 
static struct usb_driver pendrive_driver= 
{
    .name       = "USB_PENDRIVE_DRIVER", 
    .id_table   = pendrive_table, 
    .probe      = usb_drive_attach_function, 
    .disconnect = usb_drive_dettach_function
};

// Init function 
int __init module_entry(void)
{
    printk(KERN_ALERT"The Module is initialized"); 

    // Registering the usb device driver to the usb core 
    return_val = usb_register(&pendrive_driver); 
    
    if(return_val<0)
    {
        printk(KERN_ALERT"Could not register the device to the USB-core"); 
        return -1; 
    }
    
    printk(KERN_ALERT"The device is registered to the USB-core successfully"); 
    return 0;  
}

// Exit function 
void module_cleanup(void)
{
    printk(KERN_ALERT"Module Deinitialized"); 

    // Unregstering the usb device driver 
    usb_deregister(&pendrive_driver); 

    printk(KERN_ALERT"The module is de-registered from the USB-core successfully"); 
}

// Registering the entry and exit functions 
module_init(module_entry); 
module_exit(module_cleanup); 


// Driver function definitions 
int usb_drive_attach_function (struct usb_interface *intf, const struct usb_device_id *id)
{
    printk(KERN_ALERT"[ATTACHED]The USB Pendrive attached"); 
    return 0; 
}

void usb_drive_dettach_function (struct usb_interface *intf)
{
    printk(KERN_ALERT"[DETTACHED]The USB Pendrive removed");
}
