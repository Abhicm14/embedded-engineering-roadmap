# ⏱️ RTOS Tasks, Scheduler Mechanics & Context Switching

> Task Control Blocks, Task States, SysTick time slicing, and the assembly mechanics of PendSV.

---

## 1. Task State Machine

In a preemptive RTOS, a task is in one of four states at any moment:

```
            ┌─────────────────┐
            │   SUSPENDED     │
            └────────▲────────┘
                     │ vTaskSuspend() / vTaskResume()
                     ▼
           ┌───────────────────┐
           │      READY        │ ◄─── Unblocked by Queue/Semaphore/Timeout
           └─────────▲─────────┘
   Yield / Preempted │ │ Scheduler selects highest priority task
                     │ ▼
           ┌───────────────────┐
           │     RUNNING       │ ◄─── Currently executing on CPU core
           └─────────┬─────────┘
                     │ vTaskDelay() / Waiting on Queue
                     ▼
           ┌───────────────────┐
           │     BLOCKED       │ ───► Waiting on event or timeout
           └───────────────────┘
```

---

## 2. Anatomy of the Context Switch (PendSV)

The ARM Cortex-M architecture provides a dedicated software interrupt exception called **`PendSV` (Pendable Service Call)** designed specifically for RTOS context switching:

1. **Trigger:** The periodic SysTick interrupt detects that a higher priority task is ready to run, and sets the `PENDSVSET` bit in `SCB->ICSR`.
2. **Delayed Execution:** `PendSV` has the lowest interrupt priority. It waits until all active hardware interrupts finish, ensuring the context switch does not delay urgent hardware events.
3. **Hardware Stacking:** When entering `PendSV_Handler`, the CPU hardware pushes `R0-R3, R12, LR, PC, xPSR` to the running task's stack (`PSP`).
4. **Software Stacking:** In assembly, the RTOS kernel pushes the remaining registers (`R4-R11`) onto the task stack.
5. **Pointer Update:** The task's stack pointer (`PSP`) is saved into its **Task Control Block (TCB)**:
   ```c
   pxCurrentTCB->pxTopOfStack = psp;
   ```
6. **Task Selection:** The scheduler selects the next highest priority ready task:
   ```c
   vTaskSwitchContext();
   ```
7. **Restoration:** The kernel loads the new task's stack pointer, pops `R4-R11`, and executes `BX LR`. Hardware restores the remaining registers and execution resumes in the new task!
