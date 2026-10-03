#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/* FreeRTOS includes (or simulated thread wrappers for desktop verification) */
typedef struct {
    uint32_t timestamp_ms;
    float    temperature_c;
    float    pressure_hpa;
    uint16_t record_id;
} SensorRecord_t;

/* Simulated RTOS Queue / Synchronization Primitives */
#define QUEUE_MAX_ITEMS 10
static SensorRecord_t s_queue_storage[QUEUE_MAX_ITEMS];
static uint32_t s_q_head = 0;
static uint32_t s_q_tail = 0;
static uint32_t s_q_count = 0;

static bool SimulatedQueue_Send(const SensorRecord_t *item) {
    if (s_q_count >= QUEUE_MAX_ITEMS) return false;
    s_queue_storage[s_q_head] = *item;
    s_q_head = (s_q_head + 1) % QUEUE_MAX_ITEMS;
    s_q_count++;
    return true;
}

static bool SimulatedQueue_Receive(SensorRecord_t *item) {
    if (s_q_count == 0) return false;
    *item = s_queue_storage[s_q_tail];
    s_q_tail = (s_q_tail + 1) % QUEUE_MAX_ITEMS;
    s_q_count--;
    return true;
}

/* Producer Task: Simulates periodic environmental sampling */
static void Task_SensorSampler(void) {
    static uint16_t s_seq = 0;
    SensorRecord_t record = {
        .timestamp_ms  = (uint32_t)(s_seq * 100),
        .temperature_c = 22.5f + (float)(s_seq % 5) * 0.4f,
        .pressure_hpa  = 1013.25f + (float)(s_seq % 3) * 0.1f,
        .record_id     = ++s_seq
    };

    if (SimulatedQueue_Send(&record)) {
        printf("[Task: SensorSampler] Sample #%u Enqueued (Temp: %.2f C, Press: %.2f hPa)\n",
               record.record_id, record.temperature_c, record.pressure_hpa);
    } else {
        printf("[Task: SensorSampler] WARNING: Queue Full! Dropped record #%u\n", record.record_id);
    }
}

/* Consumer Task: Logs incoming records from queue to persistent storage */
static void Task_PersistentLogger(void) {
    SensorRecord_t record;
    while (SimulatedQueue_Receive(&record)) {
        /* In real hardware, write record to W25Qxx SPI Flash page or SD card */
        printf("  [Task: Logger] Committing to SPI Flash -> RecID:%04u @ %u ms | T:%.2fC P:%.2fhPa [CRC: OK]\n",
               record.record_id, record.timestamp_ms, record.temperature_c, record.pressure_hpa);
    }
}

int main(void) {
    printf("====================================================\n");
    printf(" Project 03: FreeRTOS Multitasking Sensor Logger   \n");
    printf(" Architecture: Producer Task -> FreeRTOS Queue -> Logger\n");
    printf("====================================================\n\n");

    /* Simulate 5 execution cycles */
    for (int cycle = 0; cycle < 5; cycle++) {
        printf("--- Tick Time Window %d ---\n", cycle + 1);
        Task_SensorSampler();
        Task_PersistentLogger();
        printf("\n");
    }

    printf("Execution complete: All records flushed to persistent storage successfully.\n");
    return 0;
}
