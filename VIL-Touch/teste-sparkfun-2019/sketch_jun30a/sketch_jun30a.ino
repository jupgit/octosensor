#include <Tsunami.h>
#include <SoftwareSerial.h>


SoftwareSerial mySerial(19, 18); // RX, TX
Tsunami tsunami;

void setup() {
  // put your setup code here, to run once:
  //Serial.begin(57600);
  //while (!Serial) {
  //  ; // wait for serial port to connect. Needed for native USB port only
  //}

   mySerial.begin(9600);
   tsunami.start();

}

void loop() {
  // put your main code here, to run repeatedly:
  
  tsunami.trackPlaySolo(1, 1, false);
  delay(3000);
}
