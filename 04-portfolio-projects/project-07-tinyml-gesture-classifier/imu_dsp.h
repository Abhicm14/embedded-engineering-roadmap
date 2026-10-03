#ifndef IMU_DSP_H
#define IMU_DSP_H

#include <stdint.h>
#include <stdbool.h>

#define WINDOW_SIZE 50 /* 50 samples = 1.0 second at 50 Hz */

typedef struct {
    float ax, ay, az;
    float gx, gy, gz;
} IMU_Sample_t;

typedef struct {
    float mean_accel;
    float variance_accel;
    float peak_accel;
    float mean_gyro;
    float variance_gyro;
    float peak_gyro;
} IMU_Features_t;

void IMU_DSP_Init(void);
void IMU_DSP_AddSample(const IMU_Sample_t *sample);
bool IMU_DSP_IsWindowReady(void);
void IMU_DSP_ExtractFeatures(IMU_Features_t *features);

#endif /* IMU_DSP_H */
