//// PROBLEMA: TEM UM BUFFER COM A LEITURA DO SENSOR DEMORANDO 1 SEGUNDO 



#include <hardwareserial.h>
HardwareSerial SensorSerial(2);
////// serial(1) = pin27=RX , pin26=TX 
////// serial(2) = pin16=RX , pin17=TX 

#include "TFMini.h"
TFMini tfmini;

//#include <SPI.h>
#include <Wire.h>     // default PINS: SCL 22 / SDA 21
#include "SSD1306Wire.h"

//#include <Adafruit_GFX.h>
#//include <Adafruit_SSD1306.h>

//#define SCREEN_WIDTH 128 // OLED display width, in pixels
//#define SCREEN_HEIGHT 64 // OLED display height, in pixels

SSD1306Wire display(0x3c, SDA, SCL);   // ADDRESS, SDA, SCL  -  SDA and SCL usually populate automatically based on your board's pins_arduino.h



void setup() {
  
  Serial.begin(9600);
  

// INIT OLED DISPLAY

  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_24);
  display.setTextAlignment(TEXT_ALIGN_LEFT);

 

  // INIT SENSOR
  SensorSerial.begin(115200);
  tfmini.begin(&SensorSerial);  


}


void loop() {


  uint16_t dist = tfmini.getDistance();
  //uint16_t strength = tfmini.getRecentSignalStrength(); 

  int vuMeter = map(dist, 30, 5000, 4, 125);
  vuMeter = constrain(vuMeter, 4, 125);

  Serial.println(dist);

  display.clear();
  //display.println(dist);
  display.drawString(0, 40, String(dist));

  display.display();
  
  delay(25);

 
}
