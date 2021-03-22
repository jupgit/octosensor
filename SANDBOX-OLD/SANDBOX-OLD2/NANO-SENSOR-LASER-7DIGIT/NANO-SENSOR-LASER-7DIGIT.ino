#include <Arduino.h>
#include <TM1637TinyDisplay.h>

// Module connection pins (Digital Pins)
#define CLK 4
#define DIO 5
TM1637TinyDisplay display(CLK, DIO);



#include <SoftwareSerial.h>
#include "TFMini.h"

// Setup software serial port 
SoftwareSerial MySerial(10, 11);      // Uno RX (TFMINI TX), Uno TX (TFMINI RX)
TFMini tfmini;


void setup(void) {

  Serial.begin(115200);

  MySerial.begin(TFMINI_BAUDRATE);
  
  tfmini.begin(&MySerial);  

  display.setBrightness(BRIGHT_HIGH);

  display.showString("Test");
  delay(1000);
  display.clear();


   
}

void loop(void) {

  
  uint16_t dist = tfmini.getDistance();
  uint16_t strength = tfmini.getRecentSignalStrength();

  /*
  Serial.print("Dist: ");
  Serial.print(dist);
  Serial.print(" | Str: ");
  Serial.println(strength);
  */

  Serial.println(dist);
  
   delay(5);

  display.showNumber(dist, false);          // Expect: 0000

  
}
