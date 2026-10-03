#ifndef CAN_DRIVER_H
#define CAN_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

#define CAN_ID_STD_MASK        (0x000007FFUL)
#define CAN_ID_EXT_MASK        (0x1FFFFFFFUL)

typedef struct {
    uint32_t id;         /* 11-bit standard or 29-bit extended ID */
    bool     is_extended;/* True if 29-bit CAN 2.0B frame */
    bool     is_rtr;     /* Remote Transmission Request flag */
    uint8_t  dlc;        /* Data Length Code (0 to 8 bytes) */
    uint8_t  data[8];    /* Payload data bytes */
} CAN_Frame_t;

/* Initialize CAN hardware controller (Bit timing: 500 kbps) */
bool CAN_Init(uint32_t baud_rate);

/* Configure acceptance filter bank */
void CAN_ConfigureFilter(uint8_t filter_bank, uint32_t filter_id, uint32_t filter_mask);

/* Transmit a CAN frame */
bool CAN_Transmit(const CAN_Frame_t *frame);

/* Receive a CAN frame from FIFO */
bool CAN_Receive(CAN_Frame_t *frame);

#endif /* CAN_DRIVER_H */
