# 🟣 Month 4: Real-Time Operating Systems (FreeRTOS)

> Focus: Real-time multitasking, scheduler architecture, context switching, queues, semaphores, mutexes, and priority inversion prevention.

---

## 🎯 Monthly Objectives
1. Understand why and when to transition from a bare-metal superloop to an RTOS.
2. Master FreeRTOS kernel mechanics: Task Control Block (TCB), task states, and context switching via `PendSV`.
3. Implement thread-safe inter-task communication using FreeRTOS Queues.
4. Master resource protection using Mutexes with Priority Inheritance Protocol (PIP).
5. Complete **Project 3: Multitasking Environmental Data Logger with FreeRTOS**.

---

## 📅 Weekly Breakdown

### Week 13: RTOS Mechanics & Scheduler Internals
- Preemptive vs cooperative scheduling, time-slicing with SysTick.
- Task states (Running, Ready, Blocked, Suspended).
- Lab: Port FreeRTOS kernel to STM32 and run multiple concurrent blinking tasks.

### Week 14: Inter-Process Communication (Queues)
- Thread-safe FIFO data passing, blocking with timeout, `xQueueSendFromISR`.
- Producer-Consumer design pattern.
- Lab: Build an asynchronous sensor-to-logger queue pipeline.

### Week 15: Synchronization: Mutexes vs Semaphores
- Binary semaphores for ISR unblocking vs Mutexes for resource protection.
- Unbounded Priority Inversion hazard and the Priority Inheritance Protocol (PIP).
- Lab: Replicate priority inversion and resolve it with a FreeRTOS mutex.

### Week 16: Advanced RTOS Services & Project 3 Milestone
- Software timers, event groups, and Direct-to-Task notifications.
- Low-power tickless idle operation.
- Lab: Deliver [Project 3](../../04-portfolio-projects/project-03-freertos-data-logger/).
