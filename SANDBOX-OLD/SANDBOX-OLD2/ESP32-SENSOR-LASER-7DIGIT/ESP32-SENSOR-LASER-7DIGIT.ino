#include <TFmini.h>

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

//TFMini.TriggerSource(0)


void setup(void) {

  Serial.begin(115200);

  MySerial.begin(115200);
  
  tfmini.begin(&MySerial);  

  display.setBrightness(BRIGHT_HIGH);

  

   
}

void loop(void) {

  tfmini.externalTrigger();
  uint16_t dist = tfmini.getDistance();
  uint16_t strength = tfmini.getRecentSignalStrength();

  // Take one TF Mini distance measurement
  Serial.print("Dist: ");
  Serial.print(dist);
  Serial.print(" | Str: ");
  Serial.println(strength);

   delay(25);

  //display.showString("Test");

  display.showNumber(dist, false);          // Expect: 0000

  
}
