# User Manual

## How to Use

### Basic Operation
1. **Power On:** Connect a battery or USB-C power source.
2. **Device Identity:** The OLED will show the current device name (e.g., `NODE_A`).
3. **Sending Messages:**
   - **Long Press (2s):** Sends a "Quick Message" to all devices.
4. **Viewing Messages:**
   - **Short Press:** View the last received message.
5. **Status Check:**
   - **Double Press:** Shows battery percentage and GPS status.

### Testing Communication
1. Power all three devices.
2. Press the USER button on Device A.
3. The message should appear on Devices B and C.
4. Move devices apart to test the mesh range (Device B can act as a relay for Device C if A and C are out of range).

## Range Optimization

To maximize the distance between nodes:
1. **Antennas:** Use external high-gain antennas.
2. **Elevation:** Higher is better! Place devices on hills or upper floors.
3. **Line of Sight:** Avoid obstacles like thick walls or metal structures.
4. **LoRa Settings:** In `config.h`, you can increase the Spreading Factor (SF) to 12 for maximum range (though this reduces speed).

| Environment | Expected Range |
|-------------|----------------|
| Urban       | 1-2 km         |
| Suburban    | 3-5 km         |
| Rural (LoS) | 10+ km         |
