#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();

// Display settings
const int SCREEN_WIDTH = 240;
const int SCREEN_HEIGHT = 240;
const int CENTER_X = SCREEN_WIDTH / 2;
const int CENTER_Y = SCREEN_HEIGHT / 2;
const int RADIUS = 30;

// Moisture Circles Positions
int posX[4];
int posY[4];

// Simulated or actual moisture readings (0–1023)
int moisture1, moisture2, moisture3, moisture4;

void setup() {
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);

  // circles positions
  posX[0] = 40;    posY[0] = 150; // 
  posX[1] = 88;    posY[1] = 192; // 
  posX[2] = 152;   posY[2] = 192; //
  posX[3] = 200;   posY[3] = 150; //


  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(3);

  tft.setCursor(90, 60, 1);
  tft.println("28 C");

  tft.setCursor(90, 100, 1);
  tft.println("72 %");

  tft.setTextSize(1);


}

void loop() {
  // Simulate readings or replace with analogRead()
  moisture1 = random(0, 1024); // or analogRead(A0);
  moisture2 = random(0, 1024); // or analogRead(A1);
  moisture3 = random(0, 1024); // or analogRead(A2);
  moisture4 = random(0, 1024); // or analogRead(A3);

  int readings[4] = {moisture1, moisture2, moisture3, moisture4};

  // Draw each circle
  for (int i = 0; i < 4; i++) {
    uint16_t color = getColorFromValue(readings[i]);
    tft.fillCircle(posX[i], posY[i], RADIUS, color);
    //tft.drawCircle(posX[i], posY[i], RADIUS, TFT_WHITE);  // optional outline

    // Optional: display moisture value as text
    tft.setTextColor(TFT_WHITE, color);
    tft.setTextDatum(MC_DATUM); // Centered text
    tft.setTextFont(2);
    tft.drawNumber(readings[i], posX[i], posY[i]);
  }

  delay(500); // Adjust refresh rate
}

// Map moisture value (0–1023) to RGB565 red–green gradient
uint16_t getColorFromValue(int value) {
  value = constrain(value, 0, 1023);
  uint8_t red   = map(value, 0, 1023, 255, 0);   // Red fades out
  uint8_t green = map(value, 0, 1023, 0, 255);   // Green increases
  return tft.color565(red, green, 0);
}


void drawWiFiIcon(int x, int y) {
  // Draw 3 arcs with increasing radius
  tft.drawArc(x, y, 14, 16, 225, 315, TFT_BLUE, TFT_BLACK); // outer arc
  tft.drawArc(x, y, 10, 12, 225, 315, TFT_BLUE, TFT_BLACK); // middle arc
  tft.drawArc(x, y, 6, 8, 225, 315, TFT_BLUE, TFT_BLACK);   // inner arc

  // Draw center dot
  tft.fillCircle(x, y + 10, 2, TFT_BLUE);  // small dot at bottom center
}
