///////////////////////////////////////////////////////////
//

//   
//   
///////////////////////////////////////////////////////////

/// 
///     C E N T R A L 
///


#include <esp_now.h>
#include <WiFi.h>

#include <Wire.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


// Define variables to store incoming readings
int incoming_plantTemp;
int incoming_plantUmid;
int incoming_plantMoisture1;

// Variable to store if sending data was successful
String success;


// Create a struct_message called BME280Readings to hold sensor readings
struct_message plantReadings;


// Create a struct_message to hold incoming sensor readings
struct_message incomingReadings;

esp_now_peer_info_t peerInfo;


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
  


  updateDisplay();

  delay(25);
}




void updateDisplay(){
  // Display Readings on OLED Display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("OCTOsensor READINGS");


  display.display();
  

}