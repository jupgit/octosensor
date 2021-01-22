#include<SoftwareSerial.h>
#include "TFMini.h"   // SENSOR LASER



SoftwareSerial mySUART(2, 3);  //SRX = Din-2, STX = Dpin-3

SoftwareSerial mySerial(10, 11);      // Uno RX (TFMINI TX), Uno TX (TFMINI RX)

TFMini tfmini;

int pot=0;


void setup()
{
  Serial.begin(9600);
  mySUART.begin(9600);

  // INICIALIZA O SEGUNDO SERIAL (Softserial) para a leitura do sensor
  mySerial.begin(TFMINI_BAUDRATE);

  // Initialize the TF Mini sensor
  tfmini.begin(&mySerial);  


  pinMode(A0, INPUT);
  
}

void loop()
{
  


  // Take one TF Mini distance measurement
  uint16_t dist = tfmini.getDistance();
  uint16_t strength = tfmini.getRecentSignalStrength();

  // DEBUG : Display the measurement
  Serial.print(dist);
  Serial.print(" cm      sigstr: ");
  Serial.println(strength);

   
   delay(25);



    pot = analogRead(A0);
    mySUART.println(dist);
    //Serial.println(pot);

    delay(25);

 /* 
  if (mySUART.available() > 0)
  {
    Serial.write((char)mySUART.read());
  }
  */
  
}
