// ****************************************************************************
//      V I L    M E T A L
//      Art Instalattion
//      For the ESP32 DevKit
//      Uses: Capacitive Touch, 2 LED Strips, Tsunami Wave Trigger
//
//     Comments: 
//  Programmers: Juliano Prado, Matheus 
// ****************************************************************************



// ESP32 - Serial 2 pin config
// Used to trigger Tsunami by serial
#define RXD2 16
#define TXD2 17


//// ************************************************************************
////      S P A R K F U N    T S U N A M I    W A V E    T R I G G E R
//// 
//// ************************************************************************

// Tsunami Lib
#include <Tsunami.h>            // Include the Tsunami library header
Tsunami tsunami;                // Our Tsunami object

// !!!! IMPORTANT: MODIFY Tsunami.h file to use Serial 2 on ESP 32!!!!!
//
// This is the TSUNAMI SERIAL LIBRARY:  https://github.com/robertsonics/Tsunami-Arduino-Serial-Library
// ==================================================================
// The following defines are used to control which serial class is
//  used. Uncomment only the one you wish to use. If all of them are
//  commented out, the library will use Hardware Serial
//#define __TSUNAMI_USE_SERIAL1__
//#define __TSUNAMI_USE_SERIAL2__    <<<<<<<<<<--------  UNCOMMENT THIS ON Tsunami.h LIB!!!!!!!!!!
//#define __TSUNAMI_USE_SERIAL3__
//#define __TSUNAMI_USE_ALTSOFTSERIAL__
// ==================================================================



//// LED STRIP
#include <FastLED.h>

// THIS PROJECT USES 3 LED STRIPS OUPUTS
// How max many leds in your strips?
#define NUM_LEDS 30
// set pins for the LED strips
#define DATA_PIN_1 25
#define DATA_PIN_2 26
#define DATA_PIN_3 18

#define MAX_BRIGHTNESS 255

// Define the array of leds
CRGB leds[3][NUM_LEDS];

// ANIMATION FUNCTIONS VARIABLES

int last_ledX = 0;

//strobe variables
unsigned long strobe_millis_counter = millis();
unsigned long strobe_millis_counter_old = millis();

// fire variables
bool gReverseDirection = false;


//// 9 TOUCH SENSORS -------------------------------------

// That´s the sequence they are phisically hooked
// from the board to the electrical touch terminals
// ESP32 PINS
int touchPin[]={15,2,4,32,33,27,14,12};
// Array com as leituras dos sensores
int touchValue[]={0,0,0,0,0,0,0,0};
// Array com booleanos dos sensores
boolean touchOnOff[]={0,0,0,0,0,0,0,0};
// The threshold value to be considered a touch on/off
int touchThreshold = 25;
// Number of sensors to read (max 9 in this configuration)
int touchSensors = 8;
// Zero if no sensor is touched
int touchOR = 0;




// ****************************************************************************


void setup() {
  Serial.begin(115200);
  delay(1000); // give me time to bring up serial monitor
  Serial.println("ESP32 Touch Test");
  pinMode(26, OUTPUT);
  digitalWrite(26, LOW);

  // FASTLED
  FastLED.addLeds<WS2811, DATA_PIN_1, GRB>(leds[0], NUM_LEDS).setTemperature(OvercastSky);  // GRB ordering is typical  /// WS2812B
  FastLED.addLeds<WS2811, DATA_PIN_2, GRB>(leds[1], NUM_LEDS).setTemperature(OvercastSky);  // GRB ordering is typical  /// WS2812B
  FastLED.addLeds<WS2811, DATA_PIN_3, GRB>(leds[2], NUM_LEDS).setTemperature(OvercastSky);  // GRB ordering is typical  /// WS2812B

for (int i = 0; i<3; i++) {

    FastLED.setBrightness(255);
    // LED RGB TEST
    fill_solid(leds[i], NUM_LEDS, CRGB(255,0,0));
    FastLED.show();
    delay(50); 
    fill_solid(leds[i], NUM_LEDS, CRGB(0,255,0));
    FastLED.show();
    delay(50);
    fill_solid(leds[i], NUM_LEDS, CRGB(0,0,255));
    FastLED.show();
    delay(50);
    fill_solid(leds[i], NUM_LEDS, CRGB(0,0,0));
    FastLED.show();

    delay(50);

  }

// TSUNAMI ( AUDIO )
 
  // We should wait for the Tsunami to finish reset before trying to send
  // commands.
  delay(1000);

  // Tsunami startup at 57600
  tsunami.start();
  delay(10);
  
  // Send a stop-all command and reset the sample-rate offset, in case we have
  //  reset while the Tsunami was already playing.
  tsunami.stopAllTracks();
  tsunami.samplerateOffset(0, 0);
  
  // Enable track reporting from the Tsunami
  tsunami.setReporting(true);
  
  // Allow time for the Tsunami to respond with the version string and
  //  number of tracks.
  delay(100); 

// ZERAR OS GANHOS
// tsunami.trackGain(101, 0);             // Preset Track 101 gain to -40dB
// tsunami.trackGain(102, -40);             // Preset Track 101 gain to -40dB
// tsunami.trackGain(103, -40);             // Preset Track 101 gain to -40dB
// tsunami.trackGain(104, -40);             // Preset Track 101 gain to -40dB

// TSUNAMI TEST
for (int t = 1; t<=16 ; t++) {

  tsunami.trackPlayPoly(t, 0, true);     // Start Track 1
  //tsunami.trackFade(track, 0, 50, false);  // Fade IN Track 1 up to 0db over 50 msecs

 delay(200);
}


}  /// END SETUP





















// ****************************************************************************

void loop() {

// TOUCH SENSORS
read_touch_sensors(touchSensors);

    // check if any touch is touched
    touchOR = 0;
    for (int s = 0; s < touchSensors; s++) { 
      if (touchOnOff[s]) touchOR++;
    }


// LED animations

if (touchOR) {

    /// STRIP 1 >> Fire: (LED_strip_number 0-2,cooling 20-150, sparking 50-200)
    interactive_fire(0, 20, 10); 
    /// STRIP 2 >>  INTERACTIVE STROBE: interactive_strobe() -- from 25ms to 500ms
    interactive_strobe(1, 100, CRGB(64,0,0), CRGB(0,0,0));
    //fill_solid(leds[1], NUM_LEDS, CRGB(32,8,0));     

       } else {


       /// STRIP 1 >> 
    pacifica_loop(0, CRGB(2,6,10));  

     /// STRIP 2 >> 
    //fill_solid(leds[1], NUM_LEDS, CRGB(4,0,32));     /// STRIP 1 >> 
    ani_breath(1, 12, 0, 24, 2);                     /// STRIP 2 >> 

    }


  FastLED.show();


// AUDIO TRIGGERING
  for (int s = 0; s < touchSensors; s++) { 
    if (touchOnOff[s])  

    if  (tsunami.isTrackPlaying(s+1) == false)     // if the track is NOT playing...
          tsunami.trackPlayPoly(s+1, 0, true);     // Start respective Track when touch sensor is ON
  }


displayStatusOnSerial();

//isadoraOutput();
//
delay(10);


} /// END LOOP





























// ****************************************************************************
//                        F  U  N  C  T  I  O  N  S 
// ****************************************************************************


// TOUCH SENSOR READING FUNCTION

void read_touch_sensors(int num_of_sensors) {

  // Populates the touchValue array with touch readings from each pin
  for (int i=0; i < num_of_sensors; i++) {
    touchValue[i] = touchRead(touchPin[i]);
    // converts the touch to boolean by comparing to the threshold value
    if (touchValue[i] <= touchThreshold) touchOnOff[i] = 1; else touchOnOff[i] = 0;

  }

}


void displayStatusOnSerial() {

  for (int s=0; s < touchSensors; s++){

    Serial.print("| ");
        if (touchOnOff[s]) Serial.print("ON :"); else Serial.print("OFF:");

    if (touchValue[s]) Serial.print(touchValue[s]); else Serial.print("-");
    Serial.print(" ");
  } 
    Serial.println("|");

   Serial.println(touchOR);


}





void isadoraOutput() {

  for (int s=1; s <= touchSensors; s++){
    Serial.print(s,DEC); // Canal s do Isadora
    Serial.print(touchValue[s-1]); // Envia sensor 
    Serial.println(); //Send a value to eom
  } 

}

///// PACIFICA ANIMATION



//
// These three custom blue-green color palettes were inspired by the colors found in
// the waters off the southern coast of California, https://goo.gl/maps/QQgd97jjHesHZVxQ7
//
CRGBPalette16 pacifica_palette_1 = 
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x14554B, 0x28AA50 };
CRGBPalette16 pacifica_palette_2 = 
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x0C5F52, 0x19BE5F };
CRGBPalette16 pacifica_palette_3 = 
    { 0x000208, 0x00030E, 0x000514, 0x00061A, 0x000820, 0x000927, 0x000B2D, 0x000C33, 
      0x000E39, 0x001040, 0x001450, 0x001860, 0x001C70, 0x002080, 0x1040BF, 0x2060FF };




void pacifica_loop(int LED_strip, const CRGB& bg_color)
{
  // Increment the four "color index start" counters, one for each wave layer.
  // Each is incremented at a different speed, and the speeds vary over time.
  static uint16_t sCIStart1, sCIStart2, sCIStart3, sCIStart4;
  static uint32_t sLastms = 0;
  uint32_t ms = GET_MILLIS();
  uint32_t deltams = ms - sLastms;
  sLastms = ms;
  uint16_t speedfactor1 = beatsin16(3, 179, 269);
  uint16_t speedfactor2 = beatsin16(4, 179, 269);
  uint32_t deltams1 = (deltams * speedfactor1) / 256;
  uint32_t deltams2 = (deltams * speedfactor2) / 256;
  uint32_t deltams21 = (deltams1 + deltams2) / 2;
  sCIStart1 += (deltams1 * beatsin88(1011,10,13));
  sCIStart2 -= (deltams21 * beatsin88(777,8,11));
  sCIStart3 -= (deltams1 * beatsin88(501,5,7));
  sCIStart4 -= (deltams2 * beatsin88(257,4,6));

  // Clear out the LED array to a dim background blue-green
  //fill_solid( leds[LED_strip], NUM_LEDS, CRGB( 2, 6, 10));
    fill_solid( leds[LED_strip], NUM_LEDS, bg_color);

  // Render each of four layers, with different scales and speeds, that vary over time
  pacifica_one_layer(LED_strip, pacifica_palette_1, sCIStart1, beatsin16( 3, 11 * 256, 14 * 256), beatsin8( 10, 70, 130), 0-beat16( 301) );
  pacifica_one_layer(LED_strip, pacifica_palette_2, sCIStart2, beatsin16( 4,  6 * 256,  9 * 256), beatsin8( 17, 40,  80), beat16( 401) );
  pacifica_one_layer(LED_strip, pacifica_palette_3, sCIStart3, 6 * 256, beatsin8( 9, 10,38), 0-beat16(503));
  pacifica_one_layer(LED_strip, pacifica_palette_3, sCIStart4, 5 * 256, beatsin8( 8, 10,28), beat16(601));

  // Add brighter 'whitecaps' where the waves lines up more
  pacifica_add_whitecaps(LED_strip);

  // Deepen the blues and greens a bit
  pacifica_deepen_colors(LED_strip);
}

// Add one layer of waves into the led array
void pacifica_one_layer(int LED_strip, CRGBPalette16& p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff)
{
  uint16_t ci = cistart;
  uint16_t waveangle = ioff;
  uint16_t wavescale_half = (wavescale / 2) + 20;
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    waveangle += 250;
    uint16_t s16 = sin16( waveangle ) + 32768;
    uint16_t cs = scale16( s16 , wavescale_half ) + wavescale_half;
    ci += cs;
    uint16_t sindex16 = sin16( ci) + 32768;
    uint8_t sindex8 = scale16( sindex16, 240);
    CRGB c = ColorFromPalette( p, sindex8, bri, LINEARBLEND);
    leds[LED_strip][i] += c;
  }
}

// Add extra 'white' to areas where the four layers of light have lined up brightly
void pacifica_add_whitecaps(int LED_strip)
{
  uint8_t basethreshold = beatsin8( 9, 55, 65);
  uint8_t wave = beat8( 7 );
  
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    uint8_t threshold = scale8( sin8( wave), 20) + basethreshold;
    wave += 7;
    uint8_t l = leds[LED_strip][i].getAverageLight();
    if( l > threshold) {
      uint8_t overage = l - threshold;
      uint8_t overage2 = qadd8( overage, overage);
      leds[LED_strip][i] += CRGB( overage, overage2, qadd8( overage2, overage2));
    }
  }
}

// Deepen the blues and greens
void pacifica_deepen_colors(int LED_strip)
{
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    leds[LED_strip][i].blue = scale8( leds[LED_strip][i].blue,  145); 
    leds[LED_strip][i].green= scale8( leds[LED_strip][i].green, 200); 
    leds[LED_strip][i] |= CRGB( 2, 5, 7);
  }
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



void ani_breath(int LED_strip, accum88 breath_BPM, int breathR, int breathG, int breathB ) {


      int breath_bright = beatsin8(breath_BPM, 0, 100);

  fill_solid(leds[LED_strip], NUM_LEDS, CRGB(breathR*breath_bright/100, breathG*breath_bright/100, breathB*breath_bright/100));
      //leds[LED_strip].setBrightness(breath_bright);

}




void interactive_strobe(int LED_strip, int strobe_millis, const CRGB& strobe_color, const CRGB& bg_color)  {
          

          FastLED.setBrightness(MAX_BRIGHTNESS);

          //Serial.print("strobe_millis >>>>> ");
          //.println(strobe_millis);

          strobe_millis_counter = millis();
            
            if (strobe_millis_counter > strobe_millis_counter_old + strobe_millis ) {

                fill_solid(leds[LED_strip], NUM_LEDS, strobe_color);
                FastLED.show();
                delay(10);
              
                fill_solid(leds[LED_strip], NUM_LEDS, bg_color);

                strobe_millis_counter_old = strobe_millis_counter;


            }

  }

void addGlitter(int LED_strip,  fract8 chanceOfGlitter, const CRGB& glitter_color) {
  
    FastLED.setBrightness(MAX_BRIGHTNESS);

  if( random8() < chanceOfGlitter) {
    leds[LED_strip][ random16(NUM_LEDS) ] += glitter_color;
  }

}
