// **************
// SENSOR
// **************

#include <hardwareserial.h>
#include "TFMini.h"
TFMini tfmini;

uint16_t dist = 0;
uint16_t strength = 0;


void setup() {

  
  Serial.begin(115200);


// SENSOR

  Serial2.begin(115200,SERIAL_8N1, 25 ,26); //   RX / TX
  tfmini.begin(&Serial2); 



}

void loop() {
  // put your main code here, to run repeatedly:

// ***************
// SENSOR READING
  dist = tfmini.getDistance();
  strength = tfmini.getRecentSignalStrength();

  int vuMeter = map(dist, 30, 500, 4, 125);
  vuMeter = constrain(vuMeter, 4, 125);

  Serial.println(vuMeter);
  Serial.print(" | Dist: ");
  Serial.print(dist);
  Serial.print(" | Str: ");
  Serial.println(strength);


  delay(100);




}
