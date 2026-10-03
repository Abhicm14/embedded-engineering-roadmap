# 🐧 Linux Architecture & The 4-Stage Boot Sequence

> From silicon power-on to user-space init on embedded application processors.

---

## 1. The 4-Stage Embedded Linux Boot Sequence

```
1. ROM Code (Boot ROM)
└── Hardwired on silicon by chip manufacturer (TI, NXP, Allwinner, ST).
    Initializes minimal clocks, reads boot pins (SD, eMMC, NAND, UART),
    loads SPL into internal SRAM (since DDR RAM is not yet initialized!).

2. Secondary Program Loader (SPL / MLO)
└── First stage of U-Boot. Initializes external DDR3/DDR4 RAM controllers,
    loads the full U-Boot binary from Flash/SD into DDR RAM, jumps to U-Boot.

3. Das U-Boot (Universal Bootloader)
└── Full interactive bootloader shell with network (TFTP) and storage drivers.
    Loads Linux Kernel image (zImage/uImage) and Device Tree Blob (.dtb) into RAM.
    Passes boot arguments (bootargs: console=ttyS0, root=/dev/mmcblk0p2).
    Executes bootz or bootm.

4. Linux Kernel & User-Space (Init)
└── Decompresses kernel, initializes memory management (MMU), mounts Rootfs,
    starts the first user-space process: PID 1 (/sbin/init or systemd).
```

---

## 2. Kernel Space vs User Space

```
┌────────────────────────────────────────────────────────┐
│ USER SPACE (Unprivileged CPU Ring 3)                   │
│   - Applications, Daemons, Web Servers, GUI            │
│   - Virtual memory isolation per process via MMU       │
├────────────────────────────────────────────────────────┤
│ SYSTEM CALL INTERFACE (sys_open, sys_read, sys_ioctl)  │
├────────────────────────────────────────────────────────┤
│ KERNEL SPACE (Privileged CPU Ring 0)                   │
│   - Process Scheduler, Memory Manager, Network Stack   │
│   - Hardware Device Drivers (I2C, SPI, GPIO, USB)      │
└────────────────────────────────────────────────────────┘
```
