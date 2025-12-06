#include <FastLED.h>

#define LED_PIN     26          // Data pin connected to the LED strip
#define NUM_LEDS    4          // Number of RGB pixels
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define BRIGHTNESS  100

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  // RED
  fill_solid(leds, NUM_LEDS, CRGB::Red);
  FastLED.show();
  delay(2000);

  // GREEN
  fill_solid(leds, NUM_LEDS, CRGB::Green);
  FastLED.show();
  delay(2000);

  // BLUE
  fill_solid(leds, NUM_LEDS, CRGB::Blue);
  FastLED.show();
  delay(2000);
}
