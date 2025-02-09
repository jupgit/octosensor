
// DHT Sensor
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 4     // Digital pin connected to the DHT sensor 
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

/// moisture sensor
const int sensorPin = 32;
const int airValue = 3620;
const int waterValue = 1680;
int soilMoistureValue = 0;
int soilMoisturePercent = 0;

// // OCTOSENSOR NODE SETUP
const int nodeNumber = 3;   // defines the Octosensor node number

// ESP NOW LIBS
#include <esp_now.h>
#include <WiFi.h>

// COMMUNICATION SETUP


// REPLACE WITH THE MAC Address of your receiver 
//uint8_t broadcastAddress[] = {0x94, 0xB9, 0x7E, 0xC2, 0xCA, 0xD0}; // CENTRAL ANTIGENOUS
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // ALL PAIRED DEVICES


// Define variables to store readings to be sent
int plantTemp;
int plantUmid;
int plant1Moisture;

// Define variables to store incoming readings
//int incomingCommand;
//int incomingNode;
//int incomingValue;

// Variable to store if sending data was successful
String success;

//Structure example to send data
//Must match the receiver structure
typedef struct struct_message {
    int comm;
    int node;
    int value;
} struct_message;

// Create a struct_message called plantReadings to hold sensor readings
struct_message plantReadings;

// Create a struct_message to hold incoming sensor readings
//struct_message incomingReadings;

esp_now_peer_info_t peerInfo;

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status ==0){
    success = "TX OK";
  }
  else{
    success = "TX Fail";
  }
}

/*
// Callback when data is received
void OnDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *incomingData, int len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  Serial.print("Bytes received: ");
  Serial.println(len);
  incomingCommand = incomingReadings.comm;
  incomingNode = incomingReadings.node;
  incomingValue = incomingReadings.value;
  */
}



//----------------------------------------------------------------------------

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);


dht.begin();
  Serial.println(F("DHTxx Unified Sensor Example"));
  // Print temperature sensor details.
  sensor_t sensor;

  dht.temperature().getSensor(&sensor);


      ///---- monitor
      Serial.println(F("------------------------------------"));
      Serial.println(F("Temperature Sensor"));
      Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
      Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
      Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
      Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
      Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
      Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
      Serial.println(F("------------------------------------"));
      // Print humidity sensor details.
      dht.humidity().getSensor(&sensor);
      Serial.println(F("Humidity Sensor"));
      Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
      Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
      Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
      Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
      Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
      Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
      Serial.println(F("------------------------------------"));

      
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;




}

void loop() {

  soilMoistureValue = analogRead(sensorPin);

  //Serial.print("Sensor: ");
  Serial.print(soilMoistureValue);
  Serial.print(" , ");
  //Serial.print(soilMoisturePercent);
  //Serial.println(" %");

  /// SENSOR DHT

  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    //Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
      Serial.print(" , ");

    //Serial.println(F("°C"));
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    //Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    //Serial.println(F("%"));
  }

  Serial.println();

  delay(1000);


}
