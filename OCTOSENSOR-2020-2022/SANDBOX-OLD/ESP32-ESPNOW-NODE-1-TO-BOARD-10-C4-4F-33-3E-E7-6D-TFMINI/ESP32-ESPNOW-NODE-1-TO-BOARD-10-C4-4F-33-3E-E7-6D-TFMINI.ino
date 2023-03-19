///////////////////////////////////////////////////////////
//
//  O C T O  S E N S O R
//  A wireless system with 8 laser range finder modules
//  and a central unit to be used in interactive 
//  art installations.
//
//  Project by Juliano Prado (2021) - Brazil
//
//  https://github.com/jupgit/octo-sensor
//
///////////////////////////////////////////////////////////


// NODE SETUP
const int nodeNumber = 2;   // defines the node number






// tfmini sensor
#include <HardwareSerial.h>
HardwareSerial MySerial(2);  /// ***TROCAR PELO CODIGO NATIVO DO ESP32

#include "TFMini.h"
TFMini tfmini;


// ESP NOW
#include <esp_now.h>
#include <WiFi.h>

// OLED DISPLAY
#include <Wire.h>
//#include <Adafruit_Sensor.h>
//#include <Adafruit_BME280.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


// ************************************************************
// REPLACE WITH THE MAC Address of your receiver 
uint8_t broadcastAddress[] = {0xC4, 0x4F, 0x33, 0x3E, 0xE7, 0x6D};
// ************************************************************

// Define variables to be sent
int octoCommand;  
int octoNode = nodeNumber ; 
int octoValue;

// Define variables to store incoming readings
int incomingCommand; 
int incomingNode;
int incomingValue;

// Variable to store if sending data was successful
String success;

//Structure example to send data
//Must match the receiver structure
typedef struct struct_message {
    int comm;
    int node;
    int value;
} struct_message;

// Create a struct_message called OCTOReadings to hold sensor readings
struct_message OCTOReadings;    

// Create a struct_message to hold incoming sensor readings
struct_message incomingReadings;

// Variables for sensor 65k error detection
  int lastDist = 0;        // keeps last sensor reading
  int errorCount = 0;       // counts how many error ( = reading of 65633)
  int errorLimit = 50;     // max error count until sensor reset
  








//
// Callback when data is sent (TX)
//
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "TX OK" : "TX Fail");
  if (status ==0){
    success = "TX OK";
  }
  else{
    success = "TX Fail";
  }
}


// 
// Callback when data is received (RX)
//
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  Serial.print("Bytes received: ");
  Serial.println(len);
  incomingCommand = incomingReadings.comm;
  incomingNode = incomingReadings.node;
  incomingValue  = incomingReadings.value;

}


// *******************************************************
//
//                  S   E   T   U   P
//
// *******************************************************
 
void setup() {
  
  // Init Serial Monitor
  Serial.begin(115200);
  
  // SENSOR
  MySerial.begin(115200);
  tfmini.begin(&MySerial);
  delay(100);

  // Init OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
 
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
  //
  
  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  //
  // Register for a callback function that will be called when data is received
  //
  esp_now_register_recv_cb(OnDataRecv);
}



// *******************************************************
//
//                    L  O   O   P
//
// *******************************************************

void loop() {
  
  getSensorReadings();
 
  // Set values to send
  OCTOReadings.comm = octoCommand;
  OCTOReadings.node = octoNode;
  OCTOReadings.value = octoValue;

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &OCTOReadings, sizeof(OCTOReadings));
   
  /*
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  */

  
  updateDisplay();
  delay(25);
  
}











void getSensorReadings(){


//////////////////////////////////////////////////////////
// SENSOR READING
//////////////////////////////////////////////////////////

   
   
   // EMPTY SERIAL FROM SENSOR **
   while(MySerial.read() >= 0) ; // flush the received buffer
   delay(15);
   // ** We have to read all the serial to empty it before taking the measure because
   // ** the sensor sends a read every 10ms and floods the buffer. No library enables to configure
   // ** the sampling interval nor entering the 'single scan' mode.
   // ** So this is a WORKAROUND that took some months to figure out :)


   // SENSOR READ
   uint16_t dist = tfmini.getDistance();
   uint16_t strength = tfmini.getRecentSignalStrength();

   // VU Meter Mapping
   //MeterValue = map(dist, 40, 250, 0, 80);

  // VALUES FOR THE ESPNOW WIRELESS MESSAGE
  octoCommand = 0;
  octoNode = nodeNumber;
  octoValue = dist;

  Serial.print("Local Sensor: ");
  Serial.println(octoValue);
  
}











void updateDisplay(){
  // Display Readings on OLED Display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("OCTOsensor ");
  display.print(nodeNumber);

  display.setCursor(0, 20);
  display.setTextSize(3);
  display.print(octoValue);

  display.setTextSize(1);
  display.setCursor(0, 56);
  display.print(success);
  display.print("   SYNC: ");
  display.print(incomingValue);
  display.display();
  
  // Display Readings in Serial Monitor
  Serial.println("INCOMING READINGS");
  Serial.print("Command: ");
  Serial.println(incomingReadings.comm);
  Serial.print("Node: ");
  Serial.println(incomingReadings.node);
  Serial.print("Value: ");
  Serial.print(incomingReadings.value);
  Serial.println();
}



// CREDITS:
/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-two-way-communication-esp32/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/
