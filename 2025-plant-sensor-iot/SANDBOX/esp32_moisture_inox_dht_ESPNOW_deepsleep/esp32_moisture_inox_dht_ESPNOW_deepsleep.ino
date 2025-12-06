// IoT Sensor Node with Improved Modularity and Error Handling

#define uS_TO_S_FACTOR 1000000ULL   // Conversion factor for microseconds to seconds
#define SLEEP_DURATION 300          // Sleep duration in seconds (5 minutes)
#define ACTIVE_DURATION 60          // Active time in seconds (1 minute)
#define SAMPLE_COUNT 5              // Number of samples for averaging
#define RETRY_LIMIT 3               // Max retries for ESP-NOW transmission

#define ANALOG_PIN A0               // Analog pin to read sensor data
#define WAKE_BUTTON_PIN GPIO_NUM_33 // GPIO pin connected to the wake-up button

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <esp_now.h>
#include <WiFi.h>

#define DHTPIN 4                    // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11               // DHT 11 sensor type

DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;

const int sensorPin = 32;
const int airValue = 3620;
const int waterValue = 1680;

// Structure for ESP-NOW communication
typedef struct struct_message {
    float temp;
    float umid;
    int moisture1;
} struct_message;

struct_message plantReadings, incomingReadings;
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.print("\r\nLast Packet Send Status:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void OnDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *incomingData, int len) {
    memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
    Serial.print("Bytes received: ");
    Serial.println(len);
}

void initESPNow() {
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
    esp_now_register_send_cb(OnDataSent);
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;  
    peerInfo.encrypt = false;
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
        return;
    }
    esp_now_register_recv_cb(OnDataRecv);
}

int readSoilMoisture() {
    int totalValue = 0;
    for (int i = 0; i < SAMPLE_COUNT; i++) {
        totalValue += analogRead(ANALOG_PIN);
        delay(5000);
    }
    return totalValue / SAMPLE_COUNT;
}

void readDHTSensor() {
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    plantReadings.temp = isnan(event.temperature) ? -1 : event.temperature;

    dht.humidity().getEvent(&event);
    plantReadings.umid = isnan(event.relative_humidity) ? -1 : event.relative_humidity;
}

bool sendSensorData() {
    for (int attempt = 0; attempt < RETRY_LIMIT; attempt++) {
        if (esp_now_send(broadcastAddress, (uint8_t *)&plantReadings, sizeof(plantReadings)) == ESP_OK) {
            Serial.println("Data sent successfully");
            return true;
        } else {
            Serial.print("Retry ");
            Serial.println(attempt + 1);
        }
        delay(1000);
    }
    Serial.println("Failed to send data after retries");
    return false;
}

void setup() {
    Serial.begin(115200);
    pinMode(WAKE_BUTTON_PIN, INPUT_PULLUP);
    dht.begin();
    initESPNow();
}

void loop() {
    esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
    Serial.println(wakeup_reason == ESP_SLEEP_WAKEUP_TIMER ? "Woke up from timer." : "Woke up from external trigger.");

    plantReadings.moisture1 = readSoilMoisture();
    readDHTSensor();
    sendSensorData();

    esp_sleep_enable_timer_wakeup(SLEEP_DURATION * uS_TO_S_FACTOR);
    esp_sleep_enable_ext0_wakeup(WAKE_BUTTON_PIN, 0);
    esp_deep_sleep_start();
}
