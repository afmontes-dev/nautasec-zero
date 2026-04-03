# NautaSec Zero

> Portable multi-protocol RF/NFC/RFID auditing and automation device for OT/ICS environments.

Built on ESP32 — designed and developed by [@afmontes-dev](https://github.com/afmontes-dev) under [Nauta Systems](https://www.linkedin.com/company/nauta-systems-group).

---

## What is this?

NautaSec Zero is a handheld multi-protocol device capable of reading, capturing, and emitting signals across multiple wireless protocols. It is designed for two primary use cases:

- **Security auditing** of physical access systems in industrial and OT/ICS environments
- **Physical automation** — acting as a hardware node integrated with automation platforms like n8n, Home Assistant, or custom REST APIs

---

## Hardware

| Module | Protocol | Function |
|---|---|---|
| ESP32-S CH340 38Pin | WiFi / Bluetooth | Main MCU, wireless connectivity |
| CC1101 + SMA antenna | Sub-GHz 433MHz | Signal capture and replay |
| PN532 V3 | NFC 13.56MHz | MIFARE card read/write |
| RDM6300 | RFID 125kHz | EM4100 card reading |
| SSD1306 128x64 | I2C | OLED display |
| IR LED 940nm + TSOP | Infrared | Universal IR control |
| TXS0108E | Logic level | 3.3V/5V bidirectional shifter |
| TP4056 Type-C | — | LiPo battery charging |
| LiPo 3.7V 1000mAh | — | Power supply |

**Estimated BOM cost:** ~$185.000 COP (~$45 USD)

---

## Features

- Sub-GHz signal capture and replay (433MHz)
- NFC card reading and emulation (MIFARE Classic)
- RFID 125kHz credential reading (EM4100)
- Universal IR remote control
- WiFi-based web interface for remote control
- REST API for integration with n8n and automation platforms
- OLED navigation menu
- Portable — LiPo powered

---

## Use Cases

### OT/ICS Security Auditing
- Physical access credential assessment (RFID/NFC)
- Sub-GHz signal analysis on industrial remotes and control systems
- On-site security posture evaluation without laptop dependency

### Industrial Automation
- Hardware trigger node for n8n workflows
- RFID-based asset identification and tracking
- IR control of legacy industrial equipment
- Physical-to-digita

---

## Status

🔧 **In development** — hardware ordered, firmware in progress.

| Module | Status |
|---|---|
| OLED menu | 🔄 In progress |
| IR capture/emit | ⏳ Pending |
| RFID 125kHz | ⏳ Pending |
| NFC 13.56MHz | ⏳ Pending |
| Sub-GHz CC1101 | ⏳ Pending |
| WiFi web interface | ⏳ Pending |
| n8n integration | ⏳ Pending |
| Case design | ⏳ Pending |

---

## Legal Notice

This device is intended strictly for authorized security auditing, educational purposes, and personal automation projects. Any use against systems without explicit written authorization is illegal. The author assumes no liability for misuse.

---

## Author

**Andrés F. Montes**
Mechanical Engineer — Process Automation & Industrial IoT
[GitHub](https://github.com/afmontes-dev) · [LinkedIn](https://linkedin.com/in/tu-perfil)
