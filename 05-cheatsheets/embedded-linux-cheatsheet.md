# 🐧 Embedded Linux & Device Driver Cheatsheet

> Quick reference for U-Boot bootloader commands, Flattened Device Tree (FDT) nodes, Linux Kernel Module (LKM) boilerplate, and kernel debugging utilities.

---

## 1. U-Boot Command Reference

```bash
# Print, modify, and save environment variables
printenv
setenv bootargs 'console=ttyS0,115200 root=/dev/mmcblk0p2 rw rootwait'
saveenv

# Inspect MMC / SD card partitions
mmc list
mmc dev 0
mmc part

# Load Kernel and Device Tree into RAM from FAT partition
fatload mmc 0:1 0x80000000 zImage
fatload mmc 0:1 0x88000000 am335x-boneblack.dtb

# Boot Linux Kernel with Device Tree
bootz 0x80000000 - 0x88000000

# Network boot via TFTP
setenv serverip 192.168.1.100
setenv ipaddr 192.168.1.50
tftp 0x80000000 zImage
```

---

## 2. Flattened Device Tree (DTS / DTB)

### Compile and Decompile:
```bash
# Compile Device Tree Source (.dts) to Binary (.dtb)
dtc -I dts -O dtb -o am335x-custom.dtb am335x-custom.dts

# Decompile Binary (.dtb) back to readable Source (.dts)
dtc -I dtb -O dts -o decompiled.dts target_board.dtb
```

### Typical DTS Node Example (I2C Sensor):
```dts
&i2c1 {
    status = "okay";
    clock-frequency = <400000>; // 400 kHz Fast-Mode

    bmp280: sensor@76 {
        compatible = "bosch,bmp280";
        reg = <0x76>;
        interrupt-parent = <&gpio1>;
        interrupts = <17 IRQ_TYPE_EDGE_RISING>;
        status = "okay";
    };
};
```

---

## 3. Minimal Linux Kernel Character Driver Template

```c
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Embedded Roadmap");
MODULE_DESCRIPTION("Minimal Character Device Driver");
MODULE_VERSION("1.0");

#define DEVICE_NAME "my_char_dev"
static int major_number;
static char device_buffer[256] = "Hello from Kernel Space!\n";

static ssize_t dev_read(struct file *filep, char __user *buffer, size_t len, loff_t *offset) {
    size_t bytes_to_copy = min(len, strlen(device_buffer));
    if (*offset >= strlen(device_buffer)) return 0; // EOF

    if (copy_to_user(buffer, device_buffer, bytes_to_copy)) {
        return -EFAULT;
    }
    *offset += bytes_to_copy;
    return bytes_to_copy;
}

static struct file_operations fops = {
    .owner   = THIS_MODULE,
    .read    = dev_read,
};

static int __init my_module_init(void) {
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) return major_number;
    pr_info("Registered %s with major number %d\n", DEVICE_NAME, major_number);
    return 0;
}

static void __exit my_module_exit(void) {
    unregister_chrdev(major_number, DEVICE_NAME);
    pr_info("Unregistered %s\n", DEVICE_NAME);
}

module_init(my_module_init);
module_exit(my_module_exit);
```

---

## 4. Kernel Module Management & Debugging Commands

```bash
# Insert, list, and remove kernel modules
sudo insmod my_driver.ko
lsmod | grep my_driver
sudo rmmod my_driver

# View live kernel print logs
dmesg -wH

# View dynamic kernel print levels
echo 8 > /proc/sys/kernel/printk

# Direct physical register read/write in Linux user space
devmem2 0x44E07000 word               # Read GPIO register
devmem2 0x44E07194 word 0x00000001    # Write 1 to register
```
