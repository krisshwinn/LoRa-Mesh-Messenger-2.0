# Troubleshooting

## Common Issues

### No OLED Display
- Verify you selected **"Heltec WiFi LoRa 32(V3)"** in the Arduino IDE.
- Check if the board is receiving power.

### Short Range
- Ensure the antenna is securely connected. **Never power on the LoRa chip without an antenna!**
- Increase the spreading factor (`LORA_SF`) in `config.h`.
- Check for interference or physical obstructions.

### GPS Not Working
- Ensure the GPS module has a clear view of the sky (outdoor visibility is usually required).
- Check the wiring (TX/RX swap is a common mistake).
- **Pin Conflict Note:** On Heltec V3, GPIO 17 is also the SDA pin for the OLED. If you notice display issues when GPS is connected, consider moving the GPS RX/TX to different pins (e.g., GPIO 45/46) and updating `config.h`.
- Check the status LED on the GPS module (blinking usually means a lock is acquired).

### Messages Not Delivered
- Reduce the distance between nodes.
- Verify all nodes are on the same frequency (`LORA_FREQ` in `config.h`).
- Check the Serial Monitor for debug logs.

## Debug Commands
Connect your device to a computer and open the Serial Monitor (115200 baud) to see:
- `RSSI`: Signal strength of the last received message.
- `Battery`: Current battery voltage.
- `GPS`: Current coordinates or "NO_GPS".
