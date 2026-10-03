# 📁 Project 07: TinyML Real-Time Motion & Gesture Classifier

> Target: STM32F401 / Cortex-M4F with FPU / ESP32  
> Language: Embedded C / C++ (C99/C++17)  
> Key Technologies: TensorFlow Lite for Microcontrollers (TFLM), CMSIS-NN Quantized INT8 Neural Network, 6-Axis IMU (MPU6050) DSP Pipeline

---

## 🎯 Project Overview

This capstone project deploys an on-device machine learning neural network onto an ARM Cortex-M4 microcontroller to classify physical human gestures in real-time using a 6-axis Inertial Measurement Unit (IMU).

### Architectural Highlights:
1. **Zero-Cloud Inferencing:** The entire model executes locally in $< 5\text{ ms}$ with sub-milliwatt power and zero latency.
2. **DSP Preprocessing:** A sliding time-window buffer captures accelerometer and gyroscope streams at 50 Hz, computing rolling mean, standard deviation, and energy metrics.
3. **8-bit Quantization (INT8):** Float32 model weights are quantized to INT8 using TensorFlow Lite Micro and CMSIS-NN SIMD instructions, shrinking RAM consumption by 75% without losing accuracy.
4. **Gesture Classes:**
   - Class 0: `STATIONARY` (Resting on table)
   - Class 1: `WAVE` (Horizontal side-to-side oscillation)
   - Class 2: `PUNCH` (Sharp forward linear acceleration burst)
   - Class 3: `CIRCLE` (Continuous 2D angular rotation)

---

## 🧠 TinyML Processing Pipeline

```
 [ 6-Axis IMU (50 Hz) ] ──(Ax, Ay, Az, Gx, Gy, Gz)
           │
           ▼
 ┌──────────────────────────────────────┐
 │  Sliding Window DSP Buffer (1.0 sec) │
 │  - High-Pass Gravity Filter          │
 │  - Feature Extraction (RMS, Variance)│
 └─────────────────┬────────────────────┘
                   │
                   ▼ (12 Normalized Features)
 ┌──────────────────────────────────────┐
 │   CMSIS-NN INT8 Fully-Connected NN   │
 │   Input (12) -> Dense(32) -> Dense(4)│
 └─────────────────┬────────────────────┘
                   │
                   ▼ (Softmax Output)
 ┌──────────────────────────────────────┐
 │      Gesture Confidence Scores:      │
 │  - STATIONARY :  2%                  │
 │  - WAVE       : 94% ──► TRIGGER ACTION
 │  - PUNCH      :  3%                  │
 │  - CIRCLE     :  1%                  │
 └──────────────────────────────────────┘
```
