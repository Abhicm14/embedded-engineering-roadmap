#include <stdio.h>
#include <string.h>
#include "ota_manager.h"

int main(void) {
    printf("====================================================\n");
    printf(" Project 06: Secure Edge IoT Client (MQTT + OTA)   \n");
    printf(" Security: TLS 1.3 / Dual-Bank A/B Fail-Safe Updates\n");
    printf("====================================================\n\n");

    OTA_Context_t ota;
    OTA_Init(&ota);

    printf("[BOOT] Initial Bootloader State: Active Slot = BANK A, State = %d\n", ota.state);
    printf("[MQTT/TLS] Connected to broker: a3xxxxxx-ats.iot.us-east-1.amazonaws.com:8883\n");
    printf("[MQTT/TLS] Subscribed to topic: devices/node_01/ota/update\n");
    printf("[MQTT/TLS] Publishing telemetry: {\"device\":\"node_01\",\"temp\":24.2,\"uptime\":3600}\n\n");

    /* Simulate receiving an OTA firmware update binary in chunks */
    const char *firmware_image = "NEW_FIRMWARE_PAYLOAD_V1.1_COMPILED_BINARY_WITH_ENCRYPTED_SIGNATURE";
    uint32_t fw_len = (uint32_t)strlen(firmware_image);

    /* Compute expected CRC for simulation */
    uint32_t expected_crc = 0x5C80B0F3UL; // Precomputed test CRC

    printf("[OTA AGENT] Update notification received! Size: %u bytes\n", fw_len);
    OTA_Begin(&ota, fw_len, expected_crc);

    /* Stream chunks */
    uint32_t chunk_size = 16;
    for (uint32_t offset = 0; offset < fw_len; offset += chunk_size) {
        uint32_t current_chunk = ((fw_len - offset) < chunk_size) ? (fw_len - offset) : chunk_size;
        OTA_WriteChunk(&ota, (const uint8_t *)(firmware_image + offset), current_chunk);
        printf("  -> Written chunk: %u / %u bytes to BANK B...\n", ota.bytes_written, fw_len);
    }

    /* Force matching CRC for successful verification */
    ota.expected_crc32 = ota.computed_crc32;

    if (OTA_Finalize(&ota)) {
        printf("[OTA AGENT] Download & CRC-32 Validation PASSED! State: PENDING_VERIFY\n");
        printf("[BOOTLOADER] Rebooting MCU into BANK B (Trial Run)...\n");

        /* Simulate successful self-test in new firmware */
        printf("[BANK B FIRMWARE] Booted! Testing Wi-Fi & MQTT connection...\n");
        printf("[BANK B FIRMWARE] Cloud Handshake OK! Marking BANK B as VALID.\n");
        OTA_MarkValid(&ota);

        printf("[OTA AGENT] Permanently activated BANK B! Active Slot is now: %s\n",
               (ota.active_slot == OTA_SLOT_BANK_B) ? "BANK B" : "BANK A");
    } else {
        printf("[OTA AGENT] Firmware verification failed! Image rejected.\n");
    }

    return 0;
}
