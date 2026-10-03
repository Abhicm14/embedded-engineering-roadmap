# 🟠 Month 5: Embedded Linux & System Architecture

> Focus: Cross-toolchains, bootloaders (U-Boot), Flattened Device Trees (FDT), Linux Kernel Modules (LKM), character drivers, and user-space POSIX programming.

---

## 🎯 Monthly Objectives
1. Understand the full Embedded Linux boot sequence: ROM Bootloader $\rightarrow$ SPL $\rightarrow$ U-Boot $\rightarrow$ Kernel $\rightarrow$ Rootfs.
2. Write and compile Flattened Device Tree Source (`.dts`) nodes for custom hardware peripherals.
3. Develop, cross-compile, and insert a custom Linux Kernel Character Device Driver.
4. Master safe user/kernel space memory transfers via `copy_to_user` and `copy_from_user`.
5. Complete **Project 5: Embedded Linux Custom Character Driver & POSIX App**.

---

## 📅 Weekly Breakdown

### Week 17: Embedded Linux Architecture & Cross-Compilation
- Microprocessor architecture (MMU, caches, virtual memory).
- Cross-toolchains (`arm-linux-gnueabihf-gcc`), glibc vs musl, minimal rootfs with BusyBox.
- Lab: Boot a minimal custom Linux kernel image inside QEMU.

### Week 18: U-Boot & Flattened Device Tree (FDT)
- U-Boot environment variables and boot commands.
- Device Tree nodes, properties, memory mappings, and `compatible` strings.
- Lab: Add a custom I2C sensor node to a board device tree and compile to `.dtb`.

### Week 19: Linux Kernel Modules & Character Drivers
- Kernel space vs user space memory isolation.
- `module_init()`, `module_exit()`, `file_operations` struct, and major/minor numbers.
- Lab: Write and load a basic character device driver `/dev/my_device`.

### Week 20: Hardware Control, Sysfs & Project 5 Milestone
- Creating `sysfs` attributes and handling POSIX system calls.
- User-space applications using standard file I/O (`open`, `read`, `write`, `ioctl`).
- Lab: Deliver [Project 5](../../04-portfolio-projects/project-05-embedded-linux-driver/).
