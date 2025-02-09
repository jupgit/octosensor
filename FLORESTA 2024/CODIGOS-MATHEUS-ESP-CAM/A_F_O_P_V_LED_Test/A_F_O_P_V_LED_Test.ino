// LED Strips
#include <FastLED.h>
#define NUM_LEDS 120
#define DATA_PIN_1 8
#define MAX_BRIGHTNESS 255
CRGB leds[1][NUM_LEDS];
int mode = 0;
bool gReverseDirection = false;

//strobe variables
unsigned long strobe_millis_counter = millis();
unsigned long strobe_millis_counter_old = millis();

// Variables will change:
int ledState = LOW;   // ledState used to set the LED
int previousButtonState = LOW; // will store last time button was updated
unsigned long previousMillis = 0;   // will store last time LED was updated






void setup() {

  pinMode(2, INPUT);
  pinMode(12, OUTPUT);


  // Initializing leds
  FastLED.addLeds<WS2811, DATA_PIN_1, GRB>(leds[0], NUM_LEDS).setTemperature(OvercastSky);  // GRB ordering is typical  /// WS2812B
  FastLED.setBrightness(MAX_BRIGHTNESS);
  // Tests all 3 LED strips for R,G,B
  // COLOR RED
  fill_solid(leds[0], NUM_LEDS, CRGB(64,0,0));
  FastLED.show();
  delay(1000);

    // COLOR GREEN
  fill_solid(leds[0], NUM_LEDS, CRGB(0,64,0));
  FastLED.show();
  delay(1000);
  
    // COLOR BLUE
  fill_solid(leds[0], NUM_LEDS, CRGB(0,0,64));
  FastLED.show();
  delay(1000);


  
//strobe variables
unsigned long strobe_millis_counter = millis();
unsigned long strobe_millis_counter_old = millis();

const long BLINK_INTERVAL = 500;   // interval at which to blink LED (milliseconds)


}


void loop() {

  mode = digitalRead(2);

  if (mode == 0) {
    EVERY_N_SECONDS(10) {
      fill_solid(leds[0], NUM_LEDS, CRGB(0,0,0));
    }
    ani_breath(0, 10, 0, 64, 0); 
    
    digitalWrite(12, LOW);

  }
  else {
    //interactive_fire(0, 100, 80);
    //FastLED.setBrightness(MAX_BRIGHTNESS);

  //interactive_strobe(1000, CRGB::Red, CRGB::Black);
  blink(400);


  digitalWrite(12, HIGH);

  }
  FastLED.show();
}





void ani_breath(int LED_strip, accum88 breath_BPM, int breathR, int breathG, int breathB ) {

  int breath_bright = beatsin8(breath_BPM, 10, MAX_BRIGHTNESS);

  fill_solid(leds[LED_strip], NUM_LEDS, CRGB(breathR*breath_bright/100, breathG*breath_bright/100, breathB*breath_bright/100));
}

void interactive_fire(int LED_strip, int fire_cooling, int fire_sparking)
{
// Array of temperature readings at each simulation cell
  static uint8_t heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((fire_cooling * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < fire_sparking ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      CRGB color = HeatColor( heat[j]);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      leds[LED_strip][pixelnumber] = color;
    }

// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100 

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.

}


void interactive_strobe(int strobe_millis, const CRGB& strobe_color, const CRGB& bg_color)  {
          
  FastLED.setBrightness(MAX_BRIGHTNESS);

          Serial.print("strobe_millis >>>>> ");
          Serial.println(strobe_millis);

          strobe_millis_counter = millis();
            
            if (strobe_millis_counter > strobe_millis_counter_old + strobe_millis ) {

                fill_solid(leds[0], NUM_LEDS, strobe_color);
                FastLED.show();
                delay(strobe_millis);
              
                fill_solid(leds[0], NUM_LEDS, bg_color);
                                delay(strobe_millis);


                strobe_millis_counter_old = strobe_millis_counter;


            }

  }

  void blink(int BLINK_INTERVAL) {
  // check to see if it's time to blink the LED; that is, if the difference
  // between the current time and last time you blinked the LED is bigger than
  // the interval at which you want to blink the LED.
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= BLINK_INTERVAL) {
    // if the LED is off turn it on and vice-versa:
    ledState = (ledState == LOW) ? HIGH : LOW;

    // set the LED with the ledState of the variable:
    if (ledState == LOW ) {
      fill_solid(leds[0], 120, CRGB::Black); } 
    else {
      fill_solid(leds[0], 120, CRGB::Red);
    }
    // save the last time you blinked the LED
    previousMillis = currentMillis;
  }

 
  }

  // DO OTHER WORKS HERE

  
