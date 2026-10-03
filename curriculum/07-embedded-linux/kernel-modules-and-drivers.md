# 🔌 Linux Kernel Modules (LKM) & Character Device Drivers

> Authoring in-tree/out-of-tree drivers, file operations, user-kernel memory boundaries, and locking.

---

## 1. The `file_operations` Interface

In UNIX, everything is treated as a file. A character device driver exposes physical hardware by implementing standard VFS (Virtual File System) callbacks:

```c
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

static int dev_open(struct inode *inodep, struct file *filep);
static int dev_release(struct inode *inodep, struct file *filep);
static ssize_t dev_read(struct file *filep, char __user *buffer, size_t len, loff_t *offset);
static ssize_t dev_write(struct file *filep, const char __user *buffer, size_t len, loff_t *offset);

static struct file_operations fops = {
    .owner   = THIS_MODULE,
    .open    = dev_open,
    .read    = dev_read,
    .write   = dev_write,
    .release = dev_release,
};
```

---

## 2. Crossing the Memory Boundary Safely

User space pointers cannot be directly dereferenced in kernel space:
1. User pointers might be invalid or NULL.
2. User memory might be paged out to swap disk.
3. Directly dereferencing user pointers allows malicious processes to read/write arbitrary kernel memory.

### Safe Transfer APIs:
- **`copy_to_user(char __user *to, const void *from, unsigned long n)`:** Safely sends data from kernel buffers to user space.
- **`copy_from_user(void *to, const char __user *from, unsigned long n)`:** Safely receives user data into kernel memory.
