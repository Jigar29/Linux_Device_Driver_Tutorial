#include <linux/init.h> 
#include <linux/module.h>
#include <linux/moduleparam.h>

MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Jigar Agrawal"); 

int count = 0; 

module_param(count, int, 0644); 

const char characterFromKeypad(void); 

__init int lcd_driver_init(void)
{
    printk(KERN_ALERT"Initializing the LCD, Please wait %d Sec...", count);
    printk(KERN_ALERT"Message is %c", characterFromKeypad());  
    return 0; 
}

void lcd_driver_exit(void)
{
    printk(KERN_ALERT"Turning off the LCD module");
}

module_init(lcd_driver_init); 
module_exit(lcd_driver_exit); 

