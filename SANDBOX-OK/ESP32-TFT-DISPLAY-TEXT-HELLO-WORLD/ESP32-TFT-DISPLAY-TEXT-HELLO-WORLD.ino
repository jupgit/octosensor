//The Game of Life, also known simply as Life, is a cellular automaton
//devised by the British mathematician John Horton Conway in 1970.
// https://en.wikipedia.org/wiki/Conway's_Game_of_Life


#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library
TFT_eSPI tft = TFT_eSPI();       // Invoke custom library



int sensorValue[8]={32, 64, 128, 256, 800, 512, 256, 128};
int oldSensorValue[8]={0, 0, 0, 0, 0, 0, 0, 0};



/// DEFINE COLORS

#define TFT_GRAY 0x7BEF
#define TFT_LIGHT_GRAY 0xC618
#define TFT_DARK_GRAY 0x632C
#define TFT_GREEN 0x07E0
#define TFT_LIME 0x87E0
#define TFT_BLUE 0x001F
#define TFT_RED 0xF800
#define TFT_AQUA 0x5D1C
#define TFT_YELLOW 0xFFE0
#define TFT_MAGENTA 0xF81F
#define TFT_CYAN 0x07FF
#define TFT_DARK_CYAN 0x03EF
#define TFT_ORANGE 0xFCA0
#define TFT_PINK 0xF97F
#define TFT_BROWN 0x8200
#define TFT_VIOLET 0x9199
#define TFT_SILVER 0xA510
#define TFT_GOLD 0xA508
#define TFT_NAVY 0x000F
#define TFT_MAROON 0x7800
#define TFT_PURPLE 0x780F
#define TFT_OLIVE 0x7BE0



void setup()   {

  //Set up the display
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);

  // MENU HEADER
  // MENU BAR    
  tft.fillRect(0, 0, 240, 30, TFT_DARK_GRAY);
  // SETA MENU ARROW
  tft.fillTriangle(18,10, 18,20, 10,15, TFT_WHITE);
  // MENU HEADER TEXT
  tft.setCursor(30, 8);
  tft.println(F("Sensor readings"));
  
  // PRINT NUMBERS FOR 1 TO 8
  for (int s=0; s<=7; s++) {  
    tft.setCursor(0, 80+(s*25));
    tft.print(s+1);  

  }






}

void loop() {


 updateSensorsBarGraph();
  
  delay(100);

  //  RECICLA VALORES PARA ANIMAÇAO TESTE
  sensorValue[5] = sensorValue[0];
  for (int s=0; s<=4; s++) {  
    sensorValue[s] = sensorValue[s+1]; 
  }


}








////
////
////  F U N C T I O N S 
////
////



void updateSensorsBarGraph() {

  for (int s=0; s<=7; s++) {  
    if (sensorValue[s] != oldSensorValue[s]) {
      
      // ERASES BACKGROUND OF THE BAR
      tft.fillRect(30, 76+(s*25), 210, 24, TFT_BLACK);
      // DRAWS THE BAR
      tft.fillRect(30, 76+(s*25), sensorValue[s]/5, 24, TFT_RED);
      // PRINTS THE SENSOR VALUE
      tft.setCursor(35, 80+(s*25));
      tft.print(sensorValue[s]);
  
      oldSensorValue[s] = sensorValue[s];
    }
  }
  
}
