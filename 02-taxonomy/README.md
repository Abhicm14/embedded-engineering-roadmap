# 🏛️ Embedded Systems Engineering Taxonomy

> A comprehensive, three-dimensional topic taxonomy defining the core competencies, knowledge trees, and skill boundaries required of a professional embedded systems engineer.

---

## 🎯 The Three Pillars of Embedded Mastery

Embedded engineering lives at the precise intersection of physical reality and algorithmic logic. True mastery requires balanced expertise across three pillars:

```
                                  ┌────────────────────────┐
                                  │   EMBEDDED ENGINEER    │
                                  └───────────┬────────────┘
                        ┌─────────────────────┼─────────────────────┐
                        ▼                     ▼                     ▼
             ┌─────────────────────┐┌─────────────────────┐┌─────────────────────┐
             │     1. SOFTWARE     ││     2. HARDWARE     ││   3. SOFT-SKILLS    │
             ├─────────────────────┤├─────────────────────┤├─────────────────────┤
             │ • Embedded C/C++    ││ • Digital Circuits  ││ • Datasheet Analysis│
             │ • Assembly & Startup││ • Analog & Power    ││ • Systematic Debug  │
             │ • Memory & Linkers  ││ • MCU Architectures ││ • Git & Code Reviews│
             │ • RTOS & Multitask  ││ • Schematics & PCB  ││ • System Design    │
             │ • Linux & Drivers   ││ • Signal Integrity  ││ • Tech Writing      │
             │ • Protocols & Stacks││ • Lab Instruments   ││ • Cross-Discipline  │
             └─────────────────────┘└─────────────────────┘└─────────────────────┘
```

---

## 🧭 Taxonomy Directory

| Document | Primary Focus | Industry Applications |
| :--- | :--- | :--- |
| [**Software Taxonomy**](software-taxonomy.md) | Programming languages (C, C++, Rust, ASM), toolchains, RTOS, Linux kernel, communication stacks, defensive coding, testing frameworks, and algorithms. | Firmware development, BSP engineering, RTOS multitasking, Linux driver authoring. |
| [**Hardware Taxonomy**](hardware-taxonomy.md) | Digital logic, passive/active analog circuits, power architectures (LDO, Buck, Boost), microcontrollers (ARM, RISC-V, ESP32), schematic capture, PCB layout, and lab test gear. | Hardware bringup, board support, peripheral interfacing, signal integrity analysis. |
| [**Soft-Skills & Practices**](soft-skills-taxonomy.md) | Datasheet comprehension, structured root-cause debugging, version control workflows, architectural documentation, interview mastery, and cross-functional team collaboration. | Engineering leadership, defect diagnosis, specification design, technical interviews. |

---

## 📊 Skill Proficiency Tiers

Each topic within our taxonomy is classified into one of three mastery levels:

1. **Level 1: Foundational (Fresher / Junior)**
   - Knows the definition, basic formulas, and can implement simple standalone drivers or circuits with guidance.
2. **Level 2: Proficient (Mid-Level Engineer)**
   - Can independently design, debug edge cases, write interrupt-driven/DMA firmware, and analyze oscilloscope waveforms without supervision.
3. **Level 3: Expert (Senior / Staff Architect)**
   - Architecting fault-tolerant distributed systems, performing signal integrity simulations, writing custom Linux drivers, achieving ISO 26262 / MISRA compliance, and mentoring junior engineers.
