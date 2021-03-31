/* 
TESTE DE SENSOR TFMini + SERIAL

>>>> não funcionou com 2 seriais...
Testar i2c ou outro

 */

#include <SoftwareSerial.h>
#include "TFMini.h"   // SENSOR LASER

// Setup software serial port 
SoftwareSerial mySerial(10, 11);      // Uno RX (TFMINI TX), Uno TX (TFMINI RX)
TFMini tfmini;



void setup() {

// INICIALIZA O MONITOR SERIAL
Serial.begin(9600);
Serial.println ("Initializing...");

// INICIALIZA O SEGUNDO SERIAL (Softserial) para a leitura do sensor
mySerial.begin(TFMINI_BAUDRATE);

// Initialize the TF Mini sensor
tfmini.begin(&mySerial);   




}

void loop() {
 
  // Take one TF Mini distance measurement
  uint16_t dist = tfmini.getDistance();
  uint16_t strength = tfmini.getRecentSignalStrength();

  // DEBUG : Display the measurement
  Serial.print(dist);
  Serial.print(" cm      sigstr: ");
  Serial.println(strength);

   
   delay(25);

}
