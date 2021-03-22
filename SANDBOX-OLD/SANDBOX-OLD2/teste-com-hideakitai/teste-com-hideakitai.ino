#include "TFmini.h"

HardwareSerial Serial1(2);

TFmini tfmini;

void setup()
{
    Serial.begin(115200);
    Serial1.begin(TFmini::DEFAULT_BAUDRATE, SERIAL_8N1, 26, 27);
    tfmini.attach(Serial1);
}

void loop()
{


     Serial.print("distance : ");
     Serial.println(tfmini.getDistance());
     Serial.print("strength : ");
     Serial.println(tfmini.getStrength());
     Serial.print("int time : ");
     Serial.println(tfmini.getIntegrationTime());
  }
