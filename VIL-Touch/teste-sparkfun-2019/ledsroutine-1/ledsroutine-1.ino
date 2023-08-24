#include <FastLED.h>
#define NUM_LEDS 50
#define DATA_PIN 7
#define NUM_RUNNING_LEDS 4
CRGB leds[NUM_LEDS];
//hue 0-255, saturating 0-255, and brightness (value) 0-255
/*HSV colors: 
 * hue 0: pure green
 * hue 96: pure red
 * hue 255: pure blue
 */
void fillAllLeds(CRGB color);

void setup() {
  // put your setup code here, to run once:
   delay(3000);//sanity
   Serial.begin(9600);
   FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
   Serial.print("Start\n");
   fillAllLeds(CRGB(50,80,15));
   
   
   
}


void loop() {
  CRGB blue = CRGB(0,0,50);
  CRGB green = CRGB(100,0,100);
  CRGB red = CRGB(50, 80, 15);
  static uint16_t distance =  0;
  static int flag = 0;
  if(distance <= 301){
    distance+=5;  
    delay(200);
  }
  Serial.print(distance);
   //fillAllLeds(blue);
//  fadeTowardColors(red, green);
 // stackingStripeBackwards(green,  red,  distance);
  firstChakra(green, red, distance);
  //fillIn(red, green, distance);
  delay(200);
  
}

void fillAllLeds(CRGB color){
  for(int dot = 0; dot < NUM_LEDS; dot++) { 
    leds[dot] = color;
  }
  FastLED.show();
}

void  fillIn(CRGB transientColor, CRGB bgColor, uint16_t distance){
      Serial.print("entrando\n");
      Serial.print(distance);
      for(uint16_t i = 0; i < distance ; i++){
        leds[i] = transientColor;
      }
    for(uint16_t i = distance; i < NUM_LEDS ; i++){
        leds[i] = bgColor;  
      }
  FastLED.show();
}




void firstChakra(CRGB transientColor, CRGB bgColor, uint16_t distance){
  if(distance >= 300){
    fadeTowardColors(transientColor, bgColor);
  }
  else    fillIn(transientColor, bgColor, distance);
}



