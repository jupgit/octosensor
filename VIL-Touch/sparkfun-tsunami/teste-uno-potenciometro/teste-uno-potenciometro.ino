void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  
  // read the input on analog pin 0:
  int sensorValue1 = analogRead(A0);
  // print out the value you read:
  Serial.print("(1)");
  Serial.print(sensorValue1);

  int sensorValue2 = analogRead(A1);
  // print out the value you read:
    Serial.print("  (2)");
  Serial.print(sensorValue2);

    int sensorValue3 = analogRead(A2);
  // print out the value you read:
    Serial.print("  (3)");
  Serial.print(sensorValue3);

    int sensorValue4 = analogRead(A3);
  // print out the value you read:
    Serial.print("  (4)");
  Serial.print(sensorValue4);

    int sensorValue5 = analogRead(A4);
  // print out the value you read:
   Serial.print("  (5)");
 Serial.print(sensorValue5);

    int sensorValue6 = analogRead(A5);
  // print out the value you read:
  Serial.print("  (6)");
  Serial.print(sensorValue6);

  Serial.println("*");



  
  delay(100);        // delay in between reads for stability

}
