# 🌐 IoT, Wireless Protocols & Embedded Security Track

> Wi-Fi, BLE, MQTT over TLS 1.3, hardware Root of Trust, Secure Boot, and dual-bank fail-safe OTA updates.

---

## 1. IoT Wireless Protocol Landscape

| Protocol | Physical Layer | Range | Bandwidth | Typical Power | Best Use Case |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **Wi-Fi** | 802.11 b/g/n (2.4/5GHz)| $\approx 50\text{m}$ | High (Mbps) | High ($> 80\text{mA}$) | Smart home appliances, mains-powered gateways. |
| **BLE 5** | 2.4 GHz GFSK | $\approx 30\text{m}$ | Med (1-2 Mbps)| Ultra-low ($< 10\text{mA}$) | Wearables, health sensors, smartphone provisioning. |
| **LoRaWAN**| Sub-GHz (868/915MHz) | $5 - 15\text{km}$ | Ultra-low (kbps)| Coin-cell ($< 25\text{mA}$ pulse)| Smart agriculture, utility meters, smart cities. |

---

## 2. Embedded Cryptography & Hardware Security

1. **Hardware Root of Trust (RoT):** An immutable, un-cloneable public key or cryptographic secret burned into silicon One-Time Programmable (OTP) eFuses during manufacturing.
2. **Secure Boot:**
   - The Boot ROM verifies the RSA/ECDSA digital signature of the secondary bootloader before jumping to it.
   - The secondary bootloader verifies the application image signature in Flash before booting.
   - If a corrupted or unauthorized third-party firmware binary is flashed, the chip refuses to boot!
3. **Transport Layer Security (TLS 1.3):** Mutual authentication using X.509 certificates and ephemeral Diffie-Hellman key exchange (`mbedTLS`).

---

## 3. Fail-Safe Dual-Bank A/B OTA Architecture

Never overwrite the currently executing firmware image while downloading an update!

```
Flash Partition Map:
[ Bank A (Slot 0): Active App v1.0 ] ──► Currently running CPU code
[ Bank B (Slot 1): Target Slot     ] ◄── Inbound encrypted packets streamed here
```

### Self-Test & Rollback Rule:
1. Boot into Bank B after full CRC-32 and ECDSA signature verification.
2. Start a hardware watchdog timer (e.g. 60 seconds).
3. Connect to Wi-Fi and the cloud MQTT broker.
4. If the cloud handshake succeeds, execute `OTA_MarkValid()` to permanently commit Bank B.
5. If the system crashes, hangs, or fails to connect, the watchdog expires, and the bootloader automatically reverts to Bank A on the next reset!
