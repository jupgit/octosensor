#define soundpin 15

void setup() {

  Serial.begin(115200);
  delay(6000);
  Serial.println("Buzzer Start");

  ledcAttach(soundpin,NOTE_A,7); //ledcAttach(uint8_tpin,uint32_tfreq,uint8_tresolution); for ESP core V3
  
  delay(115); 
  ledcWriteNote(soundpin,NOTE_A, 7);    // PIN, frequency, octave for ESP core V3
  delay(900);
  
  ledcWrite(soundpin,0); // No sound 
  ledcDetach(soundpin);  
  Serial.println("Buzzer Stop");
}

void loop() {

}