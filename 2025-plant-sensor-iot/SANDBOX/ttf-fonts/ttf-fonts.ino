#include <TFT_eSPI.h>
#include <Fonts/Free_Fonts.h> // Load FreeFonts
TFT_eSPI tft = TFT_eSPI();

void setup() {
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  int y = 5;

  // Default GLCD Font
  tft.setCursor(5, y);
  tft.setTextFont(1);  // Built-in font
  tft.print("Built-in Font (setTextFont(1))");
  y += 20;

  // List of FreeFonts to test
  const GFXfont* fonts[] = {
    &FreeMono9pt7b, &FreeMonoBold9pt7b, &FreeMonoOblique9pt7b, &FreeMonoBoldOblique9pt7b,
    &FreeSans9pt7b, &FreeSansBold9pt7b, &FreeSerif9pt7b
  };

  const char* fontNames[] = {
    "FreeMono9pt7b", "FreeMonoBold9pt7b", "FreeMonoOblique9pt7b", "FreeMonoBoldOblique9pt7b",
    "FreeSans9pt7b", "FreeSansBold9pt7b", "FreeSerif9pt7b"
  };

  for (int i = 0; i < sizeof(fonts)/sizeof(fonts[0]); i++) {
    tft.setFreeFont(fonts[i]);
    tft.setCursor(5, y);
    tft.print(fontNames[i]);
    y += 30;  // Adjust spacing between lines
  }
}

void loop() {
  // Nothing here
}
