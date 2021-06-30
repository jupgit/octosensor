
// DISPLAY OLED
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



// BUZZER ////////////////////////////////////////////////////////////////////////////////////////////////////

#include <CuteBuzzerSounds.h>
#define BUZZER_PIN 15





// MENU SYSTEM ////////////////////////////////////////////////////////////////////////////////////////////////////
int menu = 11;     /// = MENU 1 / PAG 1
int lastMenu = 1;
int cursorLine = 1;
int cursorRow = 0;
int lastCursorLine = 1;

String menuRange1, menuRange2, menuRange3;





// BUTTONS ////////////////////////////////////////////////////////////////////////////////////////////////////

boolean bot_ENTER = 0 ;
boolean bot_UP = 0 ;
boolean bot_DOWN = 0 ;

int bot_ENTER_PIN = 19;
int bot_UP_PIN = 32;
int bot_DOWN_PIN = 33;







// RANGES ////////////////////////////////////////////////////////////////////////////////////////////////////

// [1] Trigger (min. 30cm) - NEAR THE SENSOR
// [2] Interactive
// [3] Idle - FAR FROM SENSOR

// values in centimeters
int range1_Trigger_min = 30;   /// minimum for TFMini Sensor
int range1_Trigger_max = 80;

int range2_Interact_min = range1_Trigger_max+1;
int range2_Interact_max = 100;

int range3_Idle_min = range2_Interact_max+1;
int range3_Idle_max = 250;

// Range indicator
int octoRange = 3;




// BLINK WITHOUT DELAY ////////////////////////////////////////////////////////////////////////////////////////////////////

int blinkState = LOW;             // blinkState used to set the LED
unsigned long previousMillis = 0;        // will store last time LED was updated
const long blinkInterval = 200; 

String blinkText;



// MILLIS CLOCK  ////////////////////////////////////////////////////////////////////////////////////////////////////
  unsigned long currentMillis = millis();














// ============================================== SETUP


void setup() {
  Serial.begin(9600);




// DISPLAY OLED

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(1000); // Pause for 2 seconds

  display.clearDisplay();   // Clear the buffer
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(36,16);
  display.write("Octo");
  display.setCursor(30,32);
  display.write("Sensor");

  // Line animation
  for (int i=0; i<=64; i=i+2) {
   display.drawLine(64-i, 32, 64+i, 32, 1);
   display.display();
  }

  // buzzer
  cute.init(BUZZER_PIN);
  cute.play(S_FART3);

  delay(2000); // Pause for 2 seconds

  
  display.clearDisplay();
  

// PUSH BUTTONS
pinMode (bot_ENTER_PIN, INPUT_PULLDOWN);
pinMode (bot_UP_PIN, INPUT_PULLDOWN);
pinMode (bot_DOWN_PIN, INPUT_PULLDOWN);


}















// ============================================== LOOP


void loop() {


 currentMillis = millis();


  // blinkState
  if (currentMillis - previousMillis >= blinkInterval) {
    // save the last time you blinked
    previousMillis = currentMillis;
    // if the LED is off turn it on and vice-versa:
    if (blinkState == LOW) {
      blinkState = HIGH;
    } else {
      blinkState = LOW;
    }
  }
 



// debug
Serial.print("menu: ");
Serial.print(menu);

Serial.print(" | cursorLine: ");
Serial.print(cursorLine);

Serial.print(" | UP: ");
Serial.print(bot_UP);

Serial.print(" | DOWN: ");
Serial.print(bot_DOWN);

Serial.print(" | BLINK: ");
Serial.print(blinkState);

Serial.println();
//


/////////////////////////////////////////////////
//  (MENU): MENU 11 - Menu 1 / Pagina 1
/////////////////////////////////////////////////

if (menu == 11) {


/// Check buttons (up, down) and change cursorLine
    checkButtons();
    lastCursorLine = cursorLine;
  

/// MENU DISPLAY
    printMenu(" MENU", " Info", " Config", " Exhibit");

/// MENU LIMITS
    if (cursorLine < 1) { cursorLine = 1; }              // MININUM LINE = 1
    if (cursorLine > 3) { cursorLine = 1; menu = 12; }   // MAXIMUM LINE = 3 // GOTO PG 2



/// LED Feedback  ???
///   if (lastCursorLine!=cursorLine) {  }

//    lastCursorLine = cursorLine;


/// VERIFY IF 'ENTER' BUTTON IS PUSHED and CHANGES menu 
  
  if  (digitalRead(bot_ENTER_PIN) == 1 && bot_ENTER == 0)  {
    
      if (cursorLine == 1) { menu = 21; }  // line 1 > GO TO MENU 21 // Infos
      if (cursorLine == 2) { menu = 38; }  // line 2 > GO TO MENU 31 // Config
      if (cursorLine == 3) { menu = 41; }  // line 3 > GO TO MENU 41 // Exhibit

      cursorLine = 1; // back to the top

      bot_ENTER = 1; // flag up enter button    

 /// VERIFY IF 'ENTER' BUTTON IS NOT PRESSED
    }  else if (digitalRead(bot_ENTER_PIN)==LOW && bot_ENTER==1) {
      
       bot_ENTER = 0; // flag down enter button   
     }
     
}



/////////////////////////////////////////////////
//  (MENU): MENU 12 - Menu 1 / Pagina 2
/////////////////////////////////////////////////
  

if (menu == 12) {

/// Check buttons (up, down) and change cursorLine
    checkButtons();
    lastCursorLine = cursorLine;
    
//// MENU DISPLAY
     printMenu(" MENU", " Standby", " (empty)", " (empty)");

//// MENU LIMITS
     if (cursorLine < 1) { cursorLine = 3; menu = 11; }  // MININUM LINE = 1 / Back to 11
     if (cursorLine > 3) { cursorLine = 3; }             // MAXIMUM LINE = 3

     //if (lastCursorLine!=cursorLine) {lastCursorLine = cursorLine;}



  /// VERIFY IF 'ENTER' BUTTON IS PUSHED and CHANGES menu 

  if  (digitalRead(bot_ENTER_PIN) == 1 && bot_ENTER == 0)  {

    if (cursorLine == 1) { menu = 11; }  // linha 1 > GO TO MENU 
    if (cursorLine == 2) { menu = 11; }  // linha 2 > GO TO MENU 
    if (cursorLine == 3) { menu = 11; }  // linha 3 > GO TO MENU 

      cursorLine = 1; // back to the top

      bot_ENTER = 1; // flag up enter button    

 /// VERIFY IF 'ENTER' BUTTON IS NOT PRESSED
    }  else if (digitalRead(bot_ENTER_PIN)==LOW && bot_ENTER==1) {
      
       bot_ENTER = 0; // flag down enter button   
     }

}




/////////////////////////////////////////////////
//  (INFO): MENU 21 - Menu 2 / Pagina 1
/////////////////////////////////////////////////

if (menu == 21) {

/// Check buttons (up, down) and change cursorLine
    checkButtons();
    lastCursorLine = cursorLine;
    
/// MENU DISPLAY
    printMenu(" INFO", "", "", "");
    display.setCursor(0,20);
    display.write("Node:");
    display.write("1");            ////!!! trocar por variável
    display.setCursor(0,36);
    display.write("Sync:");
    display.write("12345");            ////!!! trocar por variável
    display.setCursor(15,56);
    display.setTextSize(1);  
    display.write("C4:4F:33:3E:C3:85"); ////!!! trocar por variável
    display.setCursor(100,0);
    display.write("v0.0");
    display.setTextSize(2);             // 



/// MENU LIMITS
   if (cursorLine < 0) { cursorLine = 0; }  // MININUM LINE = 0
   if (cursorLine > 0) { cursorLine = 0; }  // MAXIMUM LINE = 0




/// VERIFY IF 'ENTER' BUTTON IS PUSHED and CHANGES menu 
  
    if  (digitalRead(bot_ENTER_PIN) == 1 && bot_ENTER == 0)  {
    
      if (cursorLine == 0) { menu = 11; }  // line 1 > GO TO MENU 11 (Menu)

      cursorLine = 1; // back to the top

      bot_ENTER = 1; // flag up enter button

 /// VERIFY IF 'ENTER' BUTTON IS NOT PRESSED
    }  else if (digitalRead(bot_ENTER_PIN)==LOW && bot_ENTER==1) {
      
       bot_ENTER = 0; // flag down enter button   
     }
}











/////////////////////////////////////////////////
//  (PERFORM): MENU 31 - Menu 3 / Pagina 1
/////////////////////////////////////////////////
  

if (menu == 31) {

/// Check buttons (up, down) and change cursorLine
    checkButtons();
    lastCursorLine = cursorLine;
    
     printMenu(" MENU 31", " Test", " PontosG", "");

//// MENU:

// Chama função que muda linha de acordo com o Encoder
 // encoderChangeLine();

  /// LIMITES DO MENU
   if (cursorLine < 0) { cursorLine = 0; }  // MININUM LINE = 0
   if (cursorLine > 2) { cursorLine = 2; }  // MAXIMUM LINE = 2




  // VERIFICA SE O BOTAO FOI APERTADO
  // APERTADO = 0 // SOLTO = 1 ( GND quando aperta )
  if  (digitalRead(bot_ENTER_PIN) == 1 && bot_ENTER == 0)  {

    if (cursorLine == 0) { menu = 11; }  // line 0 > GO TO MENU 11
    if (cursorLine == 1) { menu = 41; }  // line 1 > GO TO MENU 41
    if (cursorLine == 2) { menu = 41; }  // line 2 > GO TO MENU 41

    cursorLine = 1;

    bot_ENTER = 1;
    }  

}


/////////////////////////////////////////////////
//  (RANGES(M)): MENU 38 - Menu 3 / Pagina 8
/////////////////////////////////////////////////

if (menu == 38) {

/// Check buttons (up, down) and change cursorLine
    checkButtons();
    lastCursorLine = cursorLine;
    
/// MENU DISPLAY

    menuRange1 = String(" 1:") + String(range1_Trigger_min) + String("-") + String(range1_Trigger_max);
    menuRange2 = String(" 2:") + String(range2_Interact_min) + String("-") + String(range2_Interact_max);
    menuRange3 = String(" 3:") + String(range3_Idle_min) + String("-") + String(range3_Idle_max);

    printMenu(" RANGES(M)", menuRange1, menuRange2, menuRange3);
    

/// MENU LIMITS
    if (cursorLine < 0) { cursorLine = 0; }   // MININUM LINE = 1
    if (cursorLine > 3) { cursorLine = 3; }   // MAXIMUM LINE = 3




/// VERIFY IF 'ENTER' BUTTON IS PUSHED and CHANGES menu 
  
  if  (digitalRead(bot_ENTER_PIN) == 1 && bot_ENTER == 0)  {

      if (cursorLine == 0) { menu = 11; }  // line 0 > GO TO MENU 11
      if (cursorLine == 1) { menu = 381; }  // line 1 > 
      if (cursorLine == 2) { menu = 382; }  // line 2 > 
      if (cursorLine == 3) { menu = 383; }  // line 3 > 
      
      cursorLine = 1; // back to the top

      bot_ENTER = 1; // flag up enter button    

 /// VERIFY IF 'ENTER' BUTTON IS NOT PRESSED
    }  else if (digitalRead(bot_ENTER_PIN)==LOW && bot_ENTER==1) {
      
       bot_ENTER = 0; // flag down enter button   
     }
     
}



//////////////////////////////////////////////////////////////////////
//  (RANGES(M)): MENU 381 - Menu 3 / Pagina 8 / LINHA 1 - EDIT
//////////////////////////////////////////////////////////////////////

if (menu == 381) {

  
/// MENU DISPLAY WITH BLINKING TEXT

    // blinkText
    if (blinkState) { blinkText = String(range1_Trigger_max); } else { blinkText = String("  ");}


    menuRange1 = String(" 1:") + String(range1_Trigger_min) + String("-") + blinkText;
    menuRange2 = String(" 2:") + String(range2_Interact_min) + String("-") + String(range2_Interact_max);
    menuRange3 = String(" 3:") + String(range3_Idle_min) + String("-") + String(range3_Idle_max);

    printMenu(" RANGES", menuRange1, menuRange2, menuRange3);


/// CUSTOM UP/DOWN BUTTOM : Change range1_Trigger_max
    // BUTTOM UP
    if (digitalRead(bot_UP_PIN)==HIGH && bot_UP==0) { 
      range1_Trigger_max = range1_Trigger_max + 1 ;
      bot_UP = 1; 
     } else if (digitalRead(bot_UP_PIN)==LOW && bot_UP==1) {
       bot_UP = 0; 
     }

    // BUTTOM DOWN
    if (digitalRead(bot_DOWN_PIN)==HIGH && bot_DOWN==0) { 
      range1_Trigger_max = range1_Trigger_max - 1 ;
      bot_DOWN = 1; 
     } else if (digitalRead(bot_DOWN_PIN)==LOW && bot_DOWN==1) {
       bot_DOWN = 0; 
     }  


/// VERIFY IF 'ENTER' BUTTON IS PUSHED and CHANGES menu 
  
  if  (digitalRead(bot_ENTER_PIN) == 1 && bot_ENTER == 0)  {
    
      menu = 38;
      
      cursorLine = 1; // back to the top

      bot_ENTER = 1; // flag up enter button   

      cute.play(S_MODE1); 


 /// VERIFY IF 'ENTER' BUTTON IS NOT PRESSED
    }  else if (digitalRead(bot_ENTER_PIN)==LOW && bot_ENTER==1) {
      
       bot_ENTER = 0; // flag down enter button   
     }



// Ranges calculations

range2_Interact_min = range1_Trigger_max + 1;


  if (range2_Interact_min >= range2_Interact_max) {
     range2_Interact_max = range2_Interact_min +1;
  }
  
  range3_Idle_min = range2_Interact_max + 1;
  
  
  if (range3_Idle_min >= range3_Idle_max) {
     range3_Idle_max = range3_Idle_min +1;
  }


}





//////////////////////////////////////////////////////////////////////
//  (RANGES(M)): MENU 382 - Menu 3 / Pagina 8 / LINHA 2 - EDIT
//////////////////////////////////////////////////////////////////////

if (menu == 382) {

  
/// MENU DISPLAY WITH BLINKING TEXT

    // blinkText
    if (blinkState) { blinkText = String(range2_Interact_max); } else { blinkText = String("  ");}


    menuRange1 = String(" 1:") + String(range1_Trigger_min) + String("-") + String(range1_Trigger_max);
    menuRange2 = String(" 2:") + String(range2_Interact_min) + String("-") + blinkText;
    menuRange3 = String(" 3:") + String(range3_Idle_min) + String("-") + String(range3_Idle_max);


   cursorLine = 2;


    printMenu(" RANGES", menuRange1, menuRange2, menuRange3);


/// CUSTOM UP/DOWN BUTTOM : Change range2_Interact_max
    // BUTTOM UP
    if (digitalRead(bot_UP_PIN)==HIGH && bot_UP==0) { 
      range2_Interact_max = range2_Interact_max + 1 ;
      bot_UP = 1; 
     } else if (digitalRead(bot_UP_PIN)==LOW && bot_UP==1) {
       bot_UP = 0; 
     }

    // BUTTOM DOWN
    if (digitalRead(bot_DOWN_PIN)==HIGH && bot_DOWN==0) { 
      range2_Interact_max = range2_Interact_max - 1 ;
      bot_DOWN = 1; 
     } else if (digitalRead(bot_DOWN_PIN)==LOW && bot_DOWN==1) {
       bot_DOWN = 0; 
     }  


/// VERIFY IF 'ENTER' BUTTON IS PUSHED and CHANGES menu 
  
  if  (digitalRead(bot_ENTER_PIN) == 1 && bot_ENTER == 0)  {
    
      menu = 38;
      
      cursorLine = 1; // back to the top

      bot_ENTER = 1; // flag up enter button   

      cute.play(S_MODE1); 


 /// VERIFY IF 'ENTER' BUTTON IS NOT PRESSED
    }  else if (digitalRead(bot_ENTER_PIN)==LOW && bot_ENTER==1) {
      
       bot_ENTER = 0; // flag down enter button   
     }



// Ranges calculations

range2_Interact_min = range1_Trigger_max + 1;


  if (range2_Interact_min >= range2_Interact_max) {
     range2_Interact_max = range2_Interact_min +1;
  }
  
  range3_Idle_min = range2_Interact_max + 1;
  
  
  if (range3_Idle_min >= range3_Idle_max) {
     range3_Idle_max = range3_Idle_min +1;
  }


}




//////////////////////////////////////////////////////////////////////
//  (RANGES(M)): MENU 383 - Menu 3 / Pagina 8 / LINHA 3 - EDIT
//////////////////////////////////////////////////////////////////////

if (menu == 383) {

  
/// MENU DISPLAY WITH BLINKING TEXT

    // blinkText
    if (blinkState) { blinkText = String(range3_Idle_max); } else { blinkText = String("  ");}


    menuRange1 = String(" 1:") + String(range1_Trigger_min) + String("-") + String(range1_Trigger_max);
    menuRange2 = String(" 2:") + String(range2_Interact_min) + String("-") + String(range2_Interact_max); 
    menuRange3 = String(" 3:") + String(range3_Idle_min) + String("-") + blinkText;


   cursorLine = 3;


    printMenu(" RANGES", menuRange1, menuRange2, menuRange3);


/// CUSTOM UP/DOWN BUTTOM : Change range2_Interact_max
    // BUTTOM UP
    if (digitalRead(bot_UP_PIN)==HIGH && bot_UP==0) { 
      range3_Idle_max = range3_Idle_max + 1 ;
      bot_UP = 1; 
     } else if (digitalRead(bot_UP_PIN)==LOW && bot_UP==1) {
       bot_UP = 0; 
     }

    // BUTTOM DOWN
    if (digitalRead(bot_DOWN_PIN)==HIGH && bot_DOWN==0) { 
      range3_Idle_max = range3_Idle_max - 1 ;
      bot_DOWN = 1; 
     } else if (digitalRead(bot_DOWN_PIN)==LOW && bot_DOWN==1) {
       bot_DOWN = 0; 
     }  


/// VERIFY IF 'ENTER' BUTTON IS PUSHED and CHANGES menu 
  
  if  (digitalRead(bot_ENTER_PIN) == 1 && bot_ENTER == 0)  {
    
      menu = 38;
      
      cursorLine = 1; // back to the top

      bot_ENTER = 1; // flag up enter button   

      cute.play(S_MODE1); 


 /// VERIFY IF 'ENTER' BUTTON IS NOT PRESSED
    }  else if (digitalRead(bot_ENTER_PIN)==LOW && bot_ENTER==1) {
      
       bot_ENTER = 0; // flag down enter button   
     }



// Ranges calculations

range2_Interact_min = range1_Trigger_max + 1;


  if (range2_Interact_min >= range2_Interact_max) {
     range2_Interact_max = range2_Interact_min +1;
  }
  
  range3_Idle_min = range2_Interact_max + 1;
  
  
  if (range3_Idle_min >= range3_Idle_max) {
     range3_Idle_max = range3_Idle_min +1;
  }


}




/////////////////////////////////////////////////
//  (MENU): MENU 41 - Menu 4 / Pagina 1
/////////////////////////////////////////////////


if (menu == 41) {

/// Check buttons (up, down) and change cursorLine
    checkButtons();
    lastCursorLine = cursorLine;
    
    printMenu(" MENU 41", "", "Pause", "");

  display.display();


  delay(2000);
  menu = 11;

  
  cursorLine = 1;

} 

}





//====================================
//==   F   U   N   C   O   E   S   ==
//====================================


void  checkButtons() {



/// REGULAR MENU UP/DOWN

    // BUTTOM UP
    if (digitalRead(bot_UP_PIN)==HIGH && bot_UP==0) { 
      bot_UP = 1; 
      cute.play(S_BUTTON_PUSHED); 
      cursorLine = cursorLine - 1 ;
     } else if (digitalRead(bot_UP_PIN)==LOW && bot_UP==1) {
       bot_UP = 0; 
     }

    // BUTTOM DOWN
    if (digitalRead(bot_DOWN_PIN)==HIGH && bot_DOWN==0) { 
      bot_DOWN = 1; 
      cute.play(S_BUTTON_PUSHED); 
        Serial.println("---");

      cursorLine = cursorLine + 1 ;
     } else if (digitalRead(bot_DOWN_PIN)==LOW && bot_DOWN==1) {
       bot_DOWN = 0; 
     }  
  
  }


void printMenu (String line0, String line1, String line2, String line3) {

  
  display.clearDisplay();
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  display.setCursor(0,0);
  display.drawLine(0,15,128,15,1);

  display.setTextSize(2);

  display.println(line0); 
  display.println(line1);
  display.println(line2);
  display.println(line3);



  /// ARROW POSITION (for each line from 1...4 ==> y = 0, 16, 32, 48)
    display.setCursor(0,(cursorLine*16));
    // Right ARROW (Char code = 16) / Left ARROW (Char code = 17)
    if ( cursorLine == 0 ) {display.write(17);} else {display.write(16);} 
    display.display();

  display.display();

 
}
  
