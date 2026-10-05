# 🛠️ Step 5: Engineering Workflow, Debugging & Tooling

> **Pillar:** FIRMWARE (STM32 + Peripherals)  
> **Core Rule:** *"Do not learn peripherals only as APIs. Understand the register, electrical signal, timing diagram, protocol transaction and failure modes behind each API call."*  
> **Prerequisites:** Steps 1–4. Refer to our [**📖 Beginner Glossary (`cheatsheets/glossary.md`)**](../../cheatsheets/glossary.md) for testing and debugging terminology.

---

## 🎯 Learning Objectives

Great embedded engineering is distinguished not by writing code, but by diagnosing complex, intermittent defects across the hardware/software boundary. By the end of this module, you should be able to:
1. Debug target hardware using SWD/JTAG with GDB: breakpoints, hardware watchpoints, inspecting call stacks, register views, and live memory dumps.
2. Structure serial logging pipelines with severity levels (`LOG_INFO`, `LOG_WARN`, `LOG_ERR`) without stalling execution.
3. Perform HardFault triage: decode ARM Cortex-M fault status registers (`CFSR`, `HFSR`, `BFAR`), unstack registers, and correlate addresses with the `.map` file.
4. Master Git workflows for firmware: atomic commits, branch hygiene, rebasing, pull request reviews, and writing professional READMEs.
5. Set up reproducible automated builds and unit test harnesses.

---

## 🧭 Topic Guides in This Module

| Topic Document | Type | Key Concepts |
| :--- | :---: | :--- |
| [**1. Debugging with GDB & OpenOCD**](debugging-and-gdb.md) | `[INTUITION]` | Breakpoints, data watchpoints, memory dumps, inspecting peripheral registers. |
| [**2. Logic Analyzers vs Oscilloscopes**](logic-analyzers-and-scopes.md) | `[INTUITION]` | When to use DSO (voltage/analog) vs Logic Analyzer (protocol decoding). |
| [**3. HardFault Triage & Map File Analysis**](hard-fault-analysis.md) | `[DEEP DIVE]` | Decoding `CFSR`, `BFAR`, unstacking `PC` and `LR`, locating crashing C line. |
| [**4. Git & Version Control for Firmware**](git-and-version-control.md) | `[INTUITION]` | `.gitignore` rules, feature branches, rebasing, code reviews, PR etiquette. |
| [**5. Testing & Reproducible Builds**](testing-and-builds.md) | `[DEEP DIVE]` | Unit testing with Unity/CMock, hardware-in-the-loop (HIL), CI/CD pipelines. |

---

## ✅ Step 5 Completion Checklist

- [ ] Can set a hardware data watchpoint in GDB to stop execution when a global variable is modified.
- [ ] Can extract the crashing instruction address from a HardFault stack dump and find the source code line in the `.map` file.
- [ ] Understands why committing `.hex` or `.elf` binaries into Git repositories is prohibited.
- [ ] Can decode an SPI bus trace in PulseView to verify setup/hold timing.

➡️ **Next Step:** [Step 6: FreeRTOS & RTOS Architecture](../06-freertos-rtos/README.md)
