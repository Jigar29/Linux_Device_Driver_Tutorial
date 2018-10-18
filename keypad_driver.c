#include <linux/init.h> 
#include <linux/module.h>

MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Jigar Agrawal"); 

const char characterFromKeypad(void)
{
    return 'b'; 
}

EXPORT_SYMBOL(characterFromKeypad); 

__init int keypad_driver_init(void)
{
    printk(KERN_ALERT"Initializing the keypad, Please wait ..."); 
    return 0; 
}

void keypad_driver_exit(void)
{
    printk(KERN_ALERT"Turning off the keypad module");
}

module_init(keypad_driver_init); 
module_exit(keypad_driver_exit); 