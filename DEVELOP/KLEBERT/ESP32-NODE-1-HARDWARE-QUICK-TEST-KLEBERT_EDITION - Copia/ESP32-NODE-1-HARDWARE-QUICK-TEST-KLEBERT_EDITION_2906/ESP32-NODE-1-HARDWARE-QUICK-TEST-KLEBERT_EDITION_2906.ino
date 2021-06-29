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
const int nodeNumber = 1;   // defines the node number



// tfmini sensor
#include <HardwareSerial.h>
HardwareSerial MySerial(2);  /// ***TROCAR PELO CODIGO NATIVO DO ESP32

#include "TFMini.h"
TFMini tfmini;


// ESP NOW
//#include <esp_now.h>
//#include <WiFi.h>

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
  


// LED STRIP //////////////////////////////////////////////////////////////////////////////////////////////////

#include <FastLED.h>
#define LED_PIN     26
#define NUM_LEDS    30   /////// NUMERO DE LEDS DA FITA ////////////////////// 
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define BRIGHTNESS  128
#define FRAMES_PER_SECOND 10 

// We're using two LED strips, one for the monitor, the for the show
// (see File>Examples>FASTLED>ArrayOfLedArrays)
#define NUM_STRIPS 2
//CRGB leds[NUM_LEDS];
CRGB LEDstrip[NUM_STRIPS][NUM_LEDS];

/*
// LED STRIP (OLD)
#include <FastLED.h>

#define LED_PIN     25
//#define CLOCK_PIN 4 
#define NUM_LEDS    3
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define BRIGHTNESS  200

CRGB leds[NUM_LEDS];
*/
// KLEBERT EDITION
bool gReverseDirection = false;
unsigned long int respira = 0; //Contador de Millis Universal
int r = 0; //Acendedor de Lampadas
int e = NUM_LEDS - 1; //Apagador de Lampadas
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
/*
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
*/

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
  MySerial.begin(115200);
      Serial.println("My Serial");

  tfmini.begin(&MySerial);
    Serial.println("TFMini begin");

  
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
//  WiFi.mode(MODE_STA);
/*
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


*/
// LED STRIPs
// KLEBERT EDITION
  FastLED.addLeds<LED_TYPE, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(250);
  
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
  
// buzzer
  cute.init(BUZZER_PIN);

    Serial.println("Buzzer");


// Laser Pointer ON

pinMode(laserPoniterPin, OUTPUT);
digitalWrite(laserPoniterPin, 1);  // turns on LASER POINTER

    Serial.println("Laser");

  
}

//Coisas da Demo Reel.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns


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
 // esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &OCTOReadings, sizeof(OCTOReadings));
   
  /*
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  */

  
  updateDisplay();


  int teste = map(octoValue, 30, 400, 0,255);
  teste = constrain(teste, 0, 255);

  // LED INDICATOR

  
  LEDstrip[0][1].setRGB(teste, teste/32,teste/8); // COR
  LEDstrip[0][0].setRGB(teste/8, teste/64,teste/16); // COR
  LEDstrip[0][2].setRGB(teste/8, teste/64,teste/16); // COR
   FastLED.show();

  

  // led strip
    int ledMeter = map (octoValue,30, 400, 0, 4);

//////////////////////////////////////////////////////////
// KLEBERT EDITION
//////////////////////////////////////////////////////////
    estado = 5;

/////////
//Relaxamento
////////
    int tensao = map(octoValue, 270, 360, 10, 100);
    if(estado == 3){
        Respirando(tensao, tensao); //Respiração em Canudo e nas cores dos Xacras como proposto pelo Paulo Teles
    }

////////
// Descarga
///////
    //int hell = map(octoValue, 180, 270, 10, 100);
    //Demo reel como sugerido pelo Juliano
    if( estado == 2){
      demo(100);
    }

////////
// Carga
///////
    int Piscalerta = map (octoValue, 80, 180, 100, 2000);
    int vermelhor = map(octoValue, 80, 180, 255,0);
    int verdor = map(octoValue, 80, 180, 0, 255);
    //Strobo
    if(estado == 1){
      trigged(vermelhor, verdor ,0,Piscalerta); //Quando mais a pessoa se aproxima, mais rapido pisca como proposto por Juliano
    }

////////
// Tensão
///////
    int pepper = map(octoValue,30, 90, 10,100);
    //Observação: usei uma função para chamar outra função para poder mudar o tempo com facilidade e evitar bugs.
    // Fire 2012 - Sugestão do Juliano.
    if(estado == 0){
    Carvao(pepper);
  }

///////    
//Idel 
//////
  //Pacifica - Sugerida por Juliano
    if(estado == 4){
        pacifica_loop(255, 145, 200);
        FastLED.show();
    }

///////
// Sobe
//////
    if(estado == 5){
      Sobe(50, 50);
    }
}
//////////////////////////////////////////////////////////
// KLEBERT EDITION FUNCIONs
//////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////
void Sobe( int inspira, int expira){
  //int ledAcende = map (teste,0, 255, 0, NUM_LEDS);
    FastLED.setBrightness(100);

  if( millis() - respira >= inspira){
    leds[NUM_LEDS - 1] = CRGB::Red;
    leds[0] = CRGB::Red;
    leds[r] = CRGB::Red;
    blur1d(leds, NUM_LEDS, 15);
    fadeToBlackBy(leds, NUM_LEDS, 200);
   // leds[r-1] = CRGB::Black;
    FastLED.show();
    
    r += 1;
    respira += inspira;
  }
   if(r == NUM_LEDS){
        leds[NUM_LEDS - 1] = CRGB::Green;
        leds[0] = CRGB::Green;
        r = 1;
        
  }

   
   }

   
    

/////////////////////////////////////////////////////////////
//Violeta = 0x8A2BE2
//Indigo = 0x4B0082
//Ciano = 0x00FFFF
//Green = 0x008000
//Yellow = 0xFFFF00
//Orange = 0xFFA500
//Red = 0xFF0000
CRGBPalette16 Nossa_Paleta =
     { 0x8A2BE2, 0x8A2BE2, 0x4B0082, 0x4B0082, 0x00FFFF, 0x00FFFF, 0x008000, 0x008000, 
      0xFFFF00, 0xFFFF00, 0xFFA500, 0xFFA500, 0xFF0000, 0xFF0000, 0x8A2BE2, 0x8A2BE2 };

//////////////////////////

void Respirando( int inspira, int expira) {
  if(r < NUM_LEDS){
    if(millis() - respira >= inspira){
      Arcoiris(r);
      blur1d(leds, NUM_LEDS, 10);
      FastLED.show();
      respira += inspira;
      r += 1;
      }
    }
    if(r == NUM_LEDS){
      if(millis() - respira >= expira){
        leds[e] = CRGB::Black;
        //blur1d(leds, NUM_LEDS, 15);
        //fadeToBlackBy(leds, NUM_LEDS, 24);
        FastLED.show();
        respira += expira;
        e -= 1;
        }
      }
      if(r == NUM_LEDS and e == 0){
        r = 0;
        e = NUM_LEDS - 1;
      }
  }

 //Strobo
void trigged(int R, int G, int B, int inspira){
  fill_solid( leds, NUM_LEDS, CRGB(R,G,B));
  if( millis() - respira >= inspira){
      FastLED.clear();
      respira += inspira;
      }
}


void Arcoiris(int r ){
  int intervalo = NUM_LEDS/7;
  if(r + 1 <= intervalo){
    leds[r] = ColorFromPalette( Nossa_Paleta, 1000, 255, currentBlending);   //leds[r] = CRGB::Violet;
  }else if(r + 1 <= 2*intervalo){
    leds[r] = ColorFromPalette( Nossa_Paleta, 7000, 255, currentBlending);    //leds[r] = CRGB::Indigo;
  }else if(r + 1 <= 3*intervalo){
    leds[r] = ColorFromPalette( Nossa_Paleta, 6000, 255, currentBlending);      //leds[r] = CRGB::Cyan;
  }else if(r + 1 <= 4*intervalo){
    leds[r] = ColorFromPalette( Nossa_Paleta, 5000, 255, currentBlending);     // leds[r] = CRGB::Green;
  }else if (r + 1 <= 5*intervalo){
    leds[r] = ColorFromPalette( Nossa_Paleta, 4000, 255, currentBlending);     // leds[r] = CRGB::Yellow;
  }else if (r + 1 <= 6*intervalo){
    leds[r] = ColorFromPalette( Nossa_Paleta, 3000, 255, currentBlending);      //leds[r] = CRGB::Orange;
  }else if(r + 1 <= 7*intervalo){
    leds[r] = ColorFromPalette( Nossa_Paleta, 2000, 255, currentBlending);      //leds[r] = CRGB::Red;
  }else if( r + 1 >= 7*intervalo){
    leds[r] = ColorFromPalette( Nossa_Paleta, 1000, 255, currentBlending);       //leds[r] = CRGB::Violet;
  }
 }
 
//////////////////////////////  
//PACIFICA
CRGBPalette16 pacifica_palette_1 = 
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x14554B, 0x28AA50 };
CRGBPalette16 pacifica_palette_2 = 
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x0C5F52, 0x19BE5F };
CRGBPalette16 pacifica_palette_3 = 
    { 0x000208, 0x00030E, 0x000514, 0x00061A, 0x000820, 0x000927, 0x000B2D, 0x000C33, 
      0x000E39, 0x001040, 0x001450, 0x001860, 0x001C70, 0x002080, 0x1040BF, 0x2060FF };


void pacifica_loop(uint8_t brg, int Sat1, int Sat2){
  static uint16_t sCIStart1, sCIStart2, sCIStart3, sCIStart4;
  static uint32_t sLastms = 0;
  uint32_t ms = GET_MILLIS();
  uint32_t deltams = ms - sLastms;
  sLastms = ms;
  uint16_t speedfactor1 = beatsin16(3, 179, 269);
  uint16_t speedfactor2 = beatsin16(4, 179, 269);
  uint32_t deltams1 = (deltams * speedfactor1) / 256;
  uint32_t deltams2 = (deltams * speedfactor2) / 256;
  uint32_t deltams21 = (deltams1 + deltams2) / 2;
  sCIStart1 += (deltams1 * beatsin88(1011,10,13));
  sCIStart2 -= (deltams21 * beatsin88(777,8,11));
  sCIStart3 -= (deltams1 * beatsin88(501,5,7));
  sCIStart4 -= (deltams2 * beatsin88(257,4,6));

  fill_solid( leds, NUM_LEDS, CRGB( 2, 6, 10));

  pacifica_one_layer( pacifica_palette_1, sCIStart1, beatsin16( 3, 11 * 256, 14 * 256), brg, 0-beat16( 301) );
  pacifica_one_layer( pacifica_palette_2, sCIStart2, beatsin16( 4,  6 * 256,  9 * 256), brg, beat16( 401) );
  pacifica_one_layer( pacifica_palette_3, sCIStart3, 6 * 256, brg, 0-beat16(503));
  pacifica_one_layer( currentPalette, sCIStart4, 5 * 256, brg, beat16(601));

  pacifica_add_whitecaps();

  pacifica_deepen_colors( Sat1, Sat2);
  }

void pacifica_one_layer( CRGBPalette16& p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff){
  uint16_t ci = cistart;
  uint16_t waveangle = ioff;
  uint16_t wavescale_half = (wavescale / 2) + 20;
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    waveangle += 250;
    uint16_t s16 = sin16( waveangle ) + 32768;
    uint16_t cs = scale16( s16 , wavescale_half ) + wavescale_half;
    ci += cs;
    uint16_t sindex16 = sin16( ci) + 32768;
    uint8_t sindex8 = scale16( sindex16, 240);
    CRGB c = ColorFromPalette( p, sindex8, bri, LINEARBLEND);
    leds[i] += c;
  }
  }

void pacifica_add_whitecaps(){
  uint8_t basethreshold = beatsin8( 9, 55, 65);
  uint8_t wave = beat8( 7 );
  
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    uint8_t threshold = scale8( sin8( wave), 20) + basethreshold;
    wave += 7;
    uint8_t l = leds[i].getAverageLight();
    if( l > threshold) {
      uint8_t overage = l - threshold;
      uint8_t overage2 = qadd8( overage, overage);
      leds[i] += CRGB( overage, overage2, qadd8( overage2, overage2));
    }
  }
  }

void pacifica_deepen_colors(int Sat1, int Sat2){
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    leds[i].blue = scale8( leds[i].blue,  Sat1); //145 
    leds[i].green= scale8( leds[i].green, Sat2); //200
    leds[i] |= CRGB( 2, 5, 7);
  }
  }
//  "Pacifica"
//  Gentle, blue-green ocean waves.
//  December 2019, Mark Kriegsman and Mary Corey March.
//  For Dan.
//

/////////////////////////////
//DEMO REEL
/////////////////////////////
void demo(int reel){
  if(millis() - respira >= reel){
  gPatterns[gCurrentPatternNumber]();
  FastLED.show();
    respira +=100;

  }
  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
}


#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void rainbow()
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter()
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter)
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti()
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

/////////////////////////
//FIRE
/////////////////////////

void Carvao(int queima){
  if(millis() - respira >= queima){
    Fogo();
    FastLED.show();
    respira += queima;
    }
  }
void Fogo()
{
  static byte heat[NUM_LEDS];
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }
    for( int j = 0; j < NUM_LEDS; j++) {
      //CRGB color = HeatColor( heat[j]);
      CRGB color = ColorFromPalette( Nossa_Paleta, heat[j], 255, currentBlending);

      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      leds[pixelnumber] = color;
    }
}
///////////////////////////////////////////////////////////
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
