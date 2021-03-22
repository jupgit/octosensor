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


// THIS SKETCH IS JUST AN *OFFLINE* HARDWARE TEST THAT WILL:
//
// 1 - Turn on OLED Display
// 2 - Buzz a sound
// 3 - Turn on 3 Monitor LEDs
// 4 - Flash UV --- DEPRECATED
// 5 - Prompt for button tests
// 6 - LED strip output
// 7 - Sensor reading
//


// test
int led_monitor = 0;



// tfmini sensor ///////////////////////////////////////////////////////////////////////////////////////////////

#include <HardwareSerial.h>
HardwareSerial MySerial(2);  /// ***TROCAR PELO CODIGO NATIVO DO ESP32

#include "TFMini.h"
TFMini tfmini;


// ESP NOW ////////////////////////////////////////////////////////////////////////////////////////////////////

#include <esp_now.h>
#include <WiFi.h>

// OLED DISPLAY ///////////////////////////////////////////////////////////////////////////////////////////////

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
// REPLACE WITH THE MAC Address of your receiver (CENTRAL)
uint8_t broadcastAddress[] = {0xC4, 0x4F, 0x33, 0x3E, 0xE7, 0x6D};
// ************************************************************

  
// LED STRIP //////////////////////////////////////////////////////////////////////////////////////////////////

#include <FastLED.h>
#define LED_PIN     26
#define NUM_LEDS    30   /////// NUMERO DE LEDS DA FITA ////////////////////// 
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define BRIGHTNESS  25
// We're using two LED strips, one for the monitor, the for the show
// (see File>Examples>FASTLED>ArrayOfLedArrays)
#define NUM_STRIPS 2
//CRGB leds[NUM_LEDS];
CRGB LEDstrip[NUM_STRIPS][NUM_LEDS];



// BUZZER ////////////////////////////////////////////////////////////////////////////////////////////////////

#include <CuteBuzzerSounds.h>
#define BUZZER_PIN 15



// BLINK CONTROL
#include <BlinkControl.h>
BlinkControl led_UV(32);    // LED UV



// PUSH BUTTONS ////////////////////////////////////////////////////////////////////////////////////////////////////

boolean bot_ENTER = 0 ;
boolean bot_UP = 0 ;
boolean bot_DOWN = 0 ;

int bot_ENTER_PIN = 33;
int bot_UP_PIN = 34;
int bot_DOWN_PIN = 35;





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

  display.display();
  delay(500); // Pause for 2 seconds


  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("  Hardware Check");
  display.display();

 

// LED STRIP
  //LEDS.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, 25>(LEDstrip[0], 3);  // 3 LEDS FOR MONITOR
  FastLED.addLeds<NEOPIXEL, 26>(LEDstrip[1], NUM_LEDS);

    
    FastLED.setBrightness(BRIGHTNESS);

// buzzer
  cute.init(BUZZER_PIN);

  cute.play(S_FART3);
  delay(500);

// PUSH BUTTONS
pinMode (bot_ENTER_PIN, INPUT);
pinMode (bot_UP_PIN, INPUT);
pinMode (bot_DOWN_PIN, INPUT);


// UV LED Blink
//  led_UV.begin();
//  led_UV.blink2();
  
}



// *******************************************************
//
//                    L  O   O   P
//
// *******************************************************

void loop() {
  
   cute.play(S_CONNECTION);

  // OLED
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0, 20);
  display.print("OLED");
  display.display();
  
  delay(2000);

  // OLED Msg
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0, 20);
  display.print("Monitor");
  display.display();

  cute.play(S_CONNECTION);

    for(int i = 0; i < 3; i++) {
      LEDstrip[0][i] = CRGB::Red;
      FastLED.show();
      delay(100);
    }
    for(int i = 0; i < 3; i++) {
      LEDstrip[0][i] = CRGB::Green;
      FastLED.show();
      delay(100);
    }
    for(int i = 0; i < 3; i++) {
      LEDstrip[0][i] = CRGB::Blue;
      FastLED.show();
      delay(100);
    }
    for(int i = 0; i < 3; i++) {
      LEDstrip[0][i] = CRGB::Black;
      FastLED.show();
      delay(100);
    }



/// LED STRIP 2 - SHOW

  // OLED Msg
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0, 20);
  display.println("LED");
  display.print("Strip");
  display.display();

  cute.play(S_CONNECTION);


    for(int i = 0; i < NUM_LEDS; i++) {
      LEDstrip[1][i] = CRGB::Red;
      FastLED.show();
      delay(10);
    }
    for(int i = 0; i < NUM_LEDS; i++) {
      LEDstrip[1][i] = CRGB::Green;
      FastLED.show();
      delay(10);
    }
    for(int i = 0; i < NUM_LEDS; i++) {
      LEDstrip[1][i] = CRGB::Blue;
      FastLED.show();
      delay(10);
    }

    for(int i = 0; i < NUM_LEDS; i++) {
      LEDstrip[1][i] = CRGB::White;
      FastLED.show();
      delay(10);
    }
    for(int i = 0; i < NUM_LEDS; i++) {
      LEDstrip[1][i] = CRGB::Black;
      FastLED.show();
      delay(10);
    }
  


  // OLED Msg
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0, 20);
  display.println("Press");
  display.print("BUTTONS");
  display.display();

  cute.play(S_CONNECTION);


  while (bot_UP==0 || bot_DOWN == 0 || bot_ENTER == 0) {

    if (digitalRead(bot_UP_PIN)) { bot_UP = 1; cute.play(S_BUTTON_PUSHED); 
 }
    if (digitalRead(bot_DOWN_PIN)) { bot_DOWN = 1; cute.play(S_BUTTON_PUSHED); 
 }
    if (digitalRead(bot_ENTER_PIN)) { bot_ENTER = 1; cute.play(S_BUTTON_PUSHED); 
 }
  }


  bot_UP = 0;
  bot_DOWN = 0;
  bot_ENTER = 0;

  
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

  
}
