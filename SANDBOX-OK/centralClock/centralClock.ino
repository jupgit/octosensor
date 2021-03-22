
uint8_t centralClock_BPM = 200;

long totalBeats;
long last_totalBeats;

unsigned int centralClock_Bar;
unsigned int last_Bar;




uint8_t centralClock_16th;
uint8_t last_16th;
int beat_in_ms;

unsigned long lastMilli;
unsigned long currentMilli;


void setup() {

Serial.begin(9600);



}

void loop() {


update_bar_16th();

  if (last_Bar != centralClock_Bar) {

    Serial.println();
    Serial.print(currentMilli);
    Serial.print(" | Bar:");
    Serial.println(centralClock_Bar);

    last_Bar = centralClock_Bar;
  
  } 

  if (last_totalBeats != totalBeats) {
      
      Serial.print("|");
      last_totalBeats = totalBeats;
    
    } 
    
  if (last_16th != centralClock_16th) {
      
      Serial.print("*");
      last_16th = centralClock_16th;
    
    } 


delay(25);


}





















/// FUNCTIONS

void update_bar_16th() {


  currentMilli = millis();
  beat_in_ms = 60000 / centralClock_BPM;

  totalBeats = (currentMilli/60000.0)*centralClock_BPM;
  centralClock_Bar = totalBeats/4;
  centralClock_16th = (currentMilli - ((int)totalBeats*beat_in_ms))/(beat_in_ms/4);

}
