
#include <HardwareSerial.h>
HardwareSerial MySerial(2);  /// 

#include "TFMini.h"
TFMini tfmini;


void setup(void) {

  Serial.begin(115200);

  MySerial.begin(115200);
  
  tfmini.begin(&MySerial);  
   
}

void loop(void) {

  
  uint16_t dist = tfmini.getDistance();
  uint16_t strength = tfmini.getRecentSignalStrength();


  Serial.print("Dist: ");
  Serial.print(dist);
  Serial.print(" | Str: ");
  Serial.println(strength);

  delay(5);
  
  
}
