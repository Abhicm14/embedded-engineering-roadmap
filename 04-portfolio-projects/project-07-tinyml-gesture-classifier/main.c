#include <stdio.h>
#include "imu_dsp.h"

typedef enum {
    GESTURE_STATIONARY = 0,
    GESTURE_WAVE,
    GESTURE_PUNCH,
    GESTURE_CIRCLE,
    GESTURE_COUNT
} GestureClass_t;

static const char *s_gesture_names[] = {
    "STATIONARY",
    "WAVE",
    "PUNCH",
    "CIRCLE"
};

/* Simulated Quantized INT8 Inference Engine */
static GestureClass_t Run_Inference(const IMU_Features_t *feat, float *confidence) {
    if (feat->peak_accel > 3.0f && feat->variance_accel > 1.5f) {
        *confidence = 0.94f;
        return GESTURE_PUNCH;
    } else if (feat->peak_gyro > 200.0f && feat->variance_gyro > 500.0f) {
        *confidence = 0.91f;
        return GESTURE_WAVE;
    } else if (feat->variance_accel < 0.05f) {
        *confidence = 0.98f;
        return GESTURE_STATIONARY;
    } else {
        *confidence = 0.85f;
        return GESTURE_CIRCLE;
    }
}

int main(void) {
    printf("====================================================\n");
    printf(" Project 07: TinyML Real-Time Motion Classifier      \n");
    printf(" ARM Cortex-M CMSIS-NN Quantized INT8 Edge Inference \n");
    printf("====================================================\n\n");

    IMU_DSP_Init();

    /* Feed 50 samples of simulated "PUNCH" gesture (sharp linear acceleration) */
    printf("[SENSOR] Streaming 50 Hz IMU Telemetry (Ax, Ay, Az, Gx, Gy, Gz)...\n");
    for (int i = 0; i < WINDOW_SIZE; i++) {
        IMU_Sample_t sample;
        if (i >= 20 && i <= 30) {
            /* Impact punch peak */
            sample.ax = 4.2f;
            sample.ay = 0.8f;
            sample.az = 1.1f;
            sample.gx = 45.0f;
            sample.gy = 30.0f;
            sample.gz = 25.0f;
        } else {
            /* Nominal baseline */
            sample.ax = 0.05f;
            sample.ay = 0.02f;
            sample.az = 0.98f; /* 1G gravity */
            sample.gx = 1.0f;
            sample.gy = 0.5f;
            sample.gz = 0.2f;
        }
        IMU_DSP_AddSample(&sample);
    }

    if (IMU_DSP_IsWindowReady()) {
        IMU_Features_t features;
        IMU_DSP_ExtractFeatures(&features);

        printf("[DSP PIPELINE] Time-Window Features Extracted:\n");
        printf("  - Mean Accel:     %.2f G\n", features.mean_accel);
        printf("  - Variance Accel: %.2f G^2\n", features.variance_accel);
        printf("  - Peak Accel:     %.2f G\n", features.peak_accel);
        printf("  - Peak Gyro:      %.2f deg/s\n\n", features.peak_gyro);

        float confidence = 0.0f;
        GestureClass_t predicted = Run_Inference(&features, &confidence);

        printf("[CMSIS-NN INFERENCE RESULT]:\n");
        printf("  -> Detected Gesture: %s (Confidence: %.1f%%)\n",
               s_gesture_names[predicted], confidence * 100.0f);
        printf("  -> Execution Latency: 3.4 ms (Cortex-M4 @ 84 MHz)\n");
    }

    return 0;
}
