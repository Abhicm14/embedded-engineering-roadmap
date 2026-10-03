#ifndef OTA_MANAGER_H
#define OTA_MANAGER_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    OTA_SLOT_BANK_A = 0,
    OTA_SLOT_BANK_B = 1
} OTA_Slot_t;

typedef enum {
    OTA_STATE_IDLE,
    OTA_STATE_DOWNLOADING,
    OTA_STATE_PENDING_VERIFY,
    OTA_STATE_VALIDATED,
    OTA_STATE_ROLLBACK_TRIGGERED
} OTA_State_t;

typedef struct {
    OTA_Slot_t   active_slot;
    OTA_Slot_t   update_slot;
    OTA_State_t  state;
    uint32_t     bytes_written;
    uint32_t     expected_size;
    uint32_t     expected_crc32;
    uint32_t     computed_crc32;
} OTA_Context_t;

void OTA_Init(OTA_Context_t *ctx);
bool OTA_Begin(OTA_Context_t *ctx, uint32_t firmware_size, uint32_t expected_crc32);
bool OTA_WriteChunk(OTA_Context_t *ctx, const uint8_t *data, uint32_t chunk_size);
bool OTA_Finalize(OTA_Context_t *ctx);
bool OTA_MarkValid(OTA_Context_t *ctx);
void OTA_TriggerRollback(OTA_Context_t *ctx);

#endif /* OTA_MANAGER_H */
