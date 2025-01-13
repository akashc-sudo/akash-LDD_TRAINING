#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/of.h>           // For Device Tree functions
#include <linux/of_gpio.h>      // For GPIO handling in Device Tree
#include <linux/delay.h>        // For delay functions (e.g., msleep)

static int led_gpio = -1;  // GPIO pin for LED (default -1)
struct device_node *node,*child;
static int __init gpio_led_init(void)
{
    int ret;
    //struct device_node *node;

    // Find the device tree node for 'my_gpio'
    node = of_find_node_by_name(NULL,"gpio");
    if(node){
    for_each_child_of_node(node, child) {
        pr_info("name of child = %s\n",child->name);
        if (of_node_name_eq(child, "akash")) {
            pr_info("searching node got in DT %s\n",child->name);
            break;
        } 
    }
   }
      else if(!node)
      {
          pr_info("not found\n");
          return -ENODEV;
      }
// Get the GPIO from the device tree
   led_gpio = of_get_named_gpio(child, "gpios", 0);
    if (led_gpio < 0) {
        pr_err("Failed to get GPIO from Device Tree\n");
        return led_gpio;  // Return error code if GPIO is not found
    }

    pr_info("LED GPIO found: %d\n", led_gpio);

    // Request the GPIO and set it as output
    ret = gpio_request(led_gpio, "LED GPIO");
    if (ret) {
        pr_err("Failed to request GPIO %d\n", led_gpio);
        return ret;  // Return error code if GPIO request fails
    }

    ret = gpio_direction_output(led_gpio, 0);  // Start with LED OFF
    if (ret) {
        pr_err("Failed to set GPIO direction\n");
        gpio_free(led_gpio);  // Free GPIO resources if direction setting fails
        return ret;
    }

    // Toggle the LED (turn ON and then OFF after a delay)
    gpio_set_value(led_gpio, 1);  // Turn LED ON
    msleep(1000);  // Wait for 1 second
    gpio_set_value(led_gpio, 0);  // Turn LED OFF

    return 0;  // Successful initialization*/


    return 0;
}

static void __exit gpio_led_exit(void)
{
    if(node)
    {
        of_node_put(node);
    }

    if (led_gpio >= 0) {
        gpio_set_value(led_gpio, 0);  // Ensure the LED is OFF
        gpio_free(led_gpio);  // Free the GPIO resources
    }

    pr_info("GPIO LED driver removed\n");
}

module_init(gpio_led_init);
module_exit(gpio_led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple driver for controlling GPIO LED using Device Tree");
