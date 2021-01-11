#include<SoftwareSerial.h>
SoftwareSerial mySUART(2, 3);  //SRX = Din-2, STX = Dpin-3

void setup()
{
  Serial.begin(115200);
  mySUART.begin(115200);
}

void loop()
{
  if (Serial.available() > 0)
  {
    byte x = Serial.read();
    mySUART.write(x);
  }
  if (mySUART.available() > 0)
  {
    Serial.write((char)mySUART.read());
  }
}
