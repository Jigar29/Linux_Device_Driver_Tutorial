#Make file selections 
obj-m := lcd_driver.o
obj-m += keypad_driver.o
obj-m += solid_chardev_driver.o

#Make file build operations 
KDIR = /lib/modules/`uname -r`/build/
MDIR = $(PWD)

all: 
	$ make -C $(KDIR) M=$(MDIR) modules
	$ gcc user_applicaion.c -o user_applicaion

clean: 
	$ make -C $(KDIR) M=$(MDIR) clean
	$ rm user_applicaion 
	$ clear
	$ echo "cleaned.." 