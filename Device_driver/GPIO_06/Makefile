
obj-m += gpio_driver.o

all:
    make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
    make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
# Name of the module (replace gpio_driver with your source file's name without the .c extension)
obj-m := gpio_driver.o

# Default values for kernel source and cross-compilation tools
ARCH ?= arm
CROSS_COMPILE ?= arm-linux-gnueabihf-
KDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

# Default target for building the kernel module
all:
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KDIR) M=$(PWD) modules

# Clean target for removing build artifacts
clean:
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KDIR) M=$(PWD) clean

# Install target for deploying the module to the target system
install:
	scp gpio_driver.ko user@target:/lib/modules/$(uname -r)/kernel/drivers/

# Help target to explain usage
help:
	@echo "Makefile Usage:"
	@echo "  make            - Build the kernel module"
	@echo "  make clean      - Clean the build artifacts"
	@echo "  make install    - Install the module to the target device"
	@echo "Variables:"
	@echo "  ARCH            - Target architecture (default: arm)"
	@echo "  CROSS_COMPILE   - Cross-compiler prefix (default: arm-linux-gnueabihf-)"
	@echo "  KDIR            - Kernel source directory (default: /lib/modules/$(uname -r)/build)"
