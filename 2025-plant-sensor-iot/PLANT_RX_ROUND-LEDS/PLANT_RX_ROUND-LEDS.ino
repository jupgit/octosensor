/// TESTING 1.28" TFT ROUND DISPLAY


/*
IMPORTANT: ALTERATION ON LIBRARY user_Setup.h FILE
Only ONE driver uncommented on the file Arduino/libraries/TFT_eSPI/User_SetUp.h
// Only define one driver, the other ones must be commented out
#define GC9A01_DRIVER    /// <<< THAT'S THE CURRENT DISPLAY CHIP


ALSO this session was altered:
======
// For ESP32 Dev board (only tested with GC9A01 display)
// The hardware SPI can be mapped to any pins

#define TFT_MOSI 23 // In some display driver board, it might be written as "SDA" and so on.
#define TFT_SCLK 18
#define TFT_CS   22 // Chip select control pin
#define TFT_DC   16  // Data Command control pin
#define TFT_RST  4  // Reset pin (could connect to Arduino RESET pin)
//#define TFT_BL   22  // LED back-light
======


*/

#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library


// Display settings
const int SCREEN_WIDTH = 240;
const int SCREEN_HEIGHT = 280;
const int CENTER_X = SCREEN_WIDTH / 2;
const int CENTER_Y = SCREEN_HEIGHT / 2;
const int RADIUS = 30;  // moisture circles radius

/// 4 LEDs 
#include <FastLED.h>

// FastLED setup
#define LED_PIN     26          // Choose an appropriate pin
#define NUM_LEDS    4
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define BRIGHTNESS  96

CRGB leds[NUM_LEDS];


// Moisture Circles Positions
int posX[4];
int posY[4];

// Simulated or actual moisture readings (0–1023)
int moisture1, moisture2, moisture3, moisture4;







void setup() {


  // FastLED initialization
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);


  tft.init();
    tft.setRotation(0);


  tft.fillScreen(TFT_BLACK);

    delay(500);

  tft.drawRect(60, 60, tft.width()/2, tft.height()/2, TFT_PURPLE);

    // Set "cursor" at top left corner of display (0,0) and select font 4

  // We can now plot text on screen using the "print" class
  tft.setTextColor(TFT_GREENYELLOW);
  tft.setCursor(65, 100, 2);
  tft.println("*PLANT SENSOR*");
  tft.setTextColor(TFT_GREEN);
  tft.setCursor(70, 120, 2);
  tft.println("v0.0 mar2025");

  delay(1000);


  tft.fillScreen(TFT_BLACK);


  // circles positions
  posX[0] = 40;    posY[0] = 150; // 
  posX[1] = 88;    posY[1] = 192; // 
  posX[2] = 152;   posY[2] = 192; //
  posX[3] = 200;   posY[3] = 150; //

/*
  tft.fillCircle(152, 192, 30, TFT_GREEN);
  tft.fillCircle(88, 192, 30, TFT_GREEN);
  tft.fillCircle(40, 150, 30, TFT_GREEN);
  tft.fillCircle(200, 150, 30, TFT_GREEN);
*/

  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(3);
  tft.setTextFont(1);

// temp display
  drawTemperatureIcon(78, 73);  // temp icon
  tft.setTextDatum(MR_DATUM); // align RIGHT
  tft.drawString("28", 130, 75);  
  tft.drawCircle(135,65,4,TFT_WHITE);
  tft.drawString("C", 165, 75);  

// umid display
  drawDropIcon(82, 125);  // drop icon
  tft.setTextDatum(MR_DATUM); // align RIGHT
  tft.drawString("72", 137, 115);
  tft.drawString("%", 165, 115);  
  

// wifi-display
  drawWiFiIcon(110, 32);  // 



}

void loop() {
  // put your main code here, to run repeatedly:


// Simulate readings or replace with analogRead()
  moisture1 = random(0, 1024); // or analogRead(A0);
  moisture2 = random(0, 1024); // or analogRead(A1);
  moisture3 = random(0, 1024); // or analogRead(A2);
  moisture4 = random(0, 1024); // or analogRead(A3);

  int readings[4] = {moisture1, moisture2, moisture3, moisture4};

  // Draw each circle
  for (int i = 0; i < 4; i++) {

    uint16_t color = getColorFromValue(readings[i]);


    if (readings[i]<200) {
          tft.fillCircle(posX[i], posY[i], RADIUS-1, TFT_DARKGREY);
          tft.fillCircle(posX[i], posY[i], RADIUS-2, TFT_BLACK);

          // Optional: display moisture value as text
    //tft.setTextColor(TFT_WHITE, color);
    tft.setTextDatum(MC_DATUM); // Centered text
    tft.setTextColor(TFT_RED);
    tft.setTextSize(2);
    tft.setTextFont(1);
    tft.drawNumber(readings[i], posX[i], posY[i]);
    } else {

    tft.fillCircle(posX[i], posY[i], RADIUS, color);
    //tft.drawCircle(posX[i], posY[i], RADIUS, TFT_WHITE);  // optional outline

    // Optional: display moisture value as text
    tft.setTextColor(TFT_WHITE, color);
    tft.setTextDatum(MC_DATUM); // Centered text
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.setTextFont(1);
    tft.drawNumber(readings[i], posX[i], posY[i]);
    }
  }

  updateLEDs();

  delay(1000); // Adjust refresh rate



}


void drawWiFiIcon(int x, int y) {
  // Draw 3 arcs with increasing radius
  tft.drawArc(x, y, 14, 16, 225, 315, TFT_BLUE, TFT_BLACK); // outer arc
  tft.drawArc(x, y, 10, 12, 225, 315, TFT_BLUE, TFT_BLACK); // middle arc
  tft.drawArc(x, y, 6, 8, 225, 315, TFT_BLUE, TFT_BLACK);   // inner arc

  // Draw center dot
  tft.fillCircle(x+2, y, 2, TFT_BLUE);  // small dot at bottom center
}

void drawTemperatureIcon(int x, int y) {
  // Draw bulb (bottom circle)
  tft.fillCircle(x, y + 10, 6, TFT_WHITE);         // Outer red bulb
  //tft.drawCircle(x, y + 10, 6, TFT_WHITE);       // Outline

  // Draw vertical tube
  tft.fillRect(x - 2, y - 12, 4, 18, TFT_WHITE); // Thermometer tube
  //tft.fillRect(x - 1, y - 11, 2, 20, TFT_WHITE);   // Red fill for mercury

  // Draw outline around tube
  //tft.drawRect(x - 2, y - 12, 4, 22, TFT_WHITE);

  // Optional: Draw side ticks
  for (int i = 0; i < 5; i++) {
    int tickY = y - 10 + i * 5;
    tft.drawFastHLine(x + 2, tickY, 4, TFT_WHITE);
  }
}

// 🔷 Draw a 28-pixel tall blue water drop icon at (x, y) – center bottom of drop
void drawDropIcon(int x, int y) {
  // Dimensions
  int height = 28;
  int width = 18;
  int radius = 9;

  // Draw bottom circle (bulb of the drop)
  tft.fillCircle(x, y - radius, radius, TFT_WHITE);

  // Draw top triangle (pointy part of the drop)
  int topY = y - height;
  tft.fillTriangle(x - radius, y - radius, x + radius, y - radius, x, topY, TFT_WHITE);

  // Optional: outline
  //tft.drawCircle(x, y - radius, radius, TFT_WHITE);
  //tft.drawLine(x, topY, x - radius, y - radius, TFT_WHITE);
  //tft.drawLine(x, topY, x + radius, y - radius, TFT_WHITE);
}


// Map moisture value (0–1023) to RGB565 red–green gradient
uint16_t getColorFromValue(int value) {
  value = constrain(value, 0, 1023);
  uint8_t red   = map(value, 0, 1023, 255, 0);   // Red fades out
  uint8_t green = map(value, 0, 1023, 0, 255);   // Green increases
  return tft.color565(red, green, 0);
}


void updateLEDs() {
  // Array of moisture variables
  int moistureValues[4] = {moisture1, moisture2, moisture3, moisture4};

  for (int i = 0; i < 4; i++) {
    // Get 16-bit RGB565 color from TFT logic
    uint16_t color565 = getColorFromValue(moistureValues[i]);

    // Convert RGB565 to RGB888 (FastLED format)
    uint8_t r = ((color565 >> 11) & 0x1F) << 3;
    uint8_t g = ((color565 >> 5) & 0x3F) << 2;
    uint8_t b = (color565 & 0x1F) << 3;

    leds[i] = CRGB(r, g, b);
  }

  FastLED.show();
}


