//=====================================================================================
// SENSORES
// Hardware Connections
//  - DHT: signal to pin 4
//  - Moisture alalog to pin 32
//=====================================================================================

// Pin that will switch on/off the sensors
#define SENSORSONOFF_PIN 2 

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

//=====================================================================================
// OLED DISPLAY
// Hardware Connections
//  - SDA to pin 21 / SCL to pin 22
//=====================================================================================

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);



//=====================================================================================
// DEEP SLEEP
// Hardware Connections
//  - Push Button to GPIO 33 pulled down with a 10K Ohm resistor.
//=====================================================================================

#include "driver/rtc_io.h"

#define BUTTON_PIN_BITMASK(GPIO) (1ULL << GPIO)  // Define GPIO bitmask
#define USE_EXT0_WAKEUP          1               // 1 = EXT0 wakeup, 0 = EXT1 wakeup
#define WAKEUP_GPIO              GPIO_NUM_33     // RTC IO pin for external wake-up

#define TIME_TO_SLEEP 10  // Time in seconds before ESP32 wakes up from deep sleep

RTC_DATA_ATTR int bootCount = 0;  // Store boot count in RTC memory




//=====================================================================================
// COMMUNICATION
//=====================================================================================

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
float plantTemp;
float plantUmid;
int plant1Moisture;

//Define variables to store incoming readings
float incomingTemp;
float incomingUmid;
int incomingMoisture1;

// Variable to store if sending data was successful
String success;

//Structure example to send data
//Must match the receiver structure
typedef struct struct_message {
    float temp;
    float umid;
    int moisture1;
} struct_message;

// Create a struct_message called plantReadings to hold sensor readings
struct_message plantReadings;

// Create a struct_message to hold incoming sensor readings
struct_message incomingReadings;

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


// Callback when data is received
void OnDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *incomingData, int len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  Serial.print("Bytes received: ");
  Serial.println(len);
  incomingTemp = incomingReadings.temp;
  incomingUmid = incomingReadings.umid;
  incomingMoisture1 = incomingReadings.moisture1;
}
  






//=====================================================================================
// S E T U P
//=====================================================================================





void setup() {

Serial.begin(9600);

// Liga e desliga energia dos sensores
pinMode(SENSORSONOFF_PIN, OUTPUT);
digitalWrite(SENSORSONOFF_PIN, HIGH);  // Turn ON SENSORS

Serial.println("Sensores energizados");
delay(2000);

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



// Init OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    //Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }


  // Display Hello
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Hello");
  display.display();


 ////////////////////////////////////////////////////////////
 // ESPNOW COMMUNICATION SETUP
 ////////////////////////////////////////////////////////////
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);


////////////////////////////////////////////////////////////
 // DEEP SLEEP SETUP
 ////////////////////////////////////////////////////////////


// Increment boot count and print it
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

  // Print wake-up reason
  print_wakeup_reason();

  /*
    Configure Wake-up Sources
    =========================
    1. External Wake-up (Button Press)
    2. Timer Wake-up (After X Seconds)
  */

  // Enable Timer Wake-up
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * 1000000);  // Convert seconds to microseconds

#if USE_EXT0_WAKEUP
  // Enable External Wake-up (EXT0)
  esp_sleep_enable_ext0_wakeup(WAKEUP_GPIO, 1);  // 1 = HIGH, 0 = LOW
  rtc_gpio_pullup_dis(WAKEUP_GPIO);
  rtc_gpio_pulldown_en(WAKEUP_GPIO);

#else  // EXT1 WAKEUP
  // Enable External Wake-up (EXT1)
  esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK(WAKEUP_GPIO), ESP_EXT1_WAKEUP_ANY_HIGH);
  rtc_gpio_pulldown_en(WAKEUP_GPIO);  // GPIO33 tied to GND in order to wake up on HIGH
  rtc_gpio_pullup_dis(WAKEUP_GPIO);
#endif


////////////////////////////////////////////////////////////
 // SENSOR READING AND ESPNOW COMMUNICATION
 ////////////////////////////////////////////////////////////



// Turn on, read DHT and Soil Moisture sensors, turnoff sensors
readSensors();

updateDisplay();


// Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) & plantReadings, sizeof(plantReadings));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }







  delay(10000);



////////////////////////////////////////////////////////////
// End of SENSOR READING AND ESPNOW COMMUNICATION
////////////////////////////////////////////////////////////


  // Print message and enter deep sleep
  Serial.println("ESP32 going to sleep now...");
  esp_deep_sleep_start();

  Serial.println("This will never be printed");  // This line will never execute



}



//=====================================================================================
// L O O P
//=====================================================================================




void loop() {




}

//=====================================================================================
// F U N C T I O N S
//=====================================================================================

int sampledMoisture() {
  const int numSamples = 5;
  const int delayBetweenSamples = 2000; // in milliseconds
  int total = 0;

  for (int i = 0; i < numSamples; i++) {
    int sample = analogRead(sensorPin);
    total += sample;
    delay(delayBetweenSamples);
    Serial.print("sample "); Serial.print(i+1);
    Serial.print(" = "); Serial.println(sample);

  }

  int average = total / numSamples;
  Serial.print("average = "); Serial.println(average);

  return average;
}


void readSensors() {


//////////////////////////////
  //    SENSOR READINGS
  //////////////////////////////

  digitalWrite(SENSORSONOFF_PIN, HIGH);  // Turn ON SENSORS

  Serial.println("Sensors *** O N ***");
  delay(2000);


  //soilMoistureValue = analogRead(sensorPin);
  soilMoistureValue =sampledMoisture();
  // Set value to send
  plantReadings.moisture1 = soilMoistureValue;   // <<<< TO BE SENT

  /// SENSOR DHT
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
  // Set value to send
    plantReadings.temp = event.temperature;   // <<<< TO BE SENT
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
  // Set value to send
    plantReadings.umid = event.relative_humidity;   // <<<< TO BE SENT
  }

  //
  Serial.print("Sensors: ");
  Serial.print(plantReadings.moisture1); Serial.print(" , ");
  Serial.print(plantReadings.temp); Serial.print(" , ");
  Serial.println(plantReadings.umid);
 
  digitalWrite(SENSORSONOFF_PIN, LOW);  // Turn OFF SENSORS
  Serial.println("Sensors *** O F F ***");

}


void updateDisplay() {
  // Display Readings on OLED Display
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 16);
  //display.print("plantMonitor");

  display.print("T: "); display.println(plantReadings.temp);
  display.print("U: "); display.println(plantReadings.umid);
  display.print("M: "); display.println(plantReadings.moisture1);

  display.display();
}


void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0:
      Serial.println("Wakeup caused by external signal using RTC_IO");
      break;
    case ESP_SLEEP_WAKEUP_EXT1:
      Serial.println("Wakeup caused by external signal using RTC_CNTL");
      break;
    case ESP_SLEEP_WAKEUP_TIMER:
      Serial.println("Wakeup caused by timer");
      break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD:
      Serial.println("Wakeup caused by touchpad");
      break;
    case ESP_SLEEP_WAKEUP_ULP:
      Serial.println("Wakeup caused by ULP program");
      break;
    default:
      Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason);
      break;
  }
}