#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define QUEUE_CAPACITY 8

typedef struct {
    uint32_t sensor_timestamp;
    int16_t  raw_adc_value;
} TelemetryPacket_t;

typedef struct {
    TelemetryPacket_t items[QUEUE_CAPACITY];
    uint32_t head;
    uint32_t tail;
    uint32_t count;
} SafeQueue_t;

static void Queue_Init(SafeQueue_t *q) {
    q->head = 0;
    q->tail = 0;
    q->count = 0;
}

static bool Queue_Send(SafeQueue_t *q, const TelemetryPacket_t *pkt) {
    if (q->count >= QUEUE_CAPACITY) return false;
    q->items[q->head] = *pkt;
    q->head = (q->head + 1) % QUEUE_CAPACITY;
    q->count++;
    return true;
}

static bool Queue_Receive(SafeQueue_t *q, TelemetryPacket_t *pkt) {
    if (q->count == 0) return false;
    *pkt = q->items[q->tail];
    q->tail = (q->tail + 1) % QUEUE_CAPACITY;
    q->count--;
    return true;
}

int main(void) {
    printf("=== FreeRTOS Producer-Consumer Pattern Demonstration ===\n");

    SafeQueue_t tele_queue;
    Queue_Init(&tele_queue);

    /* Producer produces 3 items */
    for (uint32_t i = 0; i < 3; i++) {
        TelemetryPacket_t p = { .sensor_timestamp = i * 50, .raw_adc_value = (int16_t)(1024 + i * 10) };
        Queue_Send(&tele_queue, &p);
        printf("[Producer] Sent: Time=%u ms, ADC=%d\n", p.sensor_timestamp, p.raw_adc_value);
    }

    /* Consumer consumes all items */
    TelemetryPacket_t rx;
    while (Queue_Receive(&tele_queue, &rx)) {
        printf("  [Consumer] Received: Time=%u ms, ADC=%d (Processed)\n", rx.sensor_timestamp, rx.raw_adc_value);
    }

    printf("Producer-Consumer queue cycle completed successfully.\n");
    return 0;
}
