const int sensorPin = 32;
const int airValue = 3620;
const int waterValue = 1680;
int soilMoistureValue = 0;
int soilMoisturePercent = 0;

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);


}

void loop() {

  soilMoistureValue = analogRead(sensorPin);

  //Serial.print("Sensor: ");
  Serial.println(soilMoistureValue);
  //Serial.print("  - ");
  //Serial.print(soilMoisturePercent);
  //Serial.println(" %");

  delay(1000);


}
