///////////////////////////////////////////////////////////
//
//  O C T O  S E N S O R   
//  A wireless system with 8 laser range finder modules
//  and a central unit to be used in interactive 
//  art installations.
//
//  Project by Juliano Prado (2021) - Brazil
// 
//  Revision: 03.2023
//
//  https://github.com/jupgit/octo-sensor
//
//  TF LUNA I2C VERSION
//
//  Colaborators:
//   Paulo Cesar Teles (PhD)
//   Klebert
//   Mariana Arias
//   Igor
//   
//   
///////////////////////////////////////////////////////////

/// 
///     C E N T R A L 
///


/*
#include <FastLED.h>

// LED STRIP /////////////////////////////////////////////////////

#define LED_TYPE    WS2811  //WS2812B
#define COLOR_ORDER GRB
#define MAX_BRIGHTNESS  128

// Main LED Strip
#define LED_PIN     26
#define NUM_LEDS    30   /////// NUMERO DE LEDS DA FITA ////////////////////// 

CRGB leds[NUM_LEDS];
*/

// ARRAY WITH 8 SENSORS READINGS

int octoSensor[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int previousOctoSensor[8] = {1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023};

#include <esp_now.h>
#include <WiFi.h>

#include <Wire.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#include <ESP32Servo.h>

//// -------------------------------------- floresta SETTINGS

Servo myservo;  // create servo object to control a servo
// 16 servo objects can be created on the ESP32

int pos = 0;
int servoPin = 18;

// tolerancia entre sensores em cm
int tolerance = 5;
int max_detection = 120; // dist max para detectar
int rotation = 1; // 1=CW / -1=CCW
int max_cycles = 50;   // "survailence" cycles
int curr_cycle = 0;

//fonte d'agua
int relay1Pin = 5; //

// timer (to avoid rapid fire on the relay)
unsigned long current_time = millis();
unsigned long last_time = current_time;
unsigned long relay_timer = 500;   /// delay time in ms


// status machine:

int status_machine = 0;
// status: 0 = search / 1= right / -1 = left / 2 =detected


// ------------------------------------------------------------------------

// OLED display
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// REPLACE WITH THE MAC Address of your receiver 
//uint8_t broadcastAddress[] = {0x7C, 0x9E, 0xBD, 0x45, 0xB3, 0xC0}; // OCTOSENSOR #4
uint8_t broadcastAddress[] = {0xC4, 0x4F, 0x33, 0x3E, 0xE7, 0x6D}; // CENTRAL PROTOBOARD OLED




// Define variables to store readings to be sent
int octoCommand;
int octoNode;
int octoValue;

// Define variables to store incoming readings
int incomingCommand;
int incomingNode;
int incomingValue;

// Variable to store if sending data was successful
String success;

//Structure example to send data
//Must match the receiver structure
typedef struct struct_message {
    int comm;
    int node;
    int value;
} struct_message;

// Create a struct_message called BME280Readings to hold sensor readings
struct_message OCTOReadings;

// Create a struct_message to hold incoming sensor readings
struct_message incomingReadings;

esp_now_peer_info_t peerInfo;


//   TTTTT    X   X
//     T       X X
//     T        X
//     T       X X
//     T      X   X


// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  //Serial.print("\r\nLast Packet Send Status:\t");
  //Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status ==0){
    success = "TX OK";
  }
  else{
    success = "TX Fail :(";
  }
}

//   RRRR    X   X
//   R   R    X X
//   RRRR      X
//   R R      X X
//   R  R    X   X

// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  //Serial.print("Bytes received: ");
  //Serial.println(len);
  incomingCommand = incomingReadings.comm;
  incomingNode = incomingReadings.node;
  incomingValue = incomingReadings.value;


//////////////////////////////////////////////////////////////////////////////////////
// REMOTE SENSOR VALUE into its position in Array 
   octoSensor[incomingNode-1] = incomingValue;
//////////////////////////////////////////////////////////////////////////////////////




 
}




 
// *******************************************************
//
//                  S   E   T   U   P
//
// *******************************************************


void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

  // Init OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    //Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    //Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    //Serial.println("Failed to add peer");
    return;
  }
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);

// servo 
// Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	myservo.setPeriodHertz(50);    // standard 50 hz servo
	myservo.attach(servoPin, 1000, 2000); // attaches the servo on pin 18 to the servo object
	// using default min/max of 1000us and 2000us
	// different servos may require different min/max settings
	// for an accurate 0 to 180 sweep


// water fountain relay
  pinMode(relay1Pin, OUTPUT);
  digitalWrite(relay1Pin, HIGH);
  delay(1000);
  digitalWrite(relay1Pin, LOW);
  delay(500);

/*

// Main LED Strip
  //FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setTemperature(OvercastSky);  
  FastLED.addLeds<WS2811, LED_PIN, GRB>(leds, NUM_LEDS).setTemperature(OvercastSky);  // GRB ordering is typical  /// WS2812B


  FastLED.setBrightness(MAX_BRIGHTNESS/2);

// LED STRIP TEST
  fill_solid(leds, NUM_LEDS, CRGB(255,0,0));
    FastLED.show();
    delay(500); 
  fill_solid(leds, NUM_LEDS, CRGB(0,255,0));
    FastLED.show();
    delay(500);
  fill_solid(leds, NUM_LEDS, CRGB(0,0,255));
  FastLED.show();
    delay(500);
  fill_solid(leds, NUM_LEDS, CRGB(0,0,0));
    FastLED.show();
*/

}

 



// *******************************************************
//
//                    L  O   O   P
//
// *******************************************************



void loop() {
  

 
   int sensorR = octoSensor[1];   //sensor direito
   int sensorL = octoSensor[0];   //sensor esquerdo

    // check if sensor reading = -1 (error) >> if so, set to 500 (5meters)

  if (sensorR == -1) {sensorR = 500;}
  if (sensorL == -1) {sensorL = 500;}



   int delta_d = abs(sensorR-sensorL);
   
   
   Serial.print("R:");
   Serial.print(sensorR);
   Serial.print(" L:");
   Serial.print(sensorL);
   Serial.print(" Delta:");
   Serial.println(delta_d);
   


if ( sensorR > max_detection && sensorL > max_detection ) {

        /// ---------------------------NADA DETECTADO DENTRO DA FAIXA   (max_detection)

        // status: 0 = search / 1= right / -1 = left / 2 =detected
        status_machine = 0;
       
        myservo.write(90-(25*rotation));   // SCANNING
        delay(15);

        digitalWrite(relay1Pin, HIGH);  // fonte ligada

        // resets relay timer
        current_time = millis();
        last_time = current_time;

        //fill_solid(leds, NUM_LEDS, CRGB(0,0,255));
        //FastLED.show();

        curr_cycle++;


        // ------------------------------TROCA DE SENTIDO
        if (curr_cycle>max_cycles) {
          myservo.write(90);   // STOP

          delay(random(1000,2000));
          rotation = rotation * -1;
          curr_cycle=0;

          max_cycles = random(75,125);

        }


        Serial.print(".");
  }


  //------------------------------------- VIRA DIREITA // TURN RIGHT

  if ( (sensorR < sensorL) && ((sensorR< max_detection)||(sensorL < max_detection)) && (delta_d>tolerance)   ) {
  
      // status: 0 = search / 1= right / -1 = left / 2 =detected
      status_machine = 1;
    
        myservo.write(135);   // 50% SPEED CCW
        delay(15);
        rotation = -1;

        digitalWrite(relay1Pin, HIGH);  // fonte ligada

        // resets relay timer
        current_time = millis();
        last_time = current_time;

        //fill_solid(leds, NUM_LEDS, CRGB(0,0,128));
        //FastLED.show();

        Serial.print(">");


  }


    // ---------------------------------------VIRA ESQUERDA / TURN LEFT

    if ( (sensorR > sensorL) && ((sensorR< max_detection)||(sensorL < max_detection)) && (delta_d>tolerance)) {


        // status: 0 = search / 1= right / -1 = left / 2 =detected
        status_machine = -1;

        myservo.write(45);   // 50% SPEED CCW
        delay(15);
        rotation = 1;

        digitalWrite(relay1Pin, HIGH);  // fonte ligada

        // resets relay timer
        current_time = millis();
        last_time = current_time;

        //fill_solid(leds, NUM_LEDS, CRGB(0,0,255));
        //FastLED.show();

        Serial.print("<");

  }

  // -------------------------------------DETECTA A PESSOA // DETECTION

  if ( sensorR < max_detection && sensorL < max_detection && (delta_d <= tolerance)) {

        // status: 0 = search / 1= right / -1 = left / 2 =detected
        status_machine = 2;


        myservo.write(90);   // STOP
        delay(15);

        // updates relay timer
        current_time = millis();

        // only turns off the relay after the delay timer 
        if ( current_time >= last_time + relay_timer) {
          digitalWrite(relay1Pin, LOW);  // fonte desligada
        }

          //fill_solid(leds, NUM_LEDS, CRGB(255,0,0));
          //FastLED.show();

  }



 // isadoraOutput();
  
  updateDisplay();



  delay(25);
}





////
////
////  F U N C T I O N S 
////
////




void isadoraOutput() {

  //ISADORA  
      
      for (int i=0; i<8; i++) {
        Serial.print(i+1,DEC); // Canal 1 do Isadora
        Serial.print(octoSensor[i]); // Envia sensor 1   
        Serial.println(); //Send a value to eom

      }
      
      /*
      Serial.print(1,DEC); // Canal 1 do Isadora
      Serial.print(octoSensor[0]); // Envia sensor 1   
      Serial.println(); //Send a value to eom

      Serial.print(2,DEC); // Canal 2 do Isadora
      Serial.print(octoSensor[1]); // Envia sensor 2
      Serial.println(); //Send a value to eom
      
      Serial.print(3,DEC); // Canal 2 do Isadora
      Serial.print(octoSensor[2]); // Envia sensor 2     
      Serial.println(); //Send a value to eom

      
      Serial.print(4,DEC); // Canal 2 do Isadora
      Serial.print(octoSensor[3]); // Envia sensor 2
      Serial.println(); //Send a value to eom
      
      Serial.print(5,DEC); // Canal 2 do Isadora
      Serial.print(octoSensor[4]); // Envia sensor 2
      Serial.println(); //Send a value to eom
      
      Serial.print(6,DEC); // Canal 2 do Isadora
      Serial.print(octoSensor[5]); // Envia sensor 2
      Serial.println(); //Send a value to eom
      
      Serial.print(7,DEC); // Canal 2 do Isadora
      Serial.print(octoSensor[6]); // Envia sensor 2
      Serial.println(); //Send a value to eom
      
      Serial.print(8,DEC); // Canal 2 do Isadora
      Serial.print(octoSensor[7]); // Envia sensor 2
      Serial.println(); //Send a value to eom
*/


}




void updateDisplay(){
  // Display Readings on OLED Display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  //display.print("OCTOsensor READINGS");

  //1
  display.setCursor(0, 55);
  display.print(octoSensor[0]);
  //2
  display.setCursor(110, 55);
  display.print(octoSensor[1]);


// status: 0 = search / 1= right / -1 = left / 2 =detected

  display.setTextSize(2);
  display.setCursor(0, 0);

  if ( status_machine == 0) { 
  display.print("  SEARCH");
    
  display.drawCircle(64, 40, 15, WHITE);
  display.drawTriangle(40 , 25, 40, 55, 10, 40, WHITE);
  display.drawTriangle(88 , 25, 88, 55, 117, 40, WHITE);


    }
  if ( status_machine == 1) { 
  display.print("  RIGHT");
  display.drawCircle(64, 40, 15, WHITE);
  display.drawTriangle(40 , 25, 40, 55, 10, 40, WHITE);
  display.fillTriangle(88 , 25, 88, 55, 117, 40, WHITE);  
    
    }
  if ( status_machine == -1) { 
  display.print("  LEFT");
  display.drawCircle(64, 40, 15, WHITE);
  display.fillTriangle(40 , 25, 40, 55, 10, 40, WHITE);
  display.drawTriangle(88 , 25, 88, 55, 117, 40, WHITE);  
    
    }
  if ( status_machine == 2) { 
  display.print("  DETECT");
  display.fillCircle(64, 40, 15, WHITE);
  display.fillTriangle(40 , 25, 40, 55, 10, 40, WHITE);
  display.fillTriangle(88 , 25, 88, 55, 117, 40, WHITE);  
    
    ;}

 
  /*
  //3
  display.setCursor(0, 40);
  display.print("3: ");
  display.print(octoSensor[2]);
  //4
  display.setCursor(0, 50);
  display.print("4: ");
  display.print(octoSensor[3]);
  //5
  display.setCursor(64, 20);
  display.print("5: ");
  display.print(octoSensor[4]);
  //6
  display.setCursor(64, 30);
  display.print("6: ");
  display.print(octoSensor[5]);
  //7
  display.setCursor(64, 40);
  display.print("7: ");
  display.print(octoSensor[6]);
  //8
  display.setCursor(64, 50);
  display.print("8: ");
  display.print(octoSensor[7]);

  */

  display.display();
  

}