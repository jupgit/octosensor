#include <FastLED.h>

#define LED_PIN_2 26 // Pins diferentes para cada fita 
#define NUM_LEDS 100
#define LED_TYPE WS2812B
#define NUM_STRIPS 3

CRGB ledStrip[NUM_STRIPS][NUM_LEDS];
// ledStrip[2]: animAgua

// Constants and variables for animAgua
#define pointsQty NUM_LEDS / 8
int pointsArr[pointsQty];
int skip = 0;

// Constants and variables for animSangue
int redLength = 0;

void setup() {
  // Serial monitor setup
  Serial.begin(115200);

  // For animAgua
  FastLED.addLeds<NEOPIXEL, LED_PIN_2>(ledStrip[2], NUM_LEDS);
  FastLED.clear();
  fill_solid(ledStrip[2], NUM_LEDS, CRGB(3, 0, 77));
  FastLED.show();
  
  int pointsPos = 0;
  for (int i = 0; i < pointsQty; i++) {
    pointsArr[i] = pointsPos;
    pointsPos += 8;
  }

  for (int j = 0; j < pointsQty; j++) {
    ledStrip[2][pointsArr[j]] = CRGB::Cyan;
  }
  FastLED.show();
  
  // For ...



}

void animAgua() {
  skip++;
  if ((skip % 8) == 0) {
    skip = 0;
  }

  FastLED.clear();
  fill_solid(ledStrip[2], NUM_LEDS, CRGB(3, 0, 77));

  for (int i = 0; i < pointsQty; i++) {
    ledStrip[2][pointsArr[i] + skip] = CRGB::Cyan;
  }
  FastLED.show();
  delay(100);
}

void animSangue() {
  if (redLength < NUM_LEDS) {
    ledStrip[2][redLength] = CRGB::Red;
    FastLED.show();
    redLength++;
    delay(25);
  }

  else {
    skip++;
    if ((skip % 8) == 0) {
      skip = 0;
    }

    FastLED.clear();
    fill_solid(ledStrip[2], NUM_LEDS, CRGB::Red);

    for (int i = 0; i < pointsQty; i++) {
      ledStrip[2][pointsArr[i] + skip] = CRGB(138, 21, 21);
    }
    FastLED.show();
    delay(100);
  }
}

void zerar() {
  FastLED.clear();
  FastLED.show();
}

void loop() {
  // zerar();
  Serial.println(millis());
  if (millis() >= 4000 && millis() <= 16000) {
    animSangue();
  }
  else {
    animAgua();
    redLength = 0;
  }
  
}
