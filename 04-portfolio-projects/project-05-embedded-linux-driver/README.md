# 📁 Project 05: Embedded Linux Custom Character Driver & App

> Target: Raspberry Pi / BeagleBone Black / QEMU ARM Linux  
> Language: Linux Kernel C (C99/GNU) & User-Space POSIX C  
> Key Technologies: Linux Kernel Module (LKM), Character Devices, `file_operations`, `sysfs` Attributes, `copy_to_user` / `copy_from_user`

---

## 🎯 Project Overview

This project bridges the gap between bare-metal firmware and application-processor operating systems. You will develop a loadable Linux kernel module (LKM) that creates a virtual character device node (`/dev/pwm_gpio_dev`) allowing user-space applications to control hardware timing registers safely through POSIX system calls (`open`, `read`, `write`, `ioctl`).

### Architectural Highlights:
1. **User/Kernel Space Boundary:** Safely exchanging memory buffers between non-privileged user space and privileged ring 0 kernel space using `copy_from_user()` and `copy_to_user()` with strict bounds checks.
2. **Kernel Mutex Synchronization:** Guarding shared hardware state from concurrent user processes using `DEFINE_MUTEX`.
3. **Sysfs Interface:** Exposing operational metrics (duty cycle, frequency, operation count) to user space under `/sys/class/pwm_gpio/`.
4. **POSIX User-Space Daemon:** An asynchronous control app that issues non-blocking reads and writes to the kernel character device node.

---

## 🏗️ Architecture Diagram

```
┌────────────────────────────────────────────────────────┐
│                      USER SPACE                        │
│   ┌────────────────────────────────────────────────┐   │
│   │ app_userspace (POSIX C application)            │   │
│   │   fd = open("/dev/pwm_gpio_dev", O_RDWR);      │   │
│   │   write(fd, "DUTY=75", 7);                     │   │
│   └───────────────────────┬────────────────────────┘   │
└───────────────────────────┼────────────────────────────┘
                            │ System Call Interface (syscall)
┌───────────────────────────┼────────────────────────────┐
│ KERNEL SPACE              ▼                            │
│   ┌────────────────────────────────────────────────┐   │
│   │ Character Device Driver (gpio_pwm_driver.ko)   │   │
│   │   - dev_open()                                 │   │
│   │   - dev_write() -> copy_from_user()           │   │
│   │   - Mutex lock & unlock                        │   │
│   │   - Sysfs attribute: /sys/class/...            │   │
│   └───────────────────────┬────────────────────────┘   │
│                           │                            │
│                           ▼ (Hardware I/O Memory)      │
│                [ Physical SoC Registers ]              │
└────────────────────────────────────────────────────────┘
```

---

## 🛠️ Compilation & Testing

```bash
# 1. Compile kernel module against running kernel headers
make

# 2. Insert kernel module
sudo insmod gpio_pwm_driver.ko

# 3. Check kernel log messages
dmesg | tail -n 10

# 4. Compile and run user-space test tool
gcc app_userspace.c -o app_userspace
./app_userspace
```
