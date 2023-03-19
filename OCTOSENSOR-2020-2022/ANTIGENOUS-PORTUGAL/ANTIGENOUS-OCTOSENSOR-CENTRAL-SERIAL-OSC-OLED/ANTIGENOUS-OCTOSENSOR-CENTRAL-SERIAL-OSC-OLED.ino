/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-two-way-communication-esp32/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <Wire.h>        // Wire library >> USED FOR TF-LUNA SENSOR AND OLED DISPLAY

////

// ARRAY WITH 8 SENSORS READINGS

int octoSensor[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int previousOctoSensor[8] = {1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023};



// OSC (serial for pureData & Isadora communication)
// https://github.com/CNMAT/OSC/


#include <OSCMessage.h>
#include <OSCBoards.h>
/*
Make an OSC message and send it over serial
 */

 #ifdef BOARD_HAS_USB_SERIAL
#include <SLIPEncodedUSBSerial.h>
SLIPEncodedUSBSerial SLIPSerial( thisBoardsSerialUSB );
#else
#include <SLIPEncodedSerial.h>
 SLIPEncodedSerial SLIPSerial(Serial); // Change to Serial1 or Serial2 etc. for boards with multiple serial ports that don’t have Serial
#endif


////

//  ESPNOW
#include <esp_now.h>
#include <WiFi.h>

// OLED DISPLAY
#//include <Wire.h>
//#include <Adafruit_Sensor.h>
//#include <Adafruit_BME280.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


// ************************************************************
// REPLACE WITH THE MAC Address of your receiver 
//uint8_t broadcastAddress[] = {0xC4, 0x4F, 0x33, 0x3E, 0xCC, 0x21};
uint8_t broadcastAddress[] = {0x24, 0x62, 0xAB, 0xDD, 0x36, 0x78};
//24:62:AB:DD:36:78

// ************************************************************


// Define variables to store BME280 readings to be sent
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
struct_message OCTOReadings;        //OctoSensorReadings;

// Create a struct_message to hold incoming sensor readings
struct_message incomingReadings;





//   TTTTT    X   X
//     T       X X
//     T        X
//     T       X X
//     T      X   X

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  //Serial.print("\r\nLast Packet Send Status:\t");
  //Serial.println(status == ESP_NOW_SEND_SUCCESS ? "TX OK" : "TX Fail");
  if (status ==0){
    success = "TX OK";
  }
  else{
    success = "TX Fail";
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
  incomingCommand = incomingReadings.comm;   //COMMAND
  incomingNode = incomingReadings.node;      //NODE
  incomingValue  = incomingReadings.value;   //VALUE

/*
  Serial.print("COMMAND: ");
  Serial.println(incomingCommand);
    Serial.print("NODE: ");
  Serial.println(incomingNode);
    Serial.print("VALUE: ");
  Serial.println(incomingValue);
 */ 

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
  Serial.begin(9600);

  // OLED
    Wire.begin();           // Initalize Wire 
    delay(100);

  // Init OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

    Serial.println("OLED");


  display.display();
  delay(500); // Pause for 2 seconds

// ******************************************************* ESPNOW COMM SETUP

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
  esp_now_peer_info_t peerInfo;
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


// OSC Serial
  //begin SLIPSerial just like Serial
  SLIPSerial.begin(9600);   // set this as high as you can reliably run on your platform


}




// *******************************************************
//
//                    L  O   O   P
//
// *******************************************************

 
void loop() {
  getSyncMillis();
 
  // Set values to send
  OCTOReadings.comm = octoCommand;
  OCTOReadings.node = octoNode;
  OCTOReadings.value = octoValue;


  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &OCTOReadings, sizeof(OCTOReadings));
   
  if (result == ESP_OK) {
    //Serial.println("Sent with success");
  }
  else {
    //Serial.println("Error sending the data");
  }

  
  updateDisplay();
  
 //updateSensorsBarGraph();

/*

     //ISADORA  
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



/////////OSC > pureData



            //sensor 1
      
      OSCMessage msg1("/sensor1");
      msg1.add(octoSensor[0]);
      
      SLIPSerial.beginPacket();  
      msg1.send(SLIPSerial); // send the bytes to the SLIP stream
      SLIPSerial.endPacket(); // mark the end of the OSC Packet
      msg1.empty(); // free space occupied by message
 


            //sensor 2
      
      OSCMessage msg2("/sensor2");
      msg2.add(octoSensor[1]);
      
      SLIPSerial.beginPacket();  
      msg2.send(SLIPSerial); // send the bytes to the SLIP stream
      SLIPSerial.endPacket(); // mark the end of the OSC Packet
      msg2.empty(); // free space occupied by message
 


            //sensor 3
      
      OSCMessage msg3("/sensor3");
      msg3.add(octoSensor[2]);
      
      SLIPSerial.beginPacket();  
      msg3.send(SLIPSerial); // send the bytes to the SLIP stream
      SLIPSerial.endPacket(); // mark the end of the OSC Packet
      msg3.empty(); // free space occupied by message
 


            //sensor 4
      
      OSCMessage msg4("/sensor4");
      msg4.add(octoSensor[3]);
      
      SLIPSerial.beginPacket();  
      msg4.send(SLIPSerial); // send the bytes to the SLIP stream
      SLIPSerial.endPacket(); // mark the end of the OSC Packet
      msg4.empty(); // free space occupied by message
 


            //sensor 5
      
      OSCMessage msg5("/sensor5");
      msg5.add(octoSensor[4]);
      
      SLIPSerial.beginPacket();  
      msg5.send(SLIPSerial); // send the bytes to the SLIP stream
      SLIPSerial.endPacket(); // mark the end of the OSC Packet
      msg5.empty(); // free space occupied by message
 


            //sensor 6
      
      OSCMessage msg6("/sensor6");
      msg6.add(octoSensor[5]);
      
      SLIPSerial.beginPacket();  
      msg6.send(SLIPSerial); // send the bytes to the SLIP stream
      SLIPSerial.endPacket(); // mark the end of the OSC Packet
      msg6.empty(); // free space occupied by message
 

      
            //sensor 7
      
      OSCMessage msg7("/sensor7");
      msg7.add(octoSensor[6]);
      
      SLIPSerial.beginPacket();  
      msg7.send(SLIPSerial); // send the bytes to the SLIP stream
      SLIPSerial.endPacket(); // mark the end of the OSC Packet
      msg7.empty(); // free space occupied by message
 

      
            //sensor 8
      
      OSCMessage msg8("/sensor8");
      msg8.add(octoSensor[7]);
      
      SLIPSerial.beginPacket();  
      msg8.send(SLIPSerial); // send the bytes to the SLIP stream
      SLIPSerial.endPacket(); // mark the end of the OSC Packet
      msg8.empty(); // free space occupied by message
 


}





////
////
////  F U N C T I O N S 
////
////


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


void getSyncMillis(){

  octoCommand = 10;   // comando 10 = send sync
  octoNode = 10;
  octoValue = millis()/100;
  
}
