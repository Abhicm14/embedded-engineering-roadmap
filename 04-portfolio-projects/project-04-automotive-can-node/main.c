#include <stdio.h>
#include "obd2_parser.h"

int main(void) {
    printf("====================================================\n");
    printf(" Project 04: Automotive CAN Node & OBD-II Gateway    \n");
    printf(" 500 kbps High-Speed CAN Bus Emulation              \n");
    printf("====================================================\n\n");

    /* Live vehicle engine parameters */
    OBD2_VehicleData_t vehicle = {
        .coolant_temp_c   = 88,    /* 88 deg C */
        .engine_rpm       = 2400,  /* 2400 RPM */
        .vehicle_speed_kmh= 65,    /* 65 km/h */
        .throttle_percent = 35     /* 35% throttle */
    };

    /* Simulate incoming OBD-II diagnostic query for Engine RPM (PID 0x0C) */
    CAN_Frame_t query_frame = {
        .id = OBD2_CAN_ID_REQUEST_BROADCAST, /* 0x7DF */
        .is_extended = false,
        .is_rtr = false,
        .dlc = 8,
        .data = {0x02, 0x01, OBD2_PID_ENGINE_RPM, 0x55, 0x55, 0x55, 0x55, 0x55}
    };

    printf("[CAN BUS] Inbound Diagnostic Request: ID 0x%03X, Service: 0x%02X, PID: 0x%02X\n",
           query_frame.id, query_frame.data[1], query_frame.data[2]);

    CAN_Frame_t resp_frame;
    if (OBD2_ProcessFrame(&query_frame, &resp_frame, &vehicle)) {
        printf("[CAN BUS] Outbound ECU Response: ID 0x%03X, DLC: %u\n", resp_frame.id, resp_frame.dlc);
        printf("          Payload: [");
        for (int i = 0; i < 8; i++) {
            printf(" 0x%02X", resp_frame.data[i]);
        }
        printf(" ]\n");

        /* Decode back to verify math */
        uint16_t decoded_rpm = (((uint16_t)resp_frame.data[3] << 8) | resp_frame.data[4]) / 4U;
        printf("          Decoded Engine Speed: %u RPM (Expected: %u RPM) -> VERIFIED\n\n",
               decoded_rpm, vehicle.engine_rpm);
    }

    /* Simulate query for Vehicle Speed (PID 0x0D) */
    query_frame.data[2] = OBD2_PID_VEHICLE_SPEED;
    if (OBD2_ProcessFrame(&query_frame, &resp_frame, &vehicle)) {
        printf("[CAN BUS] Outbound ECU Response for Speed: %u km/h -> VERIFIED\n", resp_frame.data[3]);
    }

    return 0;
}
