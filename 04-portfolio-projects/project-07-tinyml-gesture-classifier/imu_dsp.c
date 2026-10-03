#include "imu_dsp.h"
#include <math.h>

static IMU_Sample_t s_window[WINDOW_SIZE];
static uint32_t s_sample_count = 0;
static uint32_t s_write_idx = 0;

void IMU_DSP_Init(void) {
    s_sample_count = 0;
    s_write_idx    = 0;
}

void IMU_DSP_AddSample(const IMU_Sample_t *sample) {
    if (!sample) return;
    s_window[s_write_idx] = *sample;
    s_write_idx = (s_write_idx + 1) % WINDOW_SIZE;
    if (s_sample_count < WINDOW_SIZE) {
        s_sample_count++;
    }
}

bool IMU_DSP_IsWindowReady(void) {
    return (s_sample_count >= WINDOW_SIZE);
}

void IMU_DSP_ExtractFeatures(IMU_Features_t *features) {
    if (!features || s_sample_count == 0) return;

    float sum_accel = 0.0f;
    float sum_gyro  = 0.0f;
    float peak_a    = 0.0f;
    float peak_g    = 0.0f;

    for (uint32_t i = 0; i < s_sample_count; i++) {
        float mag_a = sqrtf(s_window[i].ax * s_window[i].ax +
                            s_window[i].ay * s_window[i].ay +
                            s_window[i].az * s_window[i].az);
        float mag_g = sqrtf(s_window[i].gx * s_window[i].gx +
                            s_window[i].gy * s_window[i].gy +
                            s_window[i].gz * s_window[i].gz);
        sum_accel += mag_a;
        sum_gyro  += mag_g;

        if (mag_a > peak_a) peak_a = mag_a;
        if (mag_g > peak_g) peak_g = mag_g;
    }

    float mean_a = sum_accel / (float)s_sample_count;
    float mean_g = sum_gyro  / (float)s_sample_count;

    float var_a = 0.0f;
    float var_g = 0.0f;

    for (uint32_t i = 0; i < s_sample_count; i++) {
        float mag_a = sqrtf(s_window[i].ax * s_window[i].ax +
                            s_window[i].ay * s_window[i].ay +
                            s_window[i].az * s_window[i].az);
        float mag_g = sqrtf(s_window[i].gx * s_window[i].gx +
                            s_window[i].gy * s_window[i].gy +
                            s_window[i].gz * s_window[i].gz);
        var_a += (mag_a - mean_a) * (mag_a - mean_a);
        var_g += (mag_g - mean_g) * (mag_g - mean_g);
    }

    features->mean_accel     = mean_a;
    features->variance_accel = var_a / (float)s_sample_count;
    features->peak_accel     = peak_a;
    features->mean_gyro      = mean_g;
    features->variance_gyro  = var_g / (float)s_sample_count;
    features->peak_gyro      = peak_g;
}
