# ⚙️ Setup & Local Environment Guide

This guide explains how to clone this repository, run a local interactive documentation server (Docsify or Python), and install the embedded development toolchain for each step of the learning path.

---

## 🚀 1. Clone & Local Preview

### Option A: Docsify (Recommended for interactive documentation)
Docsify renders Markdown files on the fly without building static HTML.

```bash
# 1. Install docsify-cli globally via npm
npm install -g docsify-cli

# 2. Clone the repository
git clone https://github.com/Abhicm14/embedded-engineering-roadmap.git
cd embedded-engineering-roadmap

# 3. Start local documentation server
docsify serve .

# 4. Open in browser at http://localhost:3000
```

### Option B: Python HTTP Server (Zero-install fallback)
```bash
# Clone the repository
git clone https://github.com/Abhicm14/embedded-engineering-roadmap.git
cd embedded-engineering-roadmap

# Run built-in HTTP server
python -m http.server 3000

# Open in browser at http://localhost:3000
```

---

## 🧰 2. Per-Step Toolchain Installation Guide

### Step 1 & 2: C Programming & Computer Fundamentals
- **Host Compiler (GCC / Clang):**
  - **Linux (Ubuntu/Debian):** `sudo apt update && sudo apt install -y build-essential gcc gdb make`
  - **macOS:** `xcode-select --install`
  - **Windows:** Install GCC via [MSYS2](https://www.msys2.org/) (`pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-gdb make`) or Scoop (`scoop install gcc make`).
- **Simulators:**
  - [Wokwi Web Simulator](https://wokwi.com/) (zero local setup)
  - [SimulIDE](https://simulide.com/) (circuit and MCU simulator)

---

### Step 3, 4 & 5: STM32 Bare-Metal, Peripherals & Debugging
- **Cross-Compiler:**
  - GNU Arm Embedded Toolchain (`arm-none-eabi-gcc`):
    - **Linux:** `sudo apt install -y gcc-arm-none-eabi binutils-arm-none-eabi gdb-multiarch`
    - **macOS:** `brew install arm-none-eabi-gcc`
    - **Windows:** Download official binary from [ARM Developer](https://developer.arm.com/downloads/-/gnu-rm) or Scoop: `scoop install arm-none-eabi-gcc`.
- **Flashing & Debug Probes:**
  - [OpenOCD](http://openocd.org/): `sudo apt install openocd` or `scoop install openocd`.
  - [STM32CubeProgrammer](https://www.st.com/en/development-tools/stm32cubeprog.html) (official ST flash utility).
- **Logic Analyzer Software:**
  - [PulseView (sigrok)](https://sigrok.org/wiki/PulseView) for USB 8-channel 24MHz logic analyzers.
  - [Saleae Logic 2](https://www.saleae.com/downloads/).

---

### Step 6: FreeRTOS & RTOS
- FreeRTOS source files are included directly in C projects or available as git submodules.
- Simulator: QEMU ARM System Emulation:
  - **Linux:** `sudo apt install -y qemu-system-arm`
  - Emulates ARM Cortex-M3/M4 (e.g. `lm3s6965evb` or `stm32vldiscovery`).

---

### Step 7: Embedded Linux
- **Cross-Toolchain:** `sudo apt install -y gcc-arm-linux-gnueabihf gcc-aarch64-linux-gnu`
- **Build Systems:**
  - Buildroot: Clone from `https://github.com/buildroot/buildroot`
  - Yocto Project / Poky: Requires Ubuntu LTS with 100GB+ free disk space.
- **Terminal Emulator:** `tio`, `minicom`, or `picocom` (`sudo apt install tio`).

---

### Step 8: Specializations (Automotive / IoT / TinyML)
- **ESP32 & IoT:** [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/) or Arduino Core.
- **Automotive CAN:** `can-utils` (`sudo apt install can-utils`) on Linux with SocketCAN.
- **TinyML:** Python 3.10+ with TensorFlow, NumPy, and CMSIS-NN kernels.
