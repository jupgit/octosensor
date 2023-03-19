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

int octoSensor[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int previousOctoSensor[8] = {1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023};



////

//  ESPNOW
#include <esp_now.h>
#include <WiFi.h>

#include <Wire.h>


// TFT LCD DISPLAY 320x240
#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library
TFT_eSPI tft = TFT_eSPI();       // Invoke custom library
 

/// DEFINE COLORS

#define TFT_GRAY 0x7BEF
#define TFT_LIGHT_GRAY 0xC618
#define TFT_DARK_GRAY 0x632C
#define TFT_GREEN 0x07E0
#define TFT_LIME 0x87E0
#define TFT_BLUE 0x001F
#define TFT_RED 0xF800
#define TFT_AQUA 0x5D1C
#define TFT_YELLOW 0xFFE0
#define TFT_MAGENTA 0xF81F
#define TFT_CYAN 0x07FF
#define TFT_DARK_CYAN 0x03EF
#define TFT_ORANGE 0xFCA0
#define TFT_PINK 0xF97F
#define TFT_BROWN 0x8200
#define TFT_VIOLET 0x9199
#define TFT_SILVER 0xA510
#define TFT_GOLD 0xA508
#define TFT_NAVY 0x000F
#define TFT_MAROON 0x7800
#define TFT_PURPLE 0x780F
#define TFT_OLIVE 



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





//   TTTTT    X   X
//     T       X X
//     T        X
//     T       X X
//     T      X   X

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


  Serial.print("COMMAND: ");
  Serial.println(incomingCommand);
    Serial.print("NODE: ");
  Serial.println(incomingNode);
    Serial.print("VALUE: ");
  Serial.println(incomingValue);
  

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
  Serial.begin(9600);









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


//Set up the display
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);

  // MENU HEADER
  // MENU BAR    
  tft.fillRect(0, 0, 240, 30, TFT_DARK_GRAY);
  // SETA MENU ARROW
  tft.fillTriangle(18,10, 18,20, 10,15, TFT_WHITE);
  // MENU HEADER TEXT
  tft.setCursor(30, 8);
  tft.println(F("Sensor readings"));
  
  // PRINT NUMBERS FOR 1 TO 8
  for (int s=0; s<=7; s++) {  
    tft.setCursor(0, 80+(s*25));
    tft.print(s+1);  

  }



  
}








// *******************************************************
//
//                    L  O   O   P
//
// *******************************************************

 
void loop() {
  getSyncMillis();
 
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
  //updateDisplay();
  
   updateSensorsBarGraph();


  delay(50);

  //update8x8();



  
}





////
////
////  F U N C T I O N S 
////
////


void updateSensorsBarGraph() {

  for (int s=0; s<=7; s++) {  
    if (octoSensor[s] != previousOctoSensor[s]) {
      
      // ERASES BACKGROUND OF THE BAR
      tft.fillRect(30, 76+(s*25), 210, 24, TFT_BLACK);
      // DRAWS THE BAR
      tft.fillRect(30, 76+(s*25), octoSensor[s]/5, 24, TFT_RED);
      // PRINTS THE SENSOR VALUE
      tft.setCursor(35, 80+(s*25));
      tft.print(octoSensor[s]);


      //ISADORA  
      Serial.print((s+1),DEC); // discuss in later
      Serial.print(octoSensor[s]); //Send a value to computer.
      Serial.println(); //Send a value to eom
      
  
      previousOctoSensor[s] = octoSensor[s];
    }
  }
  
}



void getSyncMillis(){

  octoCommand = 10;   // comando 10 = send sync
  octoNode = 10;
  octoValue = millis()/100;
  
}
