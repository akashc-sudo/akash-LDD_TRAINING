obj-m += circular_buffer_driver.o


all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean : 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean


