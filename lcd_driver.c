#include <linux/init.h> 
#include <linux/module.h>
#include <linux/moduleparam.h>

// Some module descriptions 
MODULE_LICENSE("GPL");                         // GPL - General Public Liecese. if we dont add this line to the code 
                                               // the kernel think that we have our own liecense and dont want to outsourse
                                               // the source code, so the kernel blocks some functionality, because Linux is 
                                               // meant to be open-source always
MODULE_AUTHOR("Jigar Agrawal"); 
MODULE_DESCRIPTION("This module is the sample implementation of the LCD Driver for the Linux kernel");

int count = 0; 

// Adding a module parameter here which is accessible to root only for writting and others can only see 
module_param(count, int, 0644); 

// Function frm the keyboard module. So in short the key board module should be already running and registered to the kernel 
// before this module gets attached to the kernel 
const char characterFromKeypad(void); 

// A sort of constructor 
// __init - is used to remove the init function after it is executed to save the ram memory. 
//          this fucntion is never going to be called twice 
//__initdata - this just like __init for the methods, but used for variables instead 

__init int lcd_driver_init(void)
{
    printk(KERN_ALERT"Initializing the LCD, Please wait %d Sec...", count);
    printk(KERN_ALERT"Message is %c", characterFromKeypad());  
    return 0; 
}

// A sort of desructor 
void lcd_driver_exit(void)
{
    printk(KERN_ALERT"Turning off the LCD module");
}

// attaching the init and exit function to the module implementation 
module_init(lcd_driver_init); 
module_exit(lcd_driver_exit); 

