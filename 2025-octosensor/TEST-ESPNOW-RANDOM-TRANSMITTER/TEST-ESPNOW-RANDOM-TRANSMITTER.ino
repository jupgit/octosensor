// File: espnow_transmitter.ino
#include <esp_now.h>
#include <WiFi.h>

// Structure to hold the data to be sent
typedef struct {
    int sensorID;
    int sensorValue;
    int sensorCommand;
} SensorData;

SensorData dataToSend = {1, 0, 0};  // sensorID=1, sensorCommand=0

// MAC address of the receiver (replace with your receiver's MAC address)
uint8_t receiverMAC[] = {0x30, 0xC6, 0xF7, 0x23, 0x93, 0x84};

// Callback when data is sent
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.print("Delivery Status: ");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);  // Set device in station mode

    // Initialize ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Register callback function for sent data
    esp_now_register_send_cb(onDataSent);

    // Add the peer (receiver)
    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, receiverMAC, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
        return;
    }

    Serial.println("ESP-NOW Transmitter Ready");
}

void loop() {
    // Generate random sensor value (0 to 1023)
    dataToSend.sensorValue = random(0, 1024);

    // Send the data structure
    esp_err_t result = esp_now_send(receiverMAC, (uint8_t *)&dataToSend, sizeof(dataToSend));

    Serial.print("Sending - ID: "); Serial.print(dataToSend.sensorID);
    Serial.print(", Value: "); Serial.print(dataToSend.sensorValue);
    Serial.print(", Command: "); Serial.println(dataToSend.sensorCommand);

    if (result == ESP_OK) {
        Serial.println("Data sent successfully");
    } else {
        Serial.println("Error sending data");
    }

    delay(5000);  // Send data every 5 seconds
}
