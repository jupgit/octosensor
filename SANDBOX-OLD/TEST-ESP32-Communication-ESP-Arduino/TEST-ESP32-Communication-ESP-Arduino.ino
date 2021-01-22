#define RXD2 16
#define TXD2 17

void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2);
}

void loop()
{

  /*
  if(Serial.available()>0)
  {
    byte x = Serial.read();
    Serial1.write(x);
  }
  */
  if(Serial1.available()>0)
  {
    Serial.write((char)Serial1.read());
  }
}
