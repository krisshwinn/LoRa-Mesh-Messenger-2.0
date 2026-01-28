#include <RadioLib.h>
#include <ArduinoJson.h>
#include <TinyGPS++.h>
#include "config.h"

// --- Hardware Instances ---
SX1262 radio = new Module(8, 14, 12, 13);
TinyGPSPlus gps;
HardwareSerial gpsSerial(1);

// --- State Variables ---
String last_message = "No messages";
float battery_voltage = 0.0;
String seen_ids[10];
int seen_idx = 0;

// Flag for LoRa interrupt
volatile bool receivedFlag = false;

#if defined(ESP32)
  void IRAM_ATTR setFlag(void) {
    receivedFlag = true;
  }
#else
  void setFlag(void) {
    receivedFlag = true;
  }
#endif

void setup() {
  Serial.begin(115200);
  gpsSerial.begin(9600, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);

  // Heltec V3 Battery/Vext Control
  pinMode(21, OUTPUT);
  digitalWrite(21, LOW); // Enable Vext and Battery reading

  // Initialize LoRa
  Serial.print(F("[LoRa] Initializing ... "));
  int state = radio.begin(LORA_FREQ);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
    radio.setSpreadingFactor(LORA_SF);
    radio.setOutputPower(LORA_PWR);
    radio.setCodingRate(LORA_CR);

    // Set up non-blocking reception
    radio.setPacketReceivedAction(setFlag);
    state = radio.startReceive();
    if (state != RADIOLIB_ERR_NONE) {
      Serial.println(F("startReceive failed"));
    }
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

  // Initialize OLED (Placeholder for actual library initialization)
  updateDisplay("Ready", "Waiting...");
}

void loop() {
  // Check LoRa flag
  if (receivedFlag) {
    receivedFlag = false;
    String received_raw;
    int state = radio.readData(received_raw);

    if (state == RADIOLIB_ERR_NONE) {
      handleMessage(received_raw);
    }

    // Resume listening
    radio.startReceive();
  }

  // Update GPS
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }

  // Handle Button Logic (Simplified timing)
  handleButtons();

  // Periodically update battery
  static uint32_t last_bat_check = 0;
  if (millis() - last_bat_check > 60000) {
    battery_voltage = getBatteryVoltage();
    last_bat_check = millis();
  }
}

void handleMessage(String raw) {
  StaticJsonDocument<512> doc;
  DeserializationError error = deserializeJson(doc, raw);
  if (error) return;

  const char* id = doc["id"];

  // Deduplication logic
  for (int i = 0; i < 10; i++) {
    if (seen_ids[i] == String(id)) return;
  }
  seen_ids[seen_idx] = String(id);
  seen_idx = (seen_idx + 1) % 10;

  const char* from = doc["from"];
  const char* to = doc["to"];
  const char* text = doc["text"];
  int hops = doc["hops"];

  last_message = String(from) + ": " + String(text);
  Serial.println("New Message: " + last_message);
  updateDisplay(String(from), String(text));

  // Mesh Logic
  if (String(to) != DEVICE_ID && hops < MAX_HOPS) {
    doc["hops"] = hops + 1;
    String rebroadcast_raw;
    serializeJson(doc, rebroadcast_raw);

    radio.transmit(rebroadcast_raw);
    Serial.println("Rebroadcasting message...");

    // Restart receive after transmit
    radio.startReceive();
  }
}

void handleButtons() {
  static bool last_btn_state = HIGH;
  static uint32_t press_start = 0;
  bool btn_state = digitalRead(0); // USER button on Heltec V3 is GPIO 0

  if (btn_state == LOW && last_btn_state == HIGH) {
    press_start = millis();
  } else if (btn_state == HIGH && last_btn_state == LOW) {
    uint32_t duration = millis() - press_start;
    if (duration > 2000) {
      sendQuickMessage();
    } else {
      // Short press: show last message
      updateDisplay("Last Message", last_message);
    }
  }
  last_btn_state = btn_state;
}

void updateDisplay(String header, String body) {
  // Placeholder for OLED library calls
  Serial.println("OLED [" + header + "]: " + body);
}

String getLocation() {
  if (gps.location.isValid()) {
    return String(gps.location.lat(), 6) + "," +
           String(gps.location.lng(), 6);
  }
  return "NO_GPS";
}

float getBatteryVoltage() {
  // Heltec V3 Battery reading (Requires Vext GPIO 21 to be LOW)
  // ADC1_CH0 is GPIO 1
  uint16_t raw = analogRead(1);
  float voltage = (raw / 4095.0) * 2.0 * 3.3 * 1.1; // Voltage divider and reference calibration
  return voltage;
}

void sendQuickMessage() {
  StaticJsonDocument<512> doc;
  String mid = String(DEVICE_ID) + String(millis());
  doc["id"] = mid;
  doc["from"] = DEVICE_ID;
  doc["to"] = "ALL";
  doc["text"] = "Quick Msg!";
  doc["loc"] = getLocation();
  doc["hops"] = 0;
  doc["time"] = millis();

  // Mark as seen by self
  seen_ids[seen_idx] = mid;
  seen_idx = (seen_idx + 1) % 10;

  String output;
  serializeJson(doc, output);

  updateDisplay("Sending...", "Quick Message");
  radio.transmit(output);
  radio.startReceive();
}
