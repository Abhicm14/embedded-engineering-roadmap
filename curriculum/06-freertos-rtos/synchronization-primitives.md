# 🔒 Synchronization Primitives: Semaphores, Mutexes & Notifications

> Managing concurrency, resource sharing, event signaling, and ISR handoffs in FreeRTOS.

---

## 1. Binary Semaphores vs Mutexes

Many beginners confuse Binary Semaphores with Mutexes because both seem to act as binary flags. However, their engineering intents and behaviors are fundamentally different:

| Feature | Binary Semaphore | Mutex (Mutual Exclusion) |
| :--- | :--- | :--- |
| **Primary Purpose** | **Signaling / Synchronization** (Task A tells Task B an event occurred). | **Resource Locking** (Protecting shared memory, bus, or peripheral). |
| **Initial State** | Typically starts **Empty (0)**. | Typically starts **Available (1)**. |
| **Ownership** | No ownership concept. Can be Given by an ISR and Taken by a Task. | **Strict Ownership**. The task that Takes the mutex MUST be the task that Gives it back! |
| **Priority Inversion**| ❌ No Priority Inheritance protection. | ✅ Includes **Priority Inheritance Protocol (PIP)**. |

---

## 2. ISR-to-Task Signaling (`*FromISR`)

Never perform long computation or blocking calls inside an ISR. Defer processing to a high-priority worker task using a semaphore or direct task notification:

```c
// Worker Task (Priority: High)
void vWorkerTask(void *pvParameters) {
    while (1) {
        // Block indefinitely with zero CPU overhead until ISR signals:
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // Process data outside interrupt context:
        Process_Sensor_Packet();
    }
}

// Hardware Interrupt Handler (Fires asynchronously)
void EXTI0_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // Clear hardware flag
    EXTI->PR = (1U << 0);

    // Fast, lightweight unblock signal:
    vTaskNotifyGiveFromISR(xWorkerTaskHandle, &xHigherPriorityTaskWoken);

    // Yield CPU immediately if worker task has higher priority than current task:
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
```
