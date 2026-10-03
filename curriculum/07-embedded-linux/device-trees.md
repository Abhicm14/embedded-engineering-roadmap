# 🌲 Flattened Device Trees (FDT / DTS / DTB)

> Decoupling hardware peripheral descriptions from compiled Linux kernel code.

---

## 1. Why Device Trees Exist

In older Linux kernels (pre-3.x), board layouts were hardcoded as messy C structs in the kernel source (`arch/arm/mach-*`). Linus Torvalds famously demanded a cleaner architecture.

The **Device Tree** is a hierarchical data structure describing physical hardware (base addresses, interrupt lines, clock sources, and GPIO mappings) compiled into an independent binary blob (`.dtb`). The Linux kernel remains completely generic and reads the `.dtb` at boot time to dynamically initialize drivers!

---

## 2. Anatomy of a Device Tree Node

```dts
/dts-v1/;

/ {
    model = "My Custom Embedded Board";
    compatible = "mycorp,myboard", "ti,am335x";

    /* I2C Peripheral Controller Node */
    i2c1: i2c@44e0b000 {
        compatible = "ti,omap4-i2c";
        reg = <0x44e0b000 0x1000>;
        interrupts = <70>;
        status = "okay";
        clock-frequency = <400000>; // 400 kHz

        /* Child device: Bosch BMP280 Sensor on this bus */
        bmp280: pressure-sensor@76 {
            compatible = "bosch,bmp280";
            reg = <0x76>; // 7-bit I2C address
            status = "okay";
        };
    };
};
```

- **`compatible` String:** The kernel matches this string (e.g. `"bosch,bmp280"`) against drivers declared in the kernel (`of_match_table`). When a match is found, the kernel calls the driver's `.probe()` function!
