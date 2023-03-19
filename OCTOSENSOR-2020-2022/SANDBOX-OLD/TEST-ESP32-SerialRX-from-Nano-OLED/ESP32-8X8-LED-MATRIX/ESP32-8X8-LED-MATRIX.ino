#include <SPI.h>
#include "LedMatrix.h"
 
#define NUMBER_OF_DEVICES 1
#define CS_PIN 5
LedMatrix ledMatrix = LedMatrix(NUMBER_OF_DEVICES, CS_PIN);
 
void setup()
{
ledMatrix.init();
ledMatrix.setIntensity(4); // range is 0-15
ledMatrix.setText("The quick brown fox jumps over the lazy dog");
}
 
void loop()
{
ledMatrix.clear();
//ledMatrix.scrollTextLeft();
//ledMatrix.drawText();

for (int y=0; y<8; y++) {
  for (int x=0; x<8; x++) {
    ledMatrix.setPixel(x, y); 
    delay(50);
  }
      ledMatrix.commit();

}

ledMatrix.clear();
ledMatrix.commit();


}
