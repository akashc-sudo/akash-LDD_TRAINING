obj-m += DeviceCreate.o

CC= /usr/bin/x86_64-linux-gnu-gcc-13

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules CC=$(CC)

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

