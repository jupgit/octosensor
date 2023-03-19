

#include <SPI.h>
#include <Wire.h>                 
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void setup() {
  
  Serial.begin(9600);

  // Start I2C Communication SDA = 5 and SCL = 4 on Wemos Lolin32 ESP32 with built-in SSD1306 OLED
  Wire.begin(5, 4);
  

// INIT OLED DISPLAY

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }


  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  
  OLED.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  OLED.clearDisplay();

}



void loop() {

//
// Um pixel
//

  OLED.drawPixel(64, 32, SSD1306_WHITE);
  OLED.display();
  delay(2000);


  
  OLED.clearDisplay();



//
// Letra "A" em varios tamanhos
//
  OLED.setTextColor(WHITE);
  OLED.setCursor(0,0);
  OLED.setTextSize(1);
  OLED.print("A");
  OLED.setTextSize(2);
  OLED.print("A");
  OLED.setTextSize(3);
  OLED.print("A");
  OLED.setTextSize(4);
  OLED.print("A");
  OLED.setTextSize(5);
  OLED.print("A");
  OLED.setTextSize(6);
  OLED.print("A");
   
  OLED.display();
  
  delay(3000);

  OLED.clearDisplay();



//
// IMPRIMINDO CARACTERE POR CARACTERE
//

  OLED.clearDisplay();
  OLED.setTextColor(WHITE);
  OLED.setCursor(30,30);
  OLED.setTextSize(1);
  
  OLED.write(72);
  OLED.write(101);
  OLED.write(108);
  OLED.write(108);
  OLED.write(111);
  OLED.write(32);
  OLED.write(87);
  OLED.write(111);
  OLED.write(114);
  OLED.write(108);
  OLED.write(100);
  OLED.write(33);
  
  OLED.display();
    
  delay(3000);



//
// TEXTO INVERTIDO
//

  OLED.clearDisplay();

// Sets the color to black with a white background
  OLED.setTextColor(BLACK, WHITE);
  OLED.setCursor(0 ,20);
  OLED.setTextSize(2);
  OLED.println(" Inverted    Text!  ");
   
  OLED.display();
  
  delay(3000);



//
// LINHAS (DRAWLINE)
//

  OLED.clearDisplay();
  
  OLED.drawLine(0,0,OLED.width() - 1, OLED.height() - 1, WHITE);
  OLED.drawLine(OLED.width() - 1,0,0, OLED.height() - 1, WHITE);
  
  OLED.display();

  delay(3000);


//
// RETANGULO
//

  OLED.clearDisplay();

  OLED.drawRect(100, 10, 20, 20, WHITE);
  OLED.fillRect(10, 10, 45, 15, WHITE);
  OLED.drawRoundRect(60, 20, 35, 35, 8, WHITE);
  
  OLED.display();
  
  delay(3000);

//
// CIRCULO
//

  OLED.clearDisplay();


  OLED.drawCircle(60, 30, 30, WHITE);
  OLED.fillCircle(50, 20, 5, WHITE);
  OLED.fillCircle(70, 20, 5, WHITE);

  OLED.display();
  
  delay(3000);

//
// TRIANGULO
//

  OLED.clearDisplay();


  OLED.drawTriangle(24, 1, 3, 55, 45, 55, WHITE);
  OLED.fillTriangle(104, 62, 125, 9, 83, 9, WHITE);

  OLED.display();
  
  delay(3000);


//
// PIXELS
//


  OLED.clearDisplay();

  OLED.drawPixel(20, 35, WHITE);
  OLED.drawPixel(45, 12, WHITE);
  OLED.drawPixel(120, 59, WHITE);
  OLED.drawPixel(97, 20, WHITE);
  OLED.drawPixel(35, 36, WHITE);
  OLED.drawPixel(72, 19, WHITE);
  OLED.drawPixel(90, 7, WHITE);
  OLED.drawPixel(11, 29, WHITE);
  OLED.drawPixel(57, 42, WHITE);
  OLED.drawPixel(69, 34,  WHITE);
  OLED.drawPixel(108, 12, WHITE);

  OLED.display();
  
  delay(3000);



//
// RANDOM PIXELS
//

  randomSeed(analogRead(A3));

  OLED.clearDisplay();

 for (int i=1; i<=100; i++) {

    int xRandom= random(128);
    int yRandom = random(64);

    OLED.clearDisplay();
    OLED.drawPixel(xRandom, yRandom, WHITE);

    OLED.display();

    delay(50);
    
    }

   delay(3000);


 
}
