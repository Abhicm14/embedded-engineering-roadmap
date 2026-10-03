# 📬 FreeRTOS Queues & Inter-Task Data Pipelines

> Thread-safe FIFO buffering, passing by copy versus passing by reference, and timeout management.

---

## 1. Queue Architecture

FreeRTOS Queues are the primary Inter-Process Communication (IPC) mechanism. They provide:
1. **Thread-Safe FIFO:** Multiple tasks can write to and read from the same queue without race conditions.
2. **Deterministic Blocking:** Tasks attempting to read from an empty queue enter the **Blocked** state with a configurable timeout (`portMAX_DELAY` or specific milliseconds).
3. **Pass-by-Copy Standard:** By default, FreeRTOS copies data bytes into internal queue storage, protecting the sender from accidentally overwriting data before the receiver processes it.

```c
typedef struct {
    uint32_t timestamp;
    float    pressure_hpa;
    float    temperature_c;
} SensorPayload_t;

QueueHandle_t xSensorQueue = NULL;

// Allocate queue capable of holding 10 structs
xSensorQueue = xQueueCreate(10, sizeof(SensorPayload_t));
```

---

## 2. Pass-by-Copy vs Pass-by-Reference

- **Pass-by-Copy (Small payloads $\le 32$ bytes):** Passing primitive values or small structs. Safe, eliminates dangling pointer bugs.
- **Pass-by-Reference (Large payloads $> 64$ bytes / Audio / Images):** Passing a pointer (`uint8_t *`) through the queue.
  - ⚠️ **Ownership Contract:** Once a task sends a pointer into a queue, it must surrender ownership and never modify the buffer until the receiving task frees or returns it!
