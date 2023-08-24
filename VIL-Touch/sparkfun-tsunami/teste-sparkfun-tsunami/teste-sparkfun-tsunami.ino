#include <Tsunami.h>
#include <SoftwareSerial.h>


  int delayt = 500;


SoftwareSerial mySerial(19, 18); // RX, TX
Tsunami tsunami;

void setup() {

    pinMode(9, OUTPUT);

  // put your setup code here, to run once:
  Serial.begin(57600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

   mySerial.begin(57600);
   tsunami.start();


}

void loop() {
  // put your main code here, to run repeatedly:


  Serial.print(delayt);
  digitalWrite(10, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(10);                       // wait for a second
  digitalWrite(10, HIGH);    // turn the LED off by making the voltage LOW
  delay(delayt);
  delayt = delayt - 50 ;

  //tsunami.trackPlayPoly(101, 0, true);     // Start Track 1
  //delay(5000);





  
}
