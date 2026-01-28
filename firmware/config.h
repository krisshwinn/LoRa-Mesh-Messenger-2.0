#ifndef CONFIG_H
#define CONFIG_H

// --- Device Settings ---
#define DEVICE_ID "NODE_A"   // Change for each device (e.g., NODE_A, NODE_B, NODE_C)
#define MAX_HOPS 3           // Max relay hops
#define MSG_TIMEOUT 30000    // Message expiry (ms)

// --- LoRa Settings (915MHz USA) ---
#define LORA_FREQ 915.0
#define LORA_SF 9            // Range vs Speed (6-12)
#define LORA_BW 125.0
#define LORA_CR 5            // Error correction (5-8)
#define LORA_PWR 17          // Output power in dBm (up to 22)

// --- Hardware Pins ---
// GPS (NEO-6M)
#define GPS_RX_PIN 17
#define GPS_TX_PIN 16

// OLED (Heltec V3)
// Pins are usually defined by the Heltec library, but for reference:
// SDA 17, SCL 18 (Wait, Heltec V3 has different pins, usually handled by board selection)

#endif
