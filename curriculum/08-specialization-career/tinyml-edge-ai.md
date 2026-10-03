# 🧠 TinyML & Edge Artificial Intelligence Track

> On-device neural network inferencing on ARM Cortex-M microcontrollers using TensorFlow Lite Micro and CMSIS-NN.

---

## 1. Why Run Machine Learning on Edge Microcontrollers?

1. **Zero Cloud Latency:** Predictions occur locally in $< 5\text{ ms}$ without round-trip network lag.
2. **Total Privacy:** Audio, biometric, and camera data never leave the local silicon device.
3. **Sub-Milliwatt Energy:** Microcontrollers running at 64-100 MHz draw $< 15\text{ mA}$, enabling battery lifetimes measured in months or years.

---

## 2. The TinyML Optimization Pipeline

```
Train Model in Python (PyTorch / TensorFlow)
  │ (Float32 weights: ~2.4 MB)
  ▼
Quantization & Pruning (TFLite Converter)
  │ Convert Float32 -> Quantized INT8 weights: ~60 KB!
  ▼
C Array Generation (xxd -i model.tflite > model_data.h)
  │ Placed in Cortex-M Flash (.rodata)
  ▼
CMSIS-NN Execution Kernel
  │ SIMD instructions (SMLAD - Signed Multiply-Accumulate Dual)
  ▼
Real-Time Inference Output (Output logits / Softmax classes)
```

---

## 3. Digital Signal Processing (DSP) Preprocessing

A neural network cannot reliably process raw, noisy time-series ADC or IMU data directly. Always extract normalized spectral or time-domain features:
- **Audio:** Fast Fourier Transform (FFT) $\rightarrow$ Mel-Frequency Cepstral Coefficients (MFCC) spectrograms.
- **Vibration / IMU:** Rolling RMS energy, variance, zero-crossing rate, and peak-to-peak amplitude over fixed-length sliding windows.
