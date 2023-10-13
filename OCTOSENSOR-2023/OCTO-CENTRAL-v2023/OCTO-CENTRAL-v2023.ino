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

// ARRAY WITH 8 SENSORS READINGS

int octoSensor[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int previousOctoSensor[8] = {1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023};

#include <esp_now.h>
#include <WiFi.h>

#include <Wire.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

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
}
 






// *******************************************************
//
//                    L  O   O   P
//
// *******************************************************



void loop() {
  
  /// TROCAR POR MILLIS
  getReadings();
 
  // Set values to send
  OCTOReadings.comm = octoCommand;
  OCTOReadings.node = octoNode;
  OCTOReadings.value = octoValue;

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) & OCTOReadings, sizeof(OCTOReadings));
   
  if (result == ESP_OK) {
    //Serial.println("Sent with success");
  }
  else {
    //Serial.println("Error sending the data");
  }
  
  isadoraOutput();
  
  updateDisplay();



  delay(25);
}





////
////
////  F U N C T I O N S 
////
////



void getReadings(){
  octoCommand = 1;
  octoNode = 4;
  octoValue = int(millis()/1000);
  octoValue = random(200,1000);
}


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
  display.print("OCTOsensor READINGS");

  //1
  display.setCursor(0, 20);
  display.print("1: ");
  display.print(octoSensor[0]);
  //2
  display.setCursor(0, 30);
  display.print("2: ");
  display.print(octoSensor[1]);
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

  

  display.display();
  

}