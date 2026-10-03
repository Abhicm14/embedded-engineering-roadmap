# 🐧 Step 7: Embedded Linux & System Architecture

> **Pillar:** SYSTEMS (RTOS + Linux)  
> **Core Rule:** *"Do not learn peripherals only as APIs. Understand the register, electrical signal, timing diagram, protocol transaction and failure modes behind each API call."*

---

## 🎯 Learning Objectives

When systems require rich graphical user interfaces, cellular/Ethernet routing, multi-gigabyte filesystems, and complex camera/audio pipelines, microprocessors running Embedded Linux become the industry standard. By the end of this module, you should be able to:
1. Explain the 4-stage Linux boot sequence: ROM Bootloader $\rightarrow$ SPL $\rightarrow$ U-Boot $\rightarrow$ Kernel $\rightarrow$ Rootfs (`init`/`systemd`).
2. Set up cross-compilation toolchains (`arm-linux-gnueabihf-gcc`) and build custom root filesystems using **Buildroot** and understand **Yocto Project** recipes.
3. Author Flattened Device Tree (`.dts`) nodes mapping physical hardware peripherals to Linux kernel drivers.
4. Develop, insert, and debug custom Linux Kernel Modules (LKM) and character device drivers (`file_operations`, `sysfs`, `copy_to_user`).
5. Write robust user-space C software using POSIX threads (`pthreads`), sockets, and modern `/dev/gpiochip` (`libgpiod`).

---

## 🧭 Topic Guides in This Module

| Topic Document | Key Concepts |
| :--- | :--- |
| [**1. Linux Architecture & Boot Flow**](linux-architecture-and-boot.md) | ROM $\rightarrow$ SPL $\rightarrow$ U-Boot $\rightarrow$ Kernel $\rightarrow$ Device Tree $\rightarrow$ Rootfs $\rightarrow$ Init. |
| [**2. Cross-Compilation & Rootfs (Buildroot/Yocto)**](cross-compilation-and-rootfs.md) | Toolchains, C standard libraries (glibc vs musl), BusyBox, Buildroot vs Yocto. |
| [**3. Flattened Device Trees (DTS/DTB)**](device-trees.md) | Hardware description separation, compatible strings, pin multiplexing (`pinctrl`). |
| [**4. Kernel Modules & Character Drivers**](kernel-modules-and-drivers.md) | LKM structure, `file_operations`, `ioctl`, `copy_to_user`, kernel concurrency. |
| [**5. POSIX Systems Programming & Hardware I/O**](userspace-posix-and-peripherals.md) | `libgpiod`, I2C/SPI dev interfaces, sockets, `pthreads`, IPC. |

---

## ✅ Step 7 Completion Checklist

- [ ] Can boot a minimal Linux kernel with BusyBox inside QEMU ARM.
- [ ] Understands how U-Boot loads the kernel image and device tree blob into RAM.
- [ ] Can write and compile a device tree overlay for an I2C or SPI sensor.
- [ ] Can write a kernel character driver and verify memory exchange with `copy_from_user()`.

➡️ **Next Step:** [Step 8: Specializations, Career & Job Readiness](../08-specialization-career/README.md)
