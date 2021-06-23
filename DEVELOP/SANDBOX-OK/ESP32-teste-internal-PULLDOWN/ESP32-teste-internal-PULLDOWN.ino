void setup() {
  // put your setup code here, to run once:


  pinMode(32, INPUT_PULLDOWN);
  pinMode(33, INPUT_PULLDOWN);
  pinMode(19, INPUT_PULLDOWN);


  Serial.begin(9600);


}

void loop() {


  if (digitalRead(32)) {
    Serial.print("1");
  }


  if (digitalRead(33)) {
    Serial.print("2");
  }



   if (digitalRead(19)) {
    Serial.print("3");
  }


  delay(50);

    Serial.println(".");




}
