# ⚙️ Cross-Compilation & Root Filesystems (Buildroot vs Yocto)

> Building minimal embedded distributions, toolchain triplets, and rootfs generators.

---

## 1. Toolchain Triplet Structure

When compiling code for an embedded Linux board on an x86 PC, you use a cross-compiler designated by a **triplet**:

```
arm-linux-gnueabihf-gcc
 │     │      │     │
 │     │      │     └── ABI: Hard-Float (Uses hardware FPU registers)
 │     │      └──────── Standard C Library: GNU libc (glibc)
 │     └─────────────── Operating System: Linux
 └───────────────────── Architecture: ARM (32-bit Cortex-A)
```

For 64-bit ARM boards (Raspberry Pi 4/5): `aarch64-linux-gnu-gcc`.

---

## 2. Generating Embedded Linux: Buildroot vs Yocto

Embedded systems do not install general-purpose desktop distributions (like Ubuntu Desktop). Instead, engineers build customized, stripped-down images:

| Dimension | Buildroot | Yocto Project (Poky) |
| :--- | :--- | :--- |
| **Complexity** | Simple, easy to learn in a day (`make menuconfig`). | Highly complex, steep learning curve. |
| **Build Artifact** | Generates a fixed single firmware image (`rootfs.tar`). | Generates a complete package manager repository (RPM/IPK/DEB) and custom Linux distribution. |
| **Customization** | Kconfig-based menu options. | BitBake recipes and layered architecture (`meta-layers`). |
| **Ideal Project** | Small teams, single board appliances, fast turnaround. | Enterprise products, automotive infotainment, long-term multi-board fleets. |
