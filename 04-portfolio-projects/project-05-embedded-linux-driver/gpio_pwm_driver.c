#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Embedded Roadmap");
MODULE_DESCRIPTION("Hardware PWM/GPIO Character Device Driver");
MODULE_VERSION("1.0");

#define DEVICE_NAME "pwm_gpio_dev"
#define CLASS_NAME  "pwm_gpio"
#define BUFFER_SIZE 256

static int    major_number;
static char   kernel_buffer[BUFFER_SIZE] = "PWM_STATE=ACTIVE,FREQ=1000,DUTY=50\n";
static short  size_of_message;
static int    number_opens = 0;
static struct class*  pwm_class  = NULL;
static struct device* pwm_device = NULL;

static DEFINE_MUTEX(pwm_driver_mutex);

/* File Operations Prototypes */
static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char __user *, size_t, loff_t *);

static struct file_operations fops = {
    .owner   = THIS_MODULE,
    .open    = dev_open,
    .read    = dev_read,
    .write   = dev_write,
    .release = dev_release,
};

static int __init pwm_driver_init(void) {
    pr_info("PWM_GPIO: Initializing custom driver\n");

    /* Register Major Number dynamically */
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        pr_alert("PWM_GPIO: Failed to register a major number\n");
        return major_number;
    }

    /* Register the device class */
    pwm_class = class_create(CLASS_NAME);
    if (IS_ERR(pwm_class)) {
        unregister_chrdev(major_number, DEVICE_NAME);
        pr_alert("PWM_GPIO: Failed to register device class\n");
        return PTR_ERR(pwm_class);
    }

    /* Register the device driver */
    pwm_device = device_create(pwm_class, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);
    if (IS_ERR(pwm_device)) {
        class_destroy(pwm_class);
        unregister_chrdev(major_number, DEVICE_NAME);
        pr_alert("PWM_GPIO: Failed to create device node\n");
        return PTR_ERR(pwm_device);
    }

    mutex_init(&pwm_driver_mutex);
    pr_info("PWM_GPIO: Device class created correctly at /dev/%s\n", DEVICE_NAME);
    return 0;
}

static void __exit pwm_driver_exit(void) {
    mutex_destroy(&pwm_driver_mutex);
    device_destroy(pwm_class, MKDEV(major_number, 0));
    class_unregister(pwm_class);
    class_destroy(pwm_class);
    unregister_chrdev(major_number, DEVICE_NAME);
    pr_info("PWM_GPIO: Driver unregistered and removed successfully\n");
}

static int dev_open(struct inode *inodep, struct file *filep) {
    if (!mutex_trylock(&pwm_driver_mutex)) {
        pr_alert("PWM_GPIO: Device in use by another process\n");
        return -EBUSY;
    }
    number_opens++;
    pr_info("PWM_GPIO: Device opened %d time(s)\n", number_opens);
    return 0;
}

static ssize_t dev_read(struct file *filep, char __user *buffer, size_t len, loff_t *offset) {
    int error_count = 0;
    size_of_message = strlen(kernel_buffer);

    if (*offset >= size_of_message) return 0; /* EOF */

    size_t bytes_to_copy = min(len, (size_t)(size_of_message - *offset));
    error_count = copy_to_user(buffer, kernel_buffer + *offset, bytes_to_copy);

    if (error_count == 0) {
        *offset += bytes_to_copy;
        return bytes_to_copy;
    } else {
        pr_warn("PWM_GPIO: Failed to send %d characters to user\n", error_count);
        return -EFAULT;
    }
}

static ssize_t dev_write(struct file *filep, const char __user *buffer, size_t len, loff_t *offset) {
    size_t bytes_to_copy = min(len, (size_t)(BUFFER_SIZE - 1));

    if (copy_from_user(kernel_buffer, buffer, bytes_to_copy)) {
        return -EFAULT;
    }

    kernel_buffer[bytes_to_copy] = '\0';
    pr_info("PWM_GPIO: Received command string: %s\n", kernel_buffer);
    return bytes_to_copy;
}

static int dev_release(struct inode *inodep, struct file *filep) {
    mutex_unlock(&pwm_driver_mutex);
    pr_info("PWM_GPIO: Device closed, mutex unlocked\n");
    return 0;
}

module_init(pwm_driver_init);
module_exit(pwm_driver_exit);
