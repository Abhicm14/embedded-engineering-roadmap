# ⏱️ FreeRTOS API Quick Reference Cheatsheet

> High-frequency FreeRTOS API calls, task management, queues, semaphores, mutexes, event groups, and ISR-safe interaction rules.

---

## 1. Task Management

```c
#include "FreeRTOS.h"
#include "task.h"

// 1. Task Prototype & Handle
TaskHandle_t xMyTaskHandle = NULL;
void MyTaskFunction(void *pvParameters);

// 2. Task Creation (Dynamic Allocation)
BaseType_t status = xTaskCreate(
    MyTaskFunction,         // Function pointer to task entry
    "SensTask",             // Descriptive string name
    configMINIMAL_STACK_SIZE + 128, // Stack size in 32-bit words (NOT bytes!)
    NULL,                   // Parameter passed to task
    tskIDLE_PRIORITY + 2,   // Task priority (higher number = higher priority)
    &xMyTaskHandle          // Output task handle
);

// 3. Delays
vTaskDelay(pdMS_TO_TICKS(100)); // Non-blocking relative delay (task enters Blocked state)

// 4. Periodic Fixed-Rate Execution (Prevents cumulative drift)
TickType_t xLastWakeTime = xTaskGetTickCount();
vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(50)); // Strict 50ms periodic cycle

// 5. Suspend, Resume & Delete
vTaskSuspend(xMyTaskHandle);
vTaskResume(xMyTaskHandle);
vTaskDelete(NULL); // Deletes current task and yields CPU
```

---

## 2. Queues (Thread-Safe Inter-Task FIFO)

```c
#include "queue.h"

QueueHandle_t xSensorQueue = NULL;

typedef struct {
    uint8_t  sensor_id;
    float    temperature;
    uint32_t timestamp;
} SensorData_t;

// Create Queue: 10 items of type SensorData_t
xSensorQueue = xQueueCreate(10, sizeof(SensorData_t));

// Send to Queue (Pass by copy)
SensorData_t packet = { .sensor_id = 1, .temperature = 24.5f, .timestamp = 1000 };
if (xQueueSend(xSensorQueue, (void *)&packet, pdMS_TO_TICKS(10)) != pdPASS) {
    // Queue was full for 10ms
}

// Receive from Queue (Blocks if empty)
SensorData_t rx_packet;
if (xQueueReceive(xSensorQueue, &(rx_packet), portMAX_DELAY) == pdPASS) {
    // Data received successfully
}
```

---

## 3. Semaphores & Mutexes (Synchronization & Resource Locking)

```c
#include "semphr.h"

SemaphoreHandle_t xI2CMutex = NULL;
SemaphoreHandle_t xDataReadySem = NULL;

// Create Mutex (Includes Priority Inheritance Protocol!)
xI2CMutex = xSemaphoreCreateMutex();

// Protecting Shared Hardware Resource
if (xSemaphoreTake(xI2CMutex, pdMS_TO_TICKS(20)) == pdTRUE) {
    // Critical section: access shared I2C bus safely
    I2C_WriteBytes(...);
    xSemaphoreGive(xI2CMutex); // Must always give back!
}

// Binary Semaphore (Signaling an event from ISR to Task)
xDataReadySem = xSemaphoreCreateBinary();
```

---

## 4. Interrupt-Safe APIs (`*FromISR`) Pattern

Standard FreeRTOS API functions must **NEVER** be called inside an Interrupt Service Routine. Always use the `*FromISR` variants:

```c
void EXTI0_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // Clear hardware interrupt flag first
    EXTI->PR = (1U << 0);

    // Give semaphore to unblock high-priority worker task
    xSemaphoreGiveFromISR(xDataReadySem, &xHigherPriorityTaskWoken);

    // If giving the semaphore unblocked a higher priority task, request context switch
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
```

---

## 5. Direct-to-Task Notifications (Fast, Zero-RAM Signaling)

Task notifications are 45% faster than binary semaphores and consume 0 extra bytes of RAM:

```c
// Receiver Task: Wait for notification (Acts like a binary semaphore)
uint32_t ulNotificationValue;
if (xTaskNotifyWait(0x00, ULONG_MAX, &ulNotificationValue, portMAX_DELAY) == pdTRUE) {
    // Event occurred!
}

// Sender Task or ISR:
// In Task:
xTaskNotifyGive(xWorkerTaskHandle);
// In ISR:
vTaskNotifyGiveFromISR(xWorkerTaskHandle, &xHigherPriorityTaskWoken);
portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
```
