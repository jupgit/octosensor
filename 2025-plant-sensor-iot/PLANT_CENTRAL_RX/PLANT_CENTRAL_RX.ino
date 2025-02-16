///////////////////////////////////////////////////////////
//

//   
//   
///////////////////////////////////////////////////////////

/// 
///     C E N T R A L 
///

// ARRAY WITH 8 SENSORS READINGS

//int octoSensor[8] = {0, 0, 0, 0, 0, 0, 0, 0};
//int previousOctoSensor[8] = {1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023};

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
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; 



// Define variables to store readings to be sent
//int octoCommand;
//int octoNode;
//int octoValue;

// Define variables to store incoming readings
float incoming_plantTemp;
float incoming_plantUmid;
int incoming_plantMoisture1;

// Variable to store if sending data was successful
String success;


//Structure example to send data
//Must match the receiver structure

typedef struct struct_message {
  float temp;
  float umid;
  int moisture1;
} struct_message;


// Create a struct_message called BME280Readings to hold sensor readings
struct_message plantReadings;


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
void OnDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *incomingData, int len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  //Serial.print("Bytes received: ");
  //Serial.println(len);
  incoming_plantTemp = incomingReadings.temp;
  incoming_plantUmid = incomingReadings.umid;
  incoming_plantMoisture1 = incomingReadings.moisture1;


//////////////////////////////////////////////////////////////////////////////////////
// REMOTE SENSOR VALUE into its position in Array 
//   octoSensor[incomingNode-1] = incomingValue;
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
  
  //getReadings();
 
 /*
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
  
    */

  updateDisplay();

  delay(25);
}





////
////
////  F U N C T I O N S 
////
////



void updateDisplay(){
  // Display Readings on OLED Display
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 15);
  //display.print("plantMonitor");

  //display.setCursor(0, 30);
  display.print("T: "); display.println(incoming_plantTemp);
  display.print("U: "); display.println(incoming_plantUmid);
  display.print("M: "); display.println(incoming_plantMoisture1);


  display.display();
  

}