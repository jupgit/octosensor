// ESP32 Touch Test
// Just test touch pin - Touch0 is T0 which is on GPIO 4.

void setup()
{
  Serial.begin(115200);
  delay(1000); // give me time to bring up serial monitor
  Serial.println("ESP32 Touch Test");
  pinMode(26, OUTPUT);
  digitalWrite(26, LOW);
}

void loop()
{

  int leitura4 = touchRead(T4);
  if (leitura4 <= 40)
    leitura4 = 1;
  else
    leitura4 = 0;

  int leitura5 = touchRead(T5);
  if (leitura5 <= 40)
    leitura5 = 1;
  else
    leitura5 = 0;

  int leitura6 = touchRead(T6);
  
  if (leitura6 <= 40)
    leitura6 = 1;
  else
    leitura6 = 0;

  //ISADORA  
  Serial.print(1,DEC); // Canal 1 do Isadora
  Serial.print(leitura4); // Envia sensor 1
  Serial.println(); //Send a value to eom

  Serial.print(2,DEC); // Canal 2 do Isadora
  Serial.print(leitura5); // Envia sensor 2
  Serial.println(); //Send a value to eom

  Serial.print(3,DEC); // Canal 2 do Isadora
  Serial.print(leitura6); // Envia sensor 2
  Serial.println(); //Send a value to eom

  delay(100);

}
