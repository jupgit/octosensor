#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display width and height, change if yours is different
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// OLED reset pin (often not used with ESP32)
#define OLED_RESET     -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Total number of circles
const int numCircles = 16;

// Circle properties
const int radius = 3;
const int spacing = 10;  // horizontal space between circle centers

// Starting position (center of first circle)
const int startX = 4;
const int yPos = 32; // vertical center

// Stores whether each circle is filled
bool circleFilled[numCircles] = { false };

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Most 0.96 OLEDs use address 0x3C
  display.clearDisplay();
  display.display();
  delay(1000); // Optional start delay

  // Draw unfilled circles one at a time
  for (int i = 0; i < numCircles; i++) {
    int x = startX + i * spacing;
    display.drawCircle(x, yPos, radius, SSD1306_WHITE);
    display.display();
    delay(100);
  }

  // Fill circles one at a time
  for (int i = 0; i < numCircles; i++) {
    int x = startX + i * spacing;
    display.fillCircle(x, yPos, radius, SSD1306_WHITE);
    display.display();
    delay(1000);
  }
}

void loop() {
  // Nothing to do here – everything is done in setup()
}
