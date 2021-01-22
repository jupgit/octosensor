/// TO-DO
// SMOOTHER
// FAZER FUNÇÃO DE LER SENSOR RETORNAR UM VALOR




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

uint16_t dist = 0;
uint16_t lastDist = 0;


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

  
  dist = tfmini.getDistance();
  //uint16_t strength = tfmini.getRecentSignalStrength();

  if (dist != lastDist) {
  Serial.println(dist);
  display.showNumber(dist, false);          // Expect: 0000

  lastDist = dist;
  
  }


  /*
  Serial.print("Dist: ");
  Serial.print(dist);
  Serial.print(" | Str: ");
  Serial.println(strength);
  */

  
   delay(10);


  
}
