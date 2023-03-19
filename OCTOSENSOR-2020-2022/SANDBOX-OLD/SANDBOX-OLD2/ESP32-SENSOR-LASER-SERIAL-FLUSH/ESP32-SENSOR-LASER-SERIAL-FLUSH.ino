//#include <Arduino.h>
#include <TM1637TinyDisplay.h>

// Module connection pins (Digital Pins)
#define CLK 4
#define DIO 5
TM1637TinyDisplay display(CLK, DIO);



#include <HardwareSerial.h>
HardwareSerial MySerial(2);  /// 

#include "TFMini.h"
TFMini tfmini;


void setup(void) {

  Serial.begin(115200);

  MySerial.begin(115200);
  
  tfmini.begin(&MySerial);  

  display.setBrightness(BRIGHT_HIGH);

  

   
}

void loop(void) {

  uint16_t dist = tfmini.getDistance();
  uint16_t strength = tfmini.getRecentSignalStrength();

   
   while(MySerial.read() >= 0) ; // flush the received buffer
   delay(10);


  // Take one TF Mini distance measurement
  Serial.print("Dist: ");
  Serial.print(dist);
  Serial.print(" | Str: ");
  Serial.println(strength);


  //display.showString("Test");

  display.showNumber(dist, false);          // Expect: 0000

  
}
