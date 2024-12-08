

/*   	
 *      file : GpioInterruptDriver.c 
 *    	Gpio Interrupt Driver for Raspberrypi4 	
 *
 **/



#include<linux/kernel.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/gpio.h>    
#include <linux/interrupt.h> 
#include<linux/delay.h>

#define GPIO_BUTTON  27   /* Gpio-27 mapped to the unique number --- can  check@ /sys/kernel/debug/gpio */
#define GPIO_LED     529  /* Gpio-17 mapped to the unique number 529 can  check@ /sys/kernel/debug/gpio */

/*
 *	IRQ number :
 *	dev is type of struct dev_t,holds the pair of <major><minor> of an device/driver	
 **/
static int irq_number;       
static dev_t dev = 0;   
static struct class *dev_class;
static struct cdev cdev_device

/**
 *Function prototypes
 */
static irqreturn_t irq_handler_fun(int irq, void *dev_id);
static int __init FunInit(void);
static void __exit FunExit(void);

/*
*  IRQ handler for button
*  when interrupt issued to the cpu below ISR will executes
*/
static irqreturn_t irq_handler_fun(int irq, void *dev_id)
{
    static bool led_state = false;
    led_state = !led_state;  // Toggle LED state
    gpio_set_value(GPIO_LED, led_state);
    pr_info("Button pressed: LED toggled to %d\n", led_state);
    return IRQ_HANDLED;
}

/*
** Module init function
*/
static int __init FunInit(void)
{
/*  Allocating the major number dynamically using alloc_chedev_region() api	*/

    if((alloc_chrdev_region(&dev, 0, 1, "GpioInt")) < 0) {
        pr_err("Cannot allocate major number\n");
	return -1;
    }
    pr_info("Major = %d Minor = %d\n", MAJOR(dev), MINOR(dev));

/*
 *  creating the cdev structure to maintain the device related attributes under dev directory
 *  if we are using the file_operation in driver 2nd parameter of the cdev_init should be refrence 
 *  of fileoperation instead of NULL
 */
    cdev_init(&cdev_device,&f_ops);

/* 
 * adding device to the system	using cdev_add api 
 */
    if ((cdev_add(&cdev_device,dev,1)) < 0) {
        pr_err("Cannot add the device to the system\n");
       // cdev_del(&cdev_device);
        unregister_chrdev_region(dev,1);
	return 0;
    }
/*  
 *  creating class for an device is mandatory 
 */
    if (IS_ERR(dev_class = class_create("class_dev"))) {
        pr_err("Cannot create the struct class\n");
	cdev_del(&cdev_device);
        unregister_chrdev_region(dev,1);
	return 0;
    }

/*
 * creating device associate with the dev_class and dev 
 */
    if (IS_ERR(device_create(dev_class, NULL, dev, NULL, "gpio_device"))) {
        pr_err("Cannot create the Device\n");
	cdev_del(&cdev_device);
	class_destroy(dev_class);
        unregister_chrdev_region(dev,1);
	return 0;
    }

/* 
 * To ensure using gpio is valid or not using gpio_is_valid api 
 * if it return zero means it's invalid 
 * if it return non-zero means it's valid
 */
    if (!gpio_is_valid(GPIO_BUTTON)) {
        pr_err("Invalid GPIO: %d\n", GPIO_BUTTON);
	class_destroy(dev_class);
	device_destroy(dev_class,dev);
        cdev_del(&cdev_device);
        unregister_chrdev_region(dev,1);
	return 0;
    }
 
/*  doing operation on any gpio we shold allocate/inform to the gpio hardwarde controller using 
 *  gpio_request() api 
 */    
    if (gpio_request(GPIO_BUTTON, "gpiio_button") < 0) {
        pr_err("Failed to request GPIO: %d\n", GPIO_BUTTON);
	class_destroy(dev_class);
	device_destroy(dev_class,dev);
        cdev_del(&cdev_device);
        unregister_chrdev_region(dev,1);
	return 0;
    }

/*  Configure button as input */
    gpio_direction_input(GPIO_BUTTON);
/*  delay in ms */    
    gpio_set_debounce(GPIO_BUTTON, 200);

/*
 * To ensure using gpio is valid or not using gpio_is_valid api 
 * if it return zero means it's invalid 
 * if it return non-zero means it's valid
 */
    if (!gpio_is_valid(GPIO_LED)) {
        pr_err("Invalid GPIO: %d\n", GPIO_LED); 
        gpio_free(GPIO_BUTTON);
	class_destroy(dev_class);
	device_destroy(dev_class,dev);
        cdev_del(&cdev_device);
        unregister_chrdev_region(dev,1);
	return 0;
    }

/*  doing operation on any gpio we shold allocate/inform to the gpio hardwarde controller using 
 *  gpio_request() api 
 */    
    if (gpio_request(GPIO_LED, "gpio_led") < 0) {
        pr_err("Failed to request GPIO: %d\n", GPIO_LED);
        gpio_free(GPIO_BUTTON);
	class_destroy(dev_class);
	device_destroy(dev_class,dev);
        cdev_del(&cdev_device);
        unregister_chrdev_region(dev,1);
	return 0;
        
    }
    
/* Configure LED GPIO as output  */  
   gpio_direction_output(GPIO_LED, 0);

/*  Get IRQ number for the button GPIO */
    irq_number = gpio_to_irq(GPIO_BUTTON);
    if (irq_number < 0) {
        pr_err("Cannot get IRQ number for GPIO %d\n", GPIO_BUTTON);
        gpio_free(GPIO_BUTTON);
	gpio_free(GPIO_LED);
	class_destroy(dev_class);
	device_destroy(dev_class,dev);
        cdev_del(&cdev_device);
        unregister_chrdev_region(dev,1);
	return 0;
    }

/*  
 *  registering the irq number with associatted interrupt handler in an interrupt descriptor table 
 **/
    if (request_irq(irq_number, irq_handler_fun, IRQF_TRIGGER_FALLING, "gpio_button_irq", NULL) < 0) {
        pr_err("Failed to request IRQ\n");
        gpio_free(GPIO_BUTTON);
	gpio_free(GPIO_LED);
	class_destroy(dev_class);
	device_destroy(dev_class,dev);
        cdev_del(&cdev_device);
        unregister_chrdev_region(dev,1);
	return 0;
    }

    pr_info("Interrupt Driver Loaded successfully......!\n");
    return 0;
}

/*
** Module exit function
*/
static void __exit FunExit(void)
{
        gpio_free(GPIO_BUTTON);
	gpio_free(GPIO_LED);
	class_destroy(dev_class);
	device_destroy(dev_class,dev);
        cdev_del(&cdev_device);
        unregister_chrdev_region(dev,1);
	pr_info("Driver unloaded successfully..........!\n");

}

module_init(FunInit);
module_exit(FunExit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("AKASH");
MODULE_DESCRIPTION("SImple Interrupt Driver..");
MODULE_VERSION("6.0.2");

