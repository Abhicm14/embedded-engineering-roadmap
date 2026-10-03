#ifndef OBD2_PARSER_H
#define OBD2_PARSER_H

#include <stdint.h>
#include <stdbool.h>
#include "can_driver.h"

/* Standard OBD-II Request / Response CAN IDs */
#define OBD2_CAN_ID_REQUEST_BROADCAST (0x7DF)
#define OBD2_CAN_ID_REQUEST_ECU       (0x7E0)
#define OBD2_CAN_ID_RESPONSE_ECU      (0x7E8)

/* OBD-II Diagnostic Modes (Services) */
#define OBD2_MODE_CURRENT_DATA        (0x01)
#define OBD2_MODE_CURRENT_DATA_RESP   (0x41)

/* Common Service 01 PIDs */
#define OBD2_PID_COOLANT_TEMP         (0x05)
#define OBD2_PID_ENGINE_RPM           (0x0C)
#define OBD2_PID_VEHICLE_SPEED        (0x0D)
#define OBD2_PID_THROTTLE_POS         (0x11)

typedef struct {
    int16_t  coolant_temp_c;
    uint16_t engine_rpm;
    uint8_t  vehicle_speed_kmh;
    uint8_t  throttle_percent;
} OBD2_VehicleData_t;

/* Process incoming CAN frame and craft response frame if it is an OBD-II query */
bool OBD2_ProcessFrame(const CAN_Frame_t *req, CAN_Frame_t *resp, const OBD2_VehicleData_t *live_data);

#endif /* OBD2_PARSER_H */
