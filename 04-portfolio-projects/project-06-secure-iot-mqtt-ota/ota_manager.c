#include "ota_manager.h"
#include <string.h>

/* Fast software CRC-32 implementation for verification */
static uint32_t Calculate_CRC32(uint32_t crc, const uint8_t *buf, size_t len) {
    crc = ~crc;
    while (len--) {
        crc ^= *buf++;
        for (int k = 0; k < 8; k++) {
            crc = (crc >> 1) ^ (0xEDB88320UL & -(crc & 1));
        }
    }
    return ~crc;
}

void OTA_Init(OTA_Context_t *ctx) {
    if (!ctx) return;
    memset(ctx, 0, sizeof(OTA_Context_t));
    ctx->active_slot = OTA_SLOT_BANK_A;
    ctx->update_slot = OTA_SLOT_BANK_B;
    ctx->state       = OTA_STATE_IDLE;
}

bool OTA_Begin(OTA_Context_t *ctx, uint32_t firmware_size, uint32_t expected_crc32) {
    if (!ctx || firmware_size == 0) return false;
    ctx->expected_size  = firmware_size;
    ctx->expected_crc32 = expected_crc32;
    ctx->bytes_written  = 0;
    ctx->computed_crc32 = 0;
    ctx->state          = OTA_STATE_DOWNLOADING;
    return true;
}

bool OTA_WriteChunk(OTA_Context_t *ctx, const uint8_t *data, uint32_t chunk_size) {
    if (!ctx || !data || ctx->state != OTA_STATE_DOWNLOADING) return false;

    /* In real hardware, write chunk to Flash partition at update_slot offset */
    ctx->computed_crc32 = Calculate_CRC32(ctx->computed_crc32, data, chunk_size);
    ctx->bytes_written += chunk_size;

    return true;
}

bool OTA_Finalize(OTA_Context_t *ctx) {
    if (!ctx || ctx->state != OTA_STATE_DOWNLOADING) return false;

    if (ctx->bytes_written != ctx->expected_size) {
        ctx->state = OTA_STATE_IDLE;
        return false; /* Size mismatch */
    }

    if (ctx->computed_crc32 != ctx->expected_crc32) {
        ctx->state = OTA_STATE_IDLE;
        return false; /* Corrupted image */
    }

    /* Validated image! Switch slot to pending verification */
    ctx->state = OTA_STATE_PENDING_VERIFY;
    return true;
}

bool OTA_MarkValid(OTA_Context_t *ctx) {
    if (!ctx || ctx->state != OTA_STATE_PENDING_VERIFY) return false;
    /* Commit slot permanently */
    ctx->active_slot = ctx->update_slot;
    ctx->update_slot = (ctx->active_slot == OTA_SLOT_BANK_A) ? OTA_SLOT_BANK_B : OTA_SLOT_BANK_A;
    ctx->state       = OTA_STATE_VALIDATED;
    return true;
}

void OTA_TriggerRollback(OTA_Context_t *ctx) {
    if (!ctx) return;
    /* Invert back to safe known bank */
    ctx->state = OTA_STATE_ROLLBACK_TRIGGERED;
}
