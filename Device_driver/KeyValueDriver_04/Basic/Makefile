
obj-m+=key_value_driver.o


all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules CC=$(CC)

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

