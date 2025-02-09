// File: espnow_receiver.ino
#include <esp_now.h>
#include <WiFi.h>

// Structure to hold received data
typedef struct {
    int sensorID;
    int sensorValue;
    int sensorCommand;
} SensorData;

SensorData receivedData;

// Callback when data is received (Updated for ESP-IDF v5.x)
void onDataReceived(const esp_now_recv_info_t *recv_info, const uint8_t *incomingData, int len) {
    memcpy(&receivedData, incomingData, sizeof(receivedData));

    Serial.println("Received Sensor Data:");
    Serial.print("Sensor ID: "); Serial.println(receivedData.sensorID);
    Serial.print("Sensor Value: "); Serial.println(receivedData.sensorValue);
    Serial.print("Sensor Command: "); Serial.println(receivedData.sensorCommand);
    Serial.println("----------------------");
}

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);  // Set device in station mode

    // Initialize ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Register callback function for received data (Updated API)
    esp_now_register_recv_cb(onDataReceived);

    Serial.println("ESP-NOW Receiver Ready");
}

void loop() {
    // Keep looping and waiting for data
    delay(1000);
}
