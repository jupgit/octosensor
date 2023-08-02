///////////////////////////////////////////////////////////
//
//  O C T O  S E N S O R
//  A wireless system with 8 laser range finder modules
//  and a central unit to be used in interactive 
//  art installations.
//
//  Project by Juliano Prado (2021) - Brazil
// 
//  Revision: 08.2023
//
//  https://github.com/jupgit/octo-sensor
//
//  TF LUNA I2C VERSION
//
//  Colaborators:
//   Paulo Cesar Teles (PhD)
//   Klebert
//   Mariana Arias
//   Igor
//   Matheus Ponte
//   
///////////////////////////////////////////////////////////


// OCTOSENSOR NODE SETUP
const int nodeNumber = 3;   // defines the Octosensor node number

// INTERACTIVE STATE MACHINE SETUP IN cm
int interact_min = 60;
int shot_min = 25;

int dist_min = 1;
int dist_max = 400;

String statusMachine = "IDLE";   // Status: IDLE, INTERACT, SHOT
String lastStatusMachine = statusMachine;  // memory of last state

// VARIABLES FOR THE LED INDICATOR
int R_index = 1;
int G_index = 1;
int B_index = 1;

// ANIMATION FUNCTIONS VARIABLES

int last_ledX = 0;

//strobe variables
unsigned long strobe_millis_counter = millis();
unsigned long strobe_millis_counter_old = millis();

// fire variables
bool gReverseDirection = false;

/////////////////

// SOUND ON/OFF
bool soundOn = true;


// ESP NOW LIBS
#include <esp_now.h>
#include <WiFi.h>

// I2C COMMUNICATION LIB (Sensor and OLED DIsplay)
#include <Wire.h>

// OLED DISPLAY
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// BUZZER
#include <CuteBuzzerSounds.h>
#define BUZZER_PIN 15

// TF-Luna Sensor
#include <TFLI2C.h>      // TFLuna-I2C Library v.0.1.1
TFLI2C tflI2C;           /// Initialize sensor object

int16_t  dist;    // distance in centimeters
int16_t  tfAddr = TFL_DEF_ADR;  // Use this default I2C address or
                                // set variable to your own value


// LASER POINTER
int laserPoniterPin = 5;


// COMMUNICATION SETUP


// REPLACE WITH THE MAC Address of your receiver 
//uint8_t broadcastAddress[] = {0x7C, 0x9E, 0xBD, 0x45, 0xB3, 0xC0}; // OCTOSENSOR #4
uint8_t broadcastAddress[] = {0xC4, 0x4F, 0x33, 0x3E, 0xE7, 0x6D}; // CENTRAL PROTOBOARD OLED


// Define variables to store readings to be sent
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
struct_message OCTOReadings;

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
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  Serial.print("Bytes received: ");
  Serial.println(len);
  incomingCommand = incomingReadings.comm;
  incomingNode = incomingReadings.node;
  incomingValue = incomingReadings.value;
}


 
// LED STRIP //////////////////////////////////////////////////////////////////////////////////////////////////

#include <FastLED.h>
#define LED_PIN     26
#define NUM_LEDS    30   /////// NUMERO DE LEDS DA FITA ////////////////////// 
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define BRIGHTNESS  128
// We're using two LED strips, one for the show, othe on the sensor as a monitor
// (see File>Examples>FASTLED>ArrayOfLedArrays)
// MONITOR LED STRIP (only 3 LEDs)
#define NUM_STRIPS 2
//CRGB leds[NUM_LEDS];
CRGB LEDstrip[NUM_STRIPS][NUM_LEDS];





// *******************************************************
//
//                  S   E   T   U   P
//
// *******************************************************




void setup() {
  // Init Serial Monitor
  Serial.begin(115200); 

  // SENSOR
  Wire.begin();           // Initalize Wire library

  // Init OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
 
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
// FASTLED SETUP
 ////////////////////////////////////////////////////////////

// LED STRIP  2 // 3 LEDS FOR MONITOR
  //LEDS.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, 25>(LEDstrip[0], 3); 



// BUZZER
  cute.init(BUZZER_PIN);
  cute.play(S_FART3);
  
  Serial.println("Buzzer");

  ledcSetup(0, 2000, 8);
  ledcAttachPin(BUZZER_PIN, 0);

// Laser Pointer ON

pinMode(laserPoniterPin, OUTPUT);
digitalWrite(laserPoniterPin, 1);  // turns on LASER POINTER

    Serial.println("Laser");


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
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) & OCTOReadings, sizeof(OCTOReadings));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }


  updateDisplay();





// STATUS MACHINE
// Defines the status according to the distance
// IDLE | INTERACT | SHOT
//

if (octoValue > interact_min) { statusMachine = "IDLE"; };
if (octoValue <= interact_min && octoValue > shot_min) { statusMachine = "INTERACT"; };
if (octoValue <= shot_min) { statusMachine = "SHOT"; };

if (lastStatusMachine != statusMachine) {
  if (soundOn) { 
    ledcWrite(0, 255);   // volume
    ledcWriteTone(0, 2000);  // frequency
    delay(5);
    ledcWrite(0, 0);
  }    // beeps when status change
  lastStatusMachine = statusMachine;   // updates last Status
 }

updateLEDindicator();


  delay(100);
  
}




void getReadings(){


if( tflI2C.getData( dist, tfAddr)) // If read okay...
    {
        Serial.print("Dist: ");
        Serial.println(dist);          // print the data...
        //dist = tfDist;

    }
    else dist = -1;

  
  octoCommand = 1;
  octoNode = nodeNumber;
  octoValue = dist;
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
  
  /*/ Display Readings in Serial Monitor
  Serial.println("INCOMING READINGS");
  Serial.print("Command: ");
  Serial.println(incomingReadings.comm);
  Serial.print("Node: ");
  Serial.println(incomingReadings.node);
  Serial.print("Value: ");
  Serial.print(incomingReadings.value);
  Serial.println();
  */
}



void updateLEDindicator() {

// MONITOR (INDICATOR) LED STRIP
  int LEDmonitor = map(octoValue, dist_min, dist_max, 3,255);
  LEDmonitor = constrain(LEDmonitor, 3, 255);



  // Defines the color according to the the state

  
  if (statusMachine == "IDLE") {
       R_index = 0;
       G_index = 0;
       B_index = 1;
  }

  if (statusMachine == "INTERACT") {
       R_index = 0;
       G_index = 1;
       B_index = 0;
  }

  if (statusMachine == "SHOT") {
       R_index = 1;
       G_index = 0;
       B_index = 0;
  }

  Serial.println (statusMachine);


  // Set monitor PIXELS
  LEDstrip[0][1].setRGB(LEDmonitor*R_index, LEDmonitor*G_index, LEDmonitor*B_index); // COR LED DO MEIO
  LEDstrip[0][0].setRGB(LEDmonitor*R_index, LEDmonitor*G_index, LEDmonitor*B_index); // COR LED DO LADO
  LEDstrip[0][2].setRGB(LEDmonitor*R_index, LEDmonitor*G_index, LEDmonitor*B_index); // COR LEDO DO LADO
  FastLED.show();

}





/////
/////   C R É D I T O S
/////



/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-two-way-communication-esp32/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

/*
 * Author: Everton Ramires
 * More Tutorial: https://www.youtube.com/channel/UC7zG4YQJc8v-9jNGQknOx5Q
 * 
 * 
 
 CURRENT SOUND OPTIONS: 
 (PT-BR)OPÇÕES ATUAIS DE SONS:
 
 S_CONNECTION   S_DISCONNECTION S_BUTTON_PUSHED   
 S_MODE1        S_MODE2         S_MODE3     
 S_SURPRISE     S_OHOOH         S_OHOOH2    
 S_CUDDLY       S_SLEEPING      S_HAPPY     
 S_SUPER_HAPPY  S_HAPPY_SHORT   S_SAD       
 S_CONFUSED     S_FART1         S_FART2     
 S_FART3        S_JUMP 20

 */


