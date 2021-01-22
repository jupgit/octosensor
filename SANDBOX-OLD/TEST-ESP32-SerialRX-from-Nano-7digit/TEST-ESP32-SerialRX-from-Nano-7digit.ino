// SERIAL1 DATA FROM NANO
#define RXD2 16
#define TXD2 17

const byte numChars = 32;
char receivedChars[numChars];   // an array to store the received data
boolean newData = false;

int distanceCM = 0;

// 7 DIGIT DISPLAY
#include <TM1637TinyDisplay.h>

// Module connection pins (Digital Pins)
#define CLK 4
#define DIO 5
TM1637TinyDisplay display(CLK, DIO);





void setup() {
  
    Serial.begin(115200);
    Serial.println("<Arduino is ready>");
    
    //Serial 2 do ESP32 que recebe do arduino Nano
    Serial1.begin(115200, SERIAL_8N1, RXD2, TXD2);


  // 7 DIGIT DISPLAY

  display.setBrightness(BRIGHT_HIGH);
  display.showString("Test");
  delay(1000);
  display.clear();
}


/// LOOP //////////////////////////////////////////////////////////////////////////////////////

void loop() { 

    // RECEIVE SENSOR DATA FROM NANO
    recvWithEndMarker();
    readSensorFromNano();

    display.showNumber(distanceCM, false);          // Expect: 0000



    delay(10);
}









////////////////////////////////////
////
////     F U N C T I O N S
////
////////////////////////////////////


/// NANO + SENSOR TO ESP32 SERIAL COMMUNICATION FUNCTIONS

/// Used to receive sensor data over Seria11 from Arduino Nano
void recvWithEndMarker() {
    static byte ndx = 0;
    char endMarker = '\n';
    char rc;
   
    while (Serial1.available() > 0 && newData == false) {
        rc = Serial1.read();

        if (rc != endMarker) {
            receivedChars[ndx] = rc;
            ndx++;
            if (ndx >= numChars) {
                ndx = numChars - 1;
            }
        }
        else {
            receivedChars[ndx] = '\0'; // terminate the string
            ndx = 0;
            newData = true;
        }
    }
}

/// Use to show sensor data over Seria11 from Arduino Nano
void showNewData() {
    if (newData == true) {
        Serial.print("This just in ... ");
        Serial.println(receivedChars);
        newData = false;
    }


}
void readSensorFromNano() {

      if (newData == true) {
        //Serial.println(receivedChars);
        distanceCM = atoi(receivedChars);
        newData = false;
        Serial.println(distanceCM);

    }


}

/// END OF: NANO + SENSOR TO ESP32 SERIAL COMMUNICATION FUNCTIONS




    
