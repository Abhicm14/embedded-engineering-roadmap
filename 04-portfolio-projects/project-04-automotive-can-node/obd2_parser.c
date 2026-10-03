#include "obd2_parser.h"
#include <string.h>

bool OBD2_ProcessFrame(const CAN_Frame_t *req, CAN_Frame_t *resp, const OBD2_VehicleData_t *live_data) {
    if (!req || !resp || !live_data) return false;

    /* Verify if ID is an OBD-II diagnostic request */
    if (req->id != OBD2_CAN_ID_REQUEST_BROADCAST && req->id != OBD2_CAN_ID_REQUEST_ECU) {
        return false;
    }

    uint8_t req_len = req->data[0];
    uint8_t service = req->data[1];
    uint8_t pid     = req->data[2];

    if (req_len < 2 || service != OBD2_MODE_CURRENT_DATA) {
        return false; /* Unsupported service */
    }

    /* Initialize response frame */
    memset(resp, 0xAA, sizeof(CAN_Frame_t)); /* 0xAA padding per ISO 15765-2 */
    resp->id = OBD2_CAN_ID_RESPONSE_ECU;
    resp->is_extended = false;
    resp->is_rtr = false;
    resp->dlc = 8;
    resp->data[1] = OBD2_MODE_CURRENT_DATA_RESP; /* 0x41 */
    resp->data[2] = pid;

    switch (pid) {
        case OBD2_PID_COOLANT_TEMP:
            resp->data[0] = 3; /* Response length */
            /* Formula: Temp = A - 40 -> A = Temp + 40 */
            resp->data[3] = (uint8_t)(live_data->coolant_temp_c + 40);
            return true;

        case OBD2_PID_ENGINE_RPM:
            resp->data[0] = 4; /* Response length */
            /* Formula: RPM = ((A * 256) + B) / 4 */
            {
                uint16_t rpm_scaled = live_data->engine_rpm * 4U;
                resp->data[3] = (uint8_t)(rpm_scaled >> 8);
                resp->data[4] = (uint8_t)(rpm_scaled & 0xFF);
            }
            return true;

        case OBD2_PID_VEHICLE_SPEED:
            resp->data[0] = 3;
            /* Formula: Speed = A km/h */
            resp->data[3] = live_data->vehicle_speed_kmh;
            return true;

        case OBD2_PID_THROTTLE_POS:
            resp->data[0] = 3;
            /* Formula: % = (A * 100) / 255 -> A = (% * 255) / 100 */
            resp->data[3] = (uint8_t)(((uint16_t)live_data->throttle_percent * 255U) / 100U);
            return true;

        default:
            return false; /* Unsupported PID */
    }
}
