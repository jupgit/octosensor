/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-two-way-communication-esp32/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/


////

// ARRAY WITH 8 SENSORS READINGS

int octoSensor[8];



////

//  ESPNOW
#include <esp_now.h>
#include <WiFi.h>

#include <Wire.h>

// OLED
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


// 8X8 LED MATRIX
#include <SPI.h>
#include "LedMatrix.h"
 
#define NUMBER_OF_DEVICES 1
#define CS_PIN 5
LedMatrix ledMatrix = LedMatrix(NUMBER_OF_DEVICES, CS_PIN);




// ************************************************************
// REPLACE WITH THE MAC Address of your receiver 
uint8_t broadcastAddress[] = {0xC4, 0x4F, 0x33, 0x3E, 0xCC, 0x21};
// ************************************************************


// Define variables to store BME280 readings to be sent
int octoCommand; 
int octoNode;
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

// Create a struct_message called BME280Readings to hold sensor readings
struct_message OCTOReadings;        //OctoSensorReadings;

// Create a struct_message to hold incoming sensor readings
struct_message incomingReadings;






// Callback when data is sent
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





//   RRRR    X   X
//   R   R    X X
//   RRRR      X
//   R R      X X
//   R  R    X   X

// Callback when data is received

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  Serial.print("Bytes received: ");
  Serial.println(len);
  incomingCommand = incomingReadings.comm;   //COMMAND
  incomingNode = incomingReadings.node;      //NODE
  incomingValue  = incomingReadings.value;   //VALUE

//////////////////////////////////////////////////////////////////////////////////////
// REMOTE SENSOR VALUE into its position in Array 
   octoSensor[incomingNode-1] = incomingValue;
//////////////////////////////////////////////////////////////////////////////////////


  

}




 
// *******************************************************
//
//                  S   E   T   U   P
//
// *******************************************************
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

  // Init OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }



// 8X8 LED MATRIX
 ledMatrix.init();
 ledMatrix.setIntensity(4); // range is 0-15

 ledMatrix.clear();
ledMatrix.commit();


// ******************************************************* ESPNOW COMM SETUP

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
  esp_now_peer_info_t peerInfo;
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
}








// *******************************************************
//
//                    L  O   O   P
//
// *******************************************************

 
void loop() {
  getReadings();
 
  // Set values to send
  OCTOReadings.comm = octoCommand;
  OCTOReadings.node = octoNode;
  OCTOReadings.value = octoValue;

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &OCTOReadings, sizeof(OCTOReadings));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  updateDisplay();
  delay(50);

  update8x8();



  
}




void getReadings(){

  octoCommand = 10;
  octoNode = 10;
  octoValue = millis()/100;
  
}

void updateDisplay(){

  /*
  // Display Readings on OLED Display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("INCOMING READINGS");
  display.setCursor(0, 15);
  display.print("Command: ");
  display.print(incomingCommand);
  display.setCursor(0, 25);
  display.print("Node: ");
  display.print(incomingNode);
  display.setCursor(0, 35);
  display.print("Value: ");
  display.print(incomingValue);
  display.setCursor(0, 56);
  display.print(success);
  display.display();
  */
  
  // Display Readings in Serial Monitor
  Serial.println("INCOMING READINGS");
  Serial.print("Command: ");
  Serial.print(incomingReadings.comm);
  Serial.print(" Node: ");
  Serial.print(incomingReadings.node);
  Serial.print(" Value: ");
  Serial.print(incomingReadings.value);
  Serial.println();
}


void update8x8() {

ledMatrix.clear();




for (int j=0; j<8; j++) {
  int nodeBar = map(octoSensor[j], 0, 250, 0, 7);
    for (int i=0; i<=nodeBar; i++) {
          ledMatrix.setPixel(i, j); 
    }
}
  
ledMatrix.commit();




}
