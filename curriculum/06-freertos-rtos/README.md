# ⏱️ Step 6: Real-Time Operating Systems (FreeRTOS & RTOS Internals) [DEEP DIVE]

> **Pillar:** SYSTEMS (RTOS + Linux)  
> **Core Rule:** *"Do not learn peripherals only as APIs. Understand the register, electrical signal, timing diagram, protocol transaction and failure modes behind each API call."*  
> **Prerequisites:** Steps 1–5 and at least 2 working bare-metal projects. Check our [**Beginner Glossary**](../../cheatsheets/glossary.md) for terminology.

> [!WARNING]
> **DEEP DIVE — come back after you have built a few projects.**  
> Real-Time Operating System kernel internals (Task Control Blocks, assembly context switching via `PendSV`, memory heap allocation models, and priority inversion recovery) are advanced systems topics. If you are a beginner, master bare-metal GPIO, timers, and UART drivers first before tackling multi-threaded kernels!

## 🎯 Learning Objectives

When firmware complexity grows to handle concurrent displays, networking, sensor telemetry, and motor control, superloops fall apart. By the end of this module, you should be able to:
1. Explain the internal mechanics of a preemptive real-time kernel: Task Control Block (TCB), task states, and context switching via `PendSV`.
2. Size task stacks accurately and detect stack overflows using watermarking and memory guards.
3. Synchronize concurrent tasks using Queues, Binary/Counting Semaphores, Mutexes, Event Groups, and Direct-to-Task Notifications.
4. Prevent and resolve Unbounded Priority Inversion using the **Priority Inheritance Protocol (PIP)**.
5. Communicate safely between hardware ISRs and RTOS tasks (`*FromISR` APIs).
6. Compare FreeRTOS against modern alternative kernels: **Zephyr RTOS**, **RT-Thread**, **Apache NuttX**, **Arm Mbed OS**, and **Eclipse ThreadX (Azure RTOS)**.
7. Complete **Project 5: FreeRTOS Environmental Monitor**.

---

## 🧭 Topic Guides in This Module

| Topic Document | Key Concepts |
| :--- | :--- |
| [**1. Tasks & Scheduler Internals**](tasks-and-scheduler.md) | Task states (Ready/Running/Blocked/Suspended), TCB, `PendSV` context switch, SysTick slicing. |
| [**2. Synchronization Primitives**](synchronization-primitives.md) | Semaphores, Mutexes, Event Groups, Task Notifications, ISR-to-Task handoffs. |
| [**3. Queues & Inter-Task Communication**](queues-and-data-passing.md) | Thread-safe FIFO data pipelines, pass-by-copy vs pass-by-pointer, blocking timeouts. |
| [**4. Priority Inversion & Mars Pathfinder**](priority-inversion.md) | Unbounded inversion hazard, Priority Inheritance Protocol (PIP), deadlocks. |
| [**5. The Broader RTOS Ecosystem**](other-rtos-ecosystems.md) | Architectural comparison: FreeRTOS vs Zephyr vs RT-Thread vs NuttX vs ThreadX. |

---

## 🛠️ Associated Project

- **[Project 5: FreeRTOS Environmental Monitor](../../projects/intermediate.md#project-5-freertos-environmental-monitor)**

---

## ✅ Step 6 Completion Checklist

- [ ] Can explain step-by-step how an RTOS context switch saves and restores CPU registers via `PendSV`.
- [ ] Understands why calling standard FreeRTOS API functions inside an ISR causes system corruption.
- [ ] Can configure `FreeRTOSConfig.h` memory allocations (`heap_1` through `heap_5`).
- [ ] Can analyze a multi-threaded system to eliminate deadlock and starvation conditions.

➡️ **Next Step:** [Step 7: Embedded Linux & System Architecture](../07-embedded-linux/README.md)
