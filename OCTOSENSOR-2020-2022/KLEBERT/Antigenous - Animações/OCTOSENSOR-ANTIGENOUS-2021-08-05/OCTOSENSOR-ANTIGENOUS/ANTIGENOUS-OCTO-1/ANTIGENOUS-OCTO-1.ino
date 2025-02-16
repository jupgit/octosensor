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
//  TF LUNA I2C VERSION
//
///////////////////////////////////////////////////////////


// NODE SETUP
const int nodeNumber = 1;   // defines the node number



// tfmini sensor
//#include <HardwareSerial.h>
//HardwareSerial MySerial(2);  /// ***TROCAR PELO CODIGO NATIVO DO ESP32

//#include "TFMini.h"
//TFMini tfmini;

#include <Wire.h>        // Wire library >> USED FOR TF-LUNA SENSOR AND OLED DISPLAY


// TF-Luna Sensor
#include <TFLI2C.h>      // TFLuna-I2C Library v.0.1.1

TFLI2C tflI2C;

int16_t  dist;    // distance in centimeters
int16_t  tfAddr = TFL_DEF_ADR;  // Use this default I2C address or
                                // set variable to your own value


// ESP NOW
#include <esp_now.h>
#include <WiFi.h>

// OLED DISPLAY
#//include <Wire.h>
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
//uint8_t broadcastAddress[] = {0x24, 0x62, 0xAB, 0xDD, 0x36, 0x78};
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
  


// LED STRIP //////////////////////////////////////////////////////////////////////////////////////////////////

#include <FastLED.h>
#define LED_PIN     26
#define NUM_LEDS    30   /////// NUMERO DE LEDS DA FITA ////////////////////// 
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define BRIGHTNESS  128
// We're using two LED strips, one for the monitor, the for the show
// (see File>Examples>FASTLED>ArrayOfLedArrays)
#define NUM_STRIPS 2
//CRGB leds[NUM_LEDS];
CRGB LEDstrip[NUM_STRIPS][NUM_LEDS];

// KLEBERT EDITION
bool gReverseDirection = false;
unsigned long int respira = 0; //Contador de Millis Universal
int r = 0; //Acendedor de Lampadas
int e = NUM_LEDS - 1; //Apagador de Lampadas
int Velocidade = 0; //Define modo
int estado = 0; //Define modo
// Estado == 0 - Idel
// Estado == 1 - Relaxamento
// Estado == 2 - Descarga
// Estado == 3 - Saturação
// Estado == 4 - Tensão
CRGB leds[NUM_LEDS];

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

#define COOLING  55
#define SPARKING 120

// BUZZER
#include <CuteBuzzerSounds.h>
#define BUZZER_PIN 15

// LASER POINTER

int laserPoniterPin = 5;



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
      Serial.println("Serial");

  
  // SENSOR
    Wire.begin();           // Initalize Wire library


  
  //MySerial.begin(115200);
  //    Serial.println("My Serial");
  //    tfmini.begin(&MySerial);
  //    Serial.println("TFMini begin");



  
  delay(100);

  // Init OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

    Serial.println("OLED");


  display.display();
  delay(500); // Pause for 2 seconds
 
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

    Serial.println("ESPNOW");



// LED STRIPs
  //LEDS.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, 25>(LEDstrip[0], 3);  // 3 LEDS FOR MONITOR
  //FastLED.addLeds<NEOPIXEL, 26>(LEDstrip[1], NUM_LEDS);
  FastLED.addLeds<LED_TYPE, 26, GRB>(leds, NUM_LEDS);

    
    FastLED.setBrightness(BRIGHTNESS);
    

// buzzer
  cute.init(BUZZER_PIN);
  cute.play(S_FART3);
  
  Serial.println("Buzzer");


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


  int teste = map(octoValue, 10, 150, 0,255);
  teste = constrain(teste, 0, 255);

  // LED INDICATOR

  
  LEDstrip[0][1].setRGB(teste/2, 0, 0); // COR
  LEDstrip[0][0].setRGB(teste/4, 0,0); // COR
  LEDstrip[0][2].setRGB(teste/4, 0, 0); // COR
   FastLED.show();

  

  // led strip


    int Velocidade = map (octoValue, 10, 140, 20, 1);
//////////////////////////////////////////////////////////
// KLEBERT EDITION
//////////////////////////////////////////////////////////

   if(octoValue > 300){
      estado = 0;
    }
    if(octoValue < 300){
    estado = 1;
    }
/////////
//Idel
////////
    if(estado == 0){
      int brilho = random(150, 250);
      fill_solid(leds, NUM_LEDS, CRGB::Orange);
      FastLED.show(); 
    }

////////
// Interação
///////
    if(estado == 1){
       PacificaTaubate(Velocidade, Velocidade*2, 250, 10);
       FastLED.show();
    }

}

void PacificaTaubate(int v1, int v2, int b, int sat) { 
    uint8_t sinBeat = beatsin8(v1, 2, NUM_LEDS -1, 0, 0);
    uint8_t sinBeat2 = beatsin8(v2, 0, NUM_LEDS -1, 0, 124);
    uint8_t sinBeat3 = beatsin8(v1, 0, NUM_LEDS -1, 0, 124);
    uint8_t sinBeat4= beatsin8(v2, 0, NUM_LEDS -1, 0, 0);
    FastLED.setBrightness(b);
    leds[sinBeat] = CRGB::DarkOrange;
    leds[sinBeat2] = CRGB::OrangeRed;
    leds[sinBeat3] = CRGB::Black;
    leds[sinBeat4] = CRGB::Black;
    blur1d(leds, NUM_LEDS, sat);
    fadeToBlackBy(leds, NUM_LEDS, 0);
}












void getSensorReadings(){


//////////////////////////////////////////////////////////
// SENSOR READING
//////////////////////////////////////////////////////////



if( tflI2C.getData( dist, tfAddr)) // If read okay...
    {
        Serial.print("Dist: ");
        Serial.println(dist);          // print the data...
        //dist = tfDist;

    }
    else dist = 400;
    
    //tflI2C.printStatus();           // else, print error.


   // SENSOR READ
   //uint16_t strength = tfmini.getRecentSignalStrength();

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
  
  The above copyright notice and this permission notice shall be included in a
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
