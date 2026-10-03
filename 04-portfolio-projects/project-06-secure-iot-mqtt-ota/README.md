# 📁 Project 06: Secure Edge IoT Device with MQTT over TLS & OTA Updates

> Target: ESP32 / STM32 + Wi-Fi (W5500 / ESP-AT)  
> Language: Embedded C (C99)  
> Key Technologies: TLS 1.3 Encryption, Mutual X.509 Authentication (mTLS), MQTT 3.1.1, Dual-Bank A/B Flash Partitioning, Automatic Firmware Rollback

---

## 🎯 Project Overview

This project implements an enterprise-grade IoT edge client capable of streaming encrypted telemetry to cloud brokers (AWS IoT Core, Azure IoT Hub, or Mosquitto) over TLS and performing fail-safe Over-The-Air (OTA) firmware upgrades.

### Architectural Highlights:
1. **End-to-End Cryptographic Security:** Hardware-accelerated TLS 1.3 with X.509 client certificate authentication.
2. **Dual-Bank A/B Partitioning:** Flash is split into Bank A (Running firmware) and Bank B (Update slot). The bootloader runs Bank A; new firmware streams into Bank B.
3. **Rollback Protection & Self-Test:** On reboot into new firmware, a 30-second watchdog timer starts. If the new firmware fails to connect to Wi-Fi/MQTT and validate itself via `OTA_MarkValid()`, the bootloader automatically invalidates Bank B and rolls back to Bank A on next reset.

---

## 🛡️ Dual-Bank Flash Memory Map

```
Flash Address
0x00000000 ┌──────────────────────────────────────┐
           │ Secure First-Stage Bootloader        │
0x00010000 ├──────────────────────────────────────┤
           │ Partition Table & NVS Certificates   │
0x00020000 ├──────────────────────────────────────┤
           │ Bank A (Slot 0): Active App (1.5 MB) │ ◄── Currently Running v1.0
0x001A0000 ├──────────────────────────────────────┤
           │ Bank B (Slot 1): OTA Slot   (1.5 MB) │ ◄── Inbound Download v1.1
0x00320000 ├──────────────────────────────────────┤
           │ OTA State & Rollback Flag Metadata   │
0x00400000 └──────────────────────────────────────┘ (4MB SPI Flash End)
```

---

## 🔄 OTA Update State Machine

```
   [ Normal Operation (Bank A) ]
                 │
                 │ Inbound MQTT Update Notification: "firmware_v1.1.bin"
                 ▼
   [ Downloading to Bank B ] ──(Checksum Mismatch)──► [ Abort & Purge ]
                 │
                 │ SHA-256 Validated
                 ▼
   [ Set Boot Slot = Bank B (State: PENDING_VERIFICATION) ]
                 │
                 │ Reboot CPU
                 ▼
   [ Booting Bank B ] ────(Crash / Watchdog Timeout)────► [ Rollback to Bank A ]
                 │
                 │ Wi-Fi + MQTT Handshake OK
                 ▼
   [ OTA_MarkValid() -> Permanently Activate Bank B ]
```
