///////////////////////////////////////////////////////////
//
//  O C T O  S E N S O R
//  A wireless system with 8 laser range finder modules
//  and a central unit to be used in interactive 
//  art installations.
//
//  Project by Juliano Prado (2021) - Brazil
// 
//  Revision: 01.2025
//  >>> ESPNOW receive callback updated
//  >>> Cute buzzer sound (ripped off :) - not working animore ESP32 with Core V 3.0.0
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

#include <FastLED.h>


// OCTOSENSOR NODE SETUP
const int nodeNumber = 3;   // defines the Octosensor node number

// INTERACTIVE STATE MACHINE SETUP IN cm
int interact_min = 100;
int shot_min = 20;

int dist_min = 1;
int dist_max = 400;

String statusMachine = "IDLE";   // Status: IDLE, INTERACT, SHOT
String lastStatusMachine = statusMachine;  // memory of last state

// VARIABLES FOR THE LED INDICATOR
int R_index = 1;
int G_index = 1;
int B_index = 1;

// SOUND ON/OFF
bool soundOn = true;

// ANIMATION FUNCTIONS VARIABLES

int last_ledX = 0;

//strobe variables
unsigned long strobe_millis_counter = millis();
unsigned long strobe_millis_counter_old = millis();

// fire variables
bool gReverseDirection = false;

/////////////////

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

// BUZZER ...

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
//uint8_t broadcastAddress[] = {0x30, 0xC6, 0xF7, 0x23, 0x93, 0x84}; // CENTRAL C1-25
//uint8_t broadcastAddress[] = {0xD4, 0x8A, 0xFC, 0xA6, 0xD8, 0xF4}; // CENTRAL C2-25
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // ALL PAIRED DEVICES


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
void OnDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *incomingData, int len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  Serial.print("Bytes received: ");
  Serial.println(len);
  incomingCommand = incomingReadings.comm;
  incomingNode = incomingReadings.node;
  incomingValue = incomingReadings.value;
}


 
// LED STRIP //////////////////////////////////////////////////////////////////////////////////////////////////


#define LED_TYPE    WS2811  //WS2812B
#define COLOR_ORDER GRB
#define MAX_BRIGHTNESS  128

// Panel LEDs (only 3)
#define PANEL_LED_PIN     25
#define PANEL_NUM_LEDS    3
CRGB panel_leds[PANEL_NUM_LEDS];

// Main LED Strip
#define LED_PIN     26
#define NUM_LEDS    30   /////// NUMERO DE LEDS DA FITA ////////////////////// 

CRGB leds[NUM_LEDS];



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

// Panel LEDs
  FastLED.addLeds<LED_TYPE, PANEL_LED_PIN, COLOR_ORDER>(panel_leds, PANEL_NUM_LEDS);
// Main LED Strip
  //FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setTemperature(OvercastSky);  
  FastLED.addLeds<WS2811, LED_PIN, GRB>(leds, NUM_LEDS).setTemperature(OvercastSky);  // GRB ordering is typical  /// WS2812B


  FastLED.setBrightness(MAX_BRIGHTNESS/2);

// LED STRIP TEST
  fill_solid(leds, NUM_LEDS, CRGB(255,0,0));
  fill_solid(panel_leds, PANEL_NUM_LEDS, CRGB(255,0,0));
    FastLED.show();
    delay(500); 
  fill_solid(leds, NUM_LEDS, CRGB(0,255,0));
  fill_solid(panel_leds, PANEL_NUM_LEDS, CRGB(0,255,0));
    FastLED.show();
    delay(500);
  fill_solid(leds, NUM_LEDS, CRGB(0,0,255));
  fill_solid(panel_leds, PANEL_NUM_LEDS, CRGB(0,0,255));
  FastLED.show();
    delay(500);
  fill_solid(leds, NUM_LEDS, CRGB(0,0,0));
  fill_solid(panel_leds, PANEL_NUM_LEDS, CRGB(0,0,0));
    FastLED.show();


// ANIMATION FUNCTIONS VARIABLES 

int last_ledX = 0;

//strobe variables
unsigned long strobe_millis_counter = millis();
unsigned long strobe_millis_counter_old = millis();

// fire variables
bool gReverseDirection = false;



// BUZZER...

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
   
  /*
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  */

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
  }    // beeps when status change
  lastStatusMachine = statusMachine;   // updates last Status
 }


// Interactive animations

  
  if (statusMachine == "IDLE") {
      
        fill_solid(leds, NUM_LEDS, CRGB(0, 0, 64)); //fundo azul
        ani_breath(30, 0, MAX_BRIGHTNESS);   // respira a 30 bmp, com brilho de 0 a 128
      
  }

  if (statusMachine == "INTERACT") {
 
      // convert the interactive range in cms to a number between 0 and 1023 
      // for the animation functions 
      int octoValue_to_0_1023 = map(octoValue, interact_min, shot_min, 0, 1023);
      // INTERACTIVE ANIMATION: interactive_dot_tail(in_point, out_point, position, tail_color, dot_color);
      interactive_dot_tail(0, 1023, octoValue_to_0_1023, CRGB::Blue, CRGB::White);

  }

  if (statusMachine == "SHOT") {
  
      // convert the shot range in cms to a number between 0 and 1023 
      // for the animation functions 
      int octoValue_to_0_1023 = map(octoValue, shot_min, dist_min, 0, 1023);

      // INTERACTIVE STROBE: interactive_strobe()
      interactive_strobe(40, CRGB::Red, CRGB(0,0,0));

  }




updatePanelLEDs();

FastLED.show();
  
}








// *******************************************************
//
//              F  U  N  C  T  I  O  N  S
//
// *******************************************************



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



void updatePanelLEDs() {

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

  //Serial.println (statusMachine);


  // Set monitor PIXELS
  panel_leds[1].setRGB(LEDmonitor*R_index, LEDmonitor*G_index, LEDmonitor*B_index); // COR LED DO MEIO
  panel_leds[0].setRGB(LEDmonitor*R_index, LEDmonitor*G_index, LEDmonitor*B_index); // COR LED DO LADO
  panel_leds[2].setRGB(LEDmonitor*R_index, LEDmonitor*G_index, LEDmonitor*B_index); // COR LEDO DO LADO
  FastLED.show();

}


//// -----------------------------------------------------------
////          A N I M A T I O N   F U N C T I O N S
//// -----------------------------------------------------------


/*
// (Copy, paste and adapt in the loop)

// FILL WITH A SOLID COLOR
  int sensor_to_color = map(potValue, 0, 1023, 0, 255);
  fill_solid(leds, NUM_LEDS, CRGB(255-sensor_to_color, 0, sensor_to_color));

// INTERACTIVE ANIMATION: interactive_dot(in_point, out_point, position, dot_color, dot_size);
   interactive_dot(0, 1023, potValue, CRGB::Blue, 2 );

// INTERACTIVE ANIMATION: interactive_dot_tail(in_point, out_point, position, tail_color, dot_color);
   interactive_dot_tail(0, 1023, potValue, CRGB::Purple, CRGB::Green);

// INTERACTIVE ANIMATION: interactive_meter(in_point, out_point, position, meter_color, bg_color, dot_color, 1);
   interactive_meter(0, 1023, potValue, CRGB::OrangeRed, CRGB::Red, CRGB::White, 1);

// INTERACTIVE STROBE: interactive_strobe()
  int sensor_to_millis = (map(potValue, 0, 1023, 1, 20))*25;   // 20 steps from 25ms to 500ms
  interactive_strobe(sensor_to_millis, CRGB::Red, CRGB(0,0,4));

// ANIMATION PACIFICA: ani_pacifica(bg_color)
  int sensor_to_color = map(potValue, 0, 1023, 0, 128);
  pacifica_loop(CRGB(128-sensor_to_color,0,sensor_to_color));    // CRGB(2,6,10) > original Pacifica color

// ANIMATION: ani_breath(breath_BPM, min_bright = 0, max_bright = 255)
  ani_breath(12, 0, 255);

// INTERACTIVE GLITTER
  int sensor_to_glitter = map(potValue, 0, 1023, 0, 240);
  addGlitter(sensor_to_glitter, CRGB(255,0,0));

// INTERACTIVE FIRE interactive_fire(cooling 20-150, sparking 50-200)
  //int sensor_to_cooling = map(potValue, 0, 1023, 120, 20);
  int sensor_to_sparking = map(potValue, 0, 1023, 10, 200);
  interactive_fire(60, sensor_to_sparking);

*/ 


void interactive_dot_tail(uint16_t in, uint16_t out, uint16_t position, const CRGB& tail_color, const CRGB& dot_color) {

  FastLED.setBrightness(MAX_BRIGHTNESS);

  fadeToBlackBy(leds, NUM_LEDS, 50);

  int inX = map(in, 0, 1023, 0, NUM_LEDS-1);
  int outX = map(out, 0, 1023, 0, NUM_LEDS-1);
  int ledX = map (position, 0, 1023, inX, outX);
  ledX = constrain(ledX,0,NUM_LEDS);

  //if ( ledX > last_ledX -1 && ledX < last_ledX +1)  {
    // Dot
    //leds[ledX] = CRGB::White;

  //} else {

      if ( ledX > last_ledX) {    // to the right
        for (int i=last_ledX; i <= ledX ; i++) { leds[i] = tail_color; }
      }

      if ( last_ledX > ledX) {     // to the left
        for (int i=ledX; i <= last_ledX ; i++) {leds[i] = tail_color; }
      }

      if ( last_ledX == ledX ) {
        leds[ledX] = dot_color;
      }

  //}
   // FastLED.show();

  last_ledX = ledX;


}


void interactive_dot(uint16_t in, uint16_t out, uint16_t position, const CRGB& dot_color, uint16_t dot_size) {

  FastLED.setBrightness(MAX_BRIGHTNESS);

  int inX = map(in, 0, 1023, 0, NUM_LEDS-1);
  int outX = map(out, 0, 1023, 0, NUM_LEDS-1);
  int ledX = map (position, 0, 1023, inX, outX);
  ledX = constrain(ledX,0,NUM_LEDS);
    

     for (int i = last_ledX ; i < last_ledX + dot_size; i++) {
    leds[i] = CRGB::Black; 
     }

  for (int i = ledX ; i < ledX + dot_size; i++) {
    leds[i] = dot_color; 
    }

  //FastLED.show();


  last_ledX = ledX;

}


 //Serial.print("inX:");
  //Serial.println(inX);
  //Serial.print("outX:");
  //Serial.println(outX);
  //Serial.print("----------------ledX:");
  //Serial.println(ledX);
  //Serial.print("----------------last_ledX:");
  //Serial.println(last_ledX);



void interactive_meter(int in, int out, int position, const CRGB& meter_color, const CRGB& bg_color, const CRGB& dot_color, uint16_t dot_size) {

  FastLED.setBrightness(MAX_BRIGHTNESS);


  // mapping
  int inX = map(in, 0, 1023, 0, NUM_LEDS);
  int outX = map(out, 0, 1023, 0, NUM_LEDS);
  int ledX = map (position, 0, 1023, inX, outX);
  ledX = constrain(ledX,0,1023);
  

  if (ledX != last_ledX) {
    fill_solid(leds, NUM_LEDS, bg_color);     /// RESET TO BG COLOR
    fill_solid(leds, ledX, meter_color);      /// FILLS THE BAR

 for (int i = ledX ; i < ledX + dot_size; i++) {
    leds[i] = dot_color;                      /// fills the dots
    }

    //FastLED.show();


  }


  last_ledX = ledX;


}


void ani_breath(accum88 breath_BPM, int min_bright, int max_bright) {


      int breath_bright = beatsin8(breath_BPM, min_bright, max_bright);

      FastLED.setBrightness(breath_bright);

}




void interactive_strobe(int strobe_millis, const CRGB& strobe_color, const CRGB& bg_color)  {
          
  FastLED.setBrightness(MAX_BRIGHTNESS);

          Serial.print("strobe_millis >>>>> ");
          Serial.println(strobe_millis);

          strobe_millis_counter = millis();
            
            if (strobe_millis_counter > strobe_millis_counter_old + strobe_millis ) {

                fill_solid(leds, NUM_LEDS, strobe_color);
                FastLED.show();
                delay(5);
              
                fill_solid(leds, NUM_LEDS, bg_color);

                strobe_millis_counter_old = strobe_millis_counter;


            }

  }

void addGlitter( fract8 chanceOfGlitter, const CRGB& glitter_color) {
  
    FastLED.setBrightness(MAX_BRIGHTNESS);

  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += glitter_color;
  }

}




void interactive_fire(int fire_cooling, int fire_sparking) {

    FastLED.setBrightness(MAX_BRIGHTNESS);

// Array of temperature readings at each simulation cell
  static uint8_t heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((fire_cooling * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < fire_sparking ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      CRGB color = HeatColor( heat[j]);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      leds[pixelnumber] = color;
    }

// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100 

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.

}




///// PACIFICA ANIMATION



//
// These three custom blue-green color palettes were inspired by the colors found in
// the waters off the southern coast of California, https://goo.gl/maps/QQgd97jjHesHZVxQ7
//
CRGBPalette16 pacifica_palette_1 = 
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x14554B, 0x28AA50 };
CRGBPalette16 pacifica_palette_2 = 
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x0C5F52, 0x19BE5F };
CRGBPalette16 pacifica_palette_3 = 
    { 0x000208, 0x00030E, 0x000514, 0x00061A, 0x000820, 0x000927, 0x000B2D, 0x000C33, 
      0x000E39, 0x001040, 0x001450, 0x001860, 0x001C70, 0x002080, 0x1040BF, 0x2060FF };




void pacifica_loop(const CRGB& bg_color)
{

    FastLED.setBrightness(MAX_BRIGHTNESS);

  // Increment the four "color index start" counters, one for each wave layer.
  // Each is incremented at a different speed, and the speeds vary over time.
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

  // Clear out the LED array to a dim background blue-green
  //fill_solid( leds, NUM_LEDS, CRGB( 2, 6, 10));
    fill_solid( leds, NUM_LEDS, bg_color);

  // Render each of four layers, with different scales and speeds, that vary over time
  pacifica_one_layer( pacifica_palette_1, sCIStart1, beatsin16( 3, 11 * 256, 14 * 256), beatsin8( 10, 70, 130), 0-beat16( 301) );
  pacifica_one_layer( pacifica_palette_2, sCIStart2, beatsin16( 4,  6 * 256,  9 * 256), beatsin8( 17, 40,  80), beat16( 401) );
  pacifica_one_layer( pacifica_palette_3, sCIStart3, 6 * 256, beatsin8( 9, 10,38), 0-beat16(503));
  pacifica_one_layer( pacifica_palette_3, sCIStart4, 5 * 256, beatsin8( 8, 10,28), beat16(601));

  // Add brighter 'whitecaps' where the waves lines up more
  pacifica_add_whitecaps();

  // Deepen the blues and greens a bit
  pacifica_deepen_colors();
}

// Add one layer of waves into the led array
void pacifica_one_layer( CRGBPalette16& p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff)
{
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

// Add extra 'white' to areas where the four layers of light have lined up brightly
void pacifica_add_whitecaps()
{
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

// Deepen the blues and greens
void pacifica_deepen_colors()
{
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    leds[i].blue = scale8( leds[i].blue,  145); 
    leds[i].green= scale8( leds[i].green, 200); 
    leds[i] |= CRGB( 2, 5, 7);
  }
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


