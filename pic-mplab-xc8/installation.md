# PIC Microcontroller Installation Guide

> A step-by-step guide to installing the MPLAB X IDE, XC8 Compiler, and PICSimLab simulator for the PIC16F877A microcontroller.

## Prerequisites

- **Operating System:** Windows 10/11 (64-bit) or macOS 12+
- **Internet Connection:** Required for downloading the latest toolchain versions
- **Hardware:** None required for software installation

## Step 1: Download MPLAB X IDE

1. Visit the official MPLAB X download page: https://www.microchip.com/mplab/mplab-x
2. Download the **MPLAB X IDE v5.35** installer (approx. 1.5 GB)
3. Run the installer and accept the default installation path
4. During installation, ensure the **PIC16F877A** support is included (usually checked by default)

## Step 2: Install XC8 Compiler

1. After MPLAB X installation completes, open MPLAB X IDE
2. Go to **File → Options → Compiler Settings**
3. Under **Compilers**, select **XC8**
4. Ensure the following are selected:
   - **Version:** XC8 v2.36+ (Free license)
   - **Architecture:** 8-bit PIC16F877A
   - **Language:** C (ANSI)
5. Click **OK** to save

## Step 3: Install PICSimLab Simulator

1. Download **PICSimLab v0.7** from: https://sourceforge.net/projects/picsimlab/
2. Run the installer (portable version recommended)
3. During installation, select the **PIC16F877A** board configuration
4. The simulator will automatically detect the device and create a demo board

## Step 4: Verify Installation

### MPLAB X IDE
- Open MPLAB X IDE
- Go to **Help → About MPLAB X** – should show version 5.x.x
- Create a new project: **File → New → Project → Microchip Embedded → Standalone Project**
- Select **PIC16F877A** as the target device

### XC8 Compiler
- Open the XC8 compiler window
- Try compiling a simple "Hello World" program
- Should produce a `.c` file with no errors

### PICSimLab
- Open the simulator
- Select **PIC16F877A Demo Board** from the board browser
- Click **Start Simulation** – the LED on RB0 should light up

## Step 5: Common Issues & Solutions

| Issue | Cause | Solution |
|-------|-------|----------|
| MPLAB X won't launch | Port conflict | Close other instances of MPLAB X or another IDE |
| XC8 says "No compiler found" | Compiler not selected | Go to **Options → Compiler Settings → XC8** and ensure it's enabled |
| PICSimLab not starting | Port not detected | Verify USB connection, try reinstalling the simulator |
| Project builds but runs nothing | Missing linker script | Check `pic-mplab-xc8/README.md` for linker script location |

## Step 6: Hardware Setup (Optional)

To test the firmware on real hardware:

1. **Connect the PIC16F877A** to your development board (e.g., Digispark, PICkit, or custom PCB)
2. **Pinout Reference:**
   - **RB0** (Pin 33) → LED (with 220Ω resistor)
   - **RB1** (Pin 34) → Button (with 10k pull-up)
   - **RA0** (Pin 29) → Potentiometer wiper (analog input)
3. **Power Supply:** 5V DC (3.6V–5.5V acceptable)
4. **Connect PICSimLab** to the hardware via USB or JTAG

## Troubleshooting

- **Build fails with "undefined symbol":** Check that all required libraries are installed (see ROADMAP.md for toolchain setup)
- **Simulation hangs:** Restart both MPLAB X and PICSimLab; ensure no other processes are using the PICSimLab port
- **LED doesn't blink:** Verify the LED is connected to RB0 and the XC8 code is compiled correctly

## Quick Reference

| Tool | Location | Purpose |
|------|----------|---------|
| MPLAB X IDE | `C:\Program Files (x86)\Microchip\MPLAB X\` | Project creation, debugging, compilation |
| XC8 Compiler | `C:\Program Files (x86)\Microchip\XC8\` | C code compilation |
| PICSimLab | `C:\PICSimLab\` | Real-time simulation and debugging |
| PIC16F877A Board | `C:\PIC16F877A\` | Device-specific configurations |

## Further Reading

- [MPLAB X User Guide](https://www.microchip.com/mplab/mplab-x/user-guide)
- [XC8 Compiler Manual](https://www.microchip.com/mplab/compilers/xc8)
- [PICSimLab Documentation](https://picsimlab.sourceforge.net/docs/)

## Support

- **Microchip Community Forum:** https://community.microchip.com
- **Stack Overflow:** Tag with `microchip`, `xc8`, `picsimlab`
- **GitHub Issues:** https://github.com/microchip/mplab-x/issues

*Last updated: 2026-03-28*
