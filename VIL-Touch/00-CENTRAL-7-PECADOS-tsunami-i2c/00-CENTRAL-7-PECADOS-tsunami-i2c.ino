
//// PONTOS G
//// ver set.2019 (Braga)
/////

#include <Tsunami.h>          // Include the Tsunami library header
#include <AltSoftSerial.h>    // Optional for using AltSoftSerial
#include <Wire.h>             // Arduino I2C network


////////////////////
// COMM SETTINGS (I2C)
///////////////////

// Program: I2C master reader template for multi-node Arduino I2C network
#define PAYLOAD_SIZE 2 // how many bytes to expect from each I2C salve node
#define NODE_MAX 7 // maximum number of slave nodes (I2C addresses) to probe
#define START_NODE 0 // The starting I2C address of slave nodes
#define NODE_READ_DELAY 100 // Some delay between I2C node reads

int nodePayload[PAYLOAD_SIZE];


////////////////////
// RELAY SHIELD CONFIG
///////////////////

int relay1Pin = 10;
int relay2Pin = 11;
int relay3Pin = 12;
int relay4Pin = 13;


////////////////////
// TSUNAMI INIT
///////////////////

Tsunami tsunami;                // Our Tsunami object

int track = 0 ;
int lasttrack = 0 ;
int mutetrack = 0;



// ****** C H A C R A S    S E T U P   ******

// Each chacra will play 4 audios (one at a time) acording to 4 ranges of proximity
// Each chacra can trigger a special audio, defined by a min and max value
// Each chacra can interfere with volume of the drone sound
// Each chacra can interfere with the overall pitch of the instalation


// CHACRA 1 Setup ***********************

// Proximity ranges
int chacra1_range1_min = 1 ;        // start value for the 1th range >> silence
int chacra1_range2_min = 4 ;        // start value for the 2st range >> plays 101_chacra1_range1.wav
int chacra1_range3_min = 12 ;        // start value for the 3nd range >> plays 102_chacra1_range2.wav
int chacra1_range4_min = 20 ;        // start value for the 4rd range >> plays 103_chacra1_range3.wav
int chacra1_range5_min = 33 ;        // start value for the 5th range >> plays 104_chacra1_range4.wav
int chacra1_range6_min = 60 ;        // start value for the 6th range >> silence

int chacra1_range = 0  ;             // 0=silence / 1,2,3,4 = four ranges / 5 = silence
int last_chacra1_range = 0 ;



// CHACRA 2 Setup ***********************

// Proximity ranges
int chacra2_range1_min = 1 ;        // start value for the 1th range >> silence
int chacra2_range2_min = 4 ;        // start value for the 2st range >> plays 101_chacra1_range1.wav
int chacra2_range3_min = 12 ;        // start value for the 3nd range >> plays 102_chacra1_range2.wav
int chacra2_range4_min = 20 ;        // start value for the 4rd range >> plays 103_chacra1_range3.wav
int chacra2_range5_min = 33 ;        // start value for the 5th range >> plays 104_chacra1_range4.wav
int chacra2_range6_min = 60 ;        // start value for the 6th range >> silence

int chacra2_range = 0  ;             // 0=silence / 1,2,3,4 = four ranges / 5 = silence
int last_chacra2_range = 0 ;



// CHACRA 3 Setup ***********************

// Proximity ranges
int chacra3_range1_min = 1 ;        // start value for the 1th range >> silence
int chacra3_range2_min = 4 ;        // start value for the 2st range >> plays 101_chacra1_range1.wav
int chacra3_range3_min = 12 ;        // start value for the 3nd range >> plays 102_chacra1_range2.wav
int chacra3_range4_min = 20 ;        // start value for the 4rd range >> plays 103_chacra1_range3.wav
int chacra3_range5_min = 33 ;        // start value for the 5th range >> plays 104_chacra1_range4.wav
int chacra3_range6_min = 60 ;        // start value for the 6th range >> silence

int chacra3_range = 0  ;             // 0=silence / 1,2,3,4 = four ranges / 5 = silence
int last_chacra3_range = 0 ;


// CHACRA 4 Setup ***********************

// Proximity ranges
int chacra4_range1_min = 1 ;        // start value for the 1th range >> silence
int chacra4_range2_min = 4 ;        // start value for the 2st range >> plays 101_chacra1_range1.wav
int chacra4_range3_min = 12 ;        // start value for the 3nd range >> plays 102_chacra1_range2.wav
int chacra4_range4_min = 20 ;        // start value for the 4rd range >> plays 103_chacra1_range3.wav
int chacra4_range5_min = 33 ;        // start value for the 5th range >> plays 104_chacra1_range4.wav
int chacra4_range6_min = 60 ;        // start value for the 6th range >> silence

int chacra4_range = 0  ;             // 0=silence / 1,2,3,4 = four ranges / 5 = silence
int last_chacra4_range = 0 ;


// CHACRA 5 Setup ***********************

// Proximity ranges
int chacra5_range1_min = 1 ;        // start value for the 1th range >> silence
int chacra5_range2_min = 4 ;        // start value for the 2st range >> plays 101_chacra1_range1.wav
int chacra5_range3_min = 12 ;        // start value for the 3nd range >> plays 102_chacra1_range2.wav
int chacra5_range4_min = 20 ;        // start value for the 4rd range >> plays 103_chacra1_range3.wav
int chacra5_range5_min = 33 ;        // start value for the 5th range >> plays 104_chacra1_range4.wav
int chacra5_range6_min = 60 ;        // start value for the 6th range >> silence

int chacra5_range = 0  ;             // 0=silence / 1,2,3,4 = four ranges / 5 = silence
int last_chacra5_range = 0 ;



// CHACRA 6 Setup ***********************

// Proximity ranges
int chacra6_range1_min = 1 ;        // start value for the 1th range >> silence
int chacra6_range2_min = 4 ;        // start value for the 2st range >> plays 101_chacra1_range1.wav
int chacra6_range3_min = 12 ;        // start value for the 3nd range >> plays 102_chacra1_range2.wav
int chacra6_range4_min = 20 ;        // start value for the 4rd range >> plays 103_chacra1_range3.wav
int chacra6_range5_min = 33 ;        // start value for the 5th range >> plays 104_chacra1_range4.wav
int chacra6_range6_min = 60 ;        // start value for the 6th range >> silence

int chacra6_range = 0  ;             // 0=silence / 1,2,3,4 = four ranges / 5 = silence
int last_chacra6_range = 0 ;



// CHACRA 7 Setup ***********************

// Proximity ranges
int chacra7_range1_min = 1 ;        // start value for the 1th range >> silence
int chacra7_range2_min = 4 ;        // start value for the 2st range >> plays 101_chacra1_range1.wav
int chacra7_range3_min = 12 ;        // start value for the 3nd range >> plays 102_chacra1_range2.wav
int chacra7_range4_min = 20 ;        // start value for the 4rd range >> plays 103_chacra1_range3.wav
int chacra7_range5_min = 33 ;        // start value for the 5th range >> plays 104_chacra1_range4.wav
int chacra7_range6_min = 60 ;        // start value for the 6th range >> silence

int chacra7_range = 0  ;             // 0=silence / 1,2,3,4 = four ranges / 5 = silence
int last_chacra7_range = 0 ;




//// ZEROS SENSOR READINGS
int sensor1_read = 0 ;
int sensor2_read = 0 ;
int sensor3_read = 0 ;
int sensor4_read = 0 ;
int sensor5_read = 0 ;
int sensor6_read = 0 ;
int sensor7_read = 0 ;




//// S E T U P  /////

void setup()
{




/// I2C comm setup verbose
  Serial.begin(9600);  
  Serial.println("MASTER READER NODE");
  Serial.print("Maximum Slave Nodes: ");
  Serial.println(NODE_MAX);
  Serial.print("Payload size: ");
  Serial.println(PAYLOAD_SIZE);
  Serial.println("***********************");

  delay(300);



/// I2C comm init
  Wire.begin();        // Activate I2C link

    delay(300);

  // Serial monitor
  Serial.begin(9600);







// TSUNAMI ( AUDIO )
 
  // We should wait for the Tsunami to finish reset before trying to send
  // commands.
  delay(1000);

  // Tsunami startup at 57600
  tsunami.start();
  delay(10);
  
  // Send a stop-all command and reset the sample-rate offset, in case we have
  //  reset while the Tsunami was already playing.
  tsunami.stopAllTracks();
  tsunami.samplerateOffset(0, 0);
  
  // Enable track reporting from the Tsunami
  tsunami.setReporting(true);
  
  // Allow time for the Tsunami to respond with the version string and
  //  number of tracks.
  delay(100); 


// ZERAR OS GANHOS
// tsunami.trackGain(101, 0);             // Preset Track 101 gain to -40dB
//tsunami.trackGain(102, -40);             // Preset Track 101 gain to -40dB
// tsunami.trackGain(103, -40);             // Preset Track 101 gain to -40dB
// tsunami.trackGain(104, -40);             // Preset Track 101 gain to -40dB


/// Initialize RELAY OUTPUTs
  pinMode(relay1Pin, OUTPUT);
  pinMode(relay2Pin, OUTPUT);  
  pinMode(relay3Pin, OUTPUT);  
  pinMode(relay4Pin, OUTPUT);


/// LOOP DA TRILHA BG
tsunami.trackLoop(001, 1); 
tsunami.trackPlayPoly(001, 0, true); 
tsunami.trackGain(001, -15);

}





//////////////////////////////////////

//    L       OOO     OOO    PPPP
//    L      O   O   O   O   P   P
//    L      O   O   O   O   P   P
//    L      O   O   O   O   PPPP
//    L      O   O   O   O   P
//    LLLLL   OOO     OOO    P

//////////////////////////////////////


void loop()
{
  

//////////////////////////////
//// C H A K R A  == 1 ==  ///
//////////////////////////////


/// READING CHAKRA 1 (comm)

      int nodeAddress = 1;   // chakra number
      
      Wire.requestFrom(nodeAddress, 2);    // request data from node#
      if(Wire.available() == 2) {

            int chakraReceivedID = Wire.read(); 
            sensor1_read = Wire.read(); 

      }
      
      delay(NODE_READ_DELAY);

/// END READING CHAKRA 1


//// ************************************************************************
//// RANGE SELECTORS (converts sensor reads to ranges for each sensor)
//// ************************************************************************
  
// SENSOR 1
while (1){
  if (sensor1_read <= chacra1_range1_min){ chacra1_range = 1; break;}
  if (sensor1_read <= chacra1_range2_min){ chacra1_range = 2; break;}
  if (sensor1_read <= chacra1_range3_min){ chacra1_range = 3; break;}
  if (sensor1_read <= chacra1_range4_min){ chacra1_range = 4; break;}
  if (sensor1_read <= chacra1_range5_min){ chacra1_range = 5; break;}
  if (sensor1_read <= chacra1_range6_min){ chacra1_range = 6; break;} 
  break;
}


//// ************************************************************************
//// ACTIONS SHOOTING !!!
//// (if range changes, shoot an action: sound and/or Relays
//// ************************************************************************

// CHACRA 1
if(chacra1_range!= last_chacra1_range){


  track = 100 + (chacra1_range-1) ;
  Serial.println(track);

  lasttrack = 100 + last_chacra1_range ;
   if (track == 105) {  
  for (int mutetrack=101; mutetrack <= 105; mutetrack++) {
    tsunami.trackStop(mutetrack);    
  } 
   }
  tsunami.trackPlayPoly(track, 0, true);     // Start Track 1

  last_chacra1_range = chacra1_range;
}


/*

// Relay Chacra 1
  if(sensor1_read >= chacra1_trigger_min && sensor1_read <= chacra1_trigger_max) {
   digitalWrite(relay1Pin, HIGH);
   digitalWrite(relay2Pin, HIGH);   
  } else {
   digitalWrite(relay1Pin, LOW);
   digitalWrite(relay2Pin, LOW);  
  }
*/


//////////////////////////////
//// C H A K R A  == 2 ==  ///
//////////////////////////////


/// READING CHAKRA 2 (comm)

      nodeAddress = 2;   // chakra number
      
      Wire.requestFrom(nodeAddress, 2);    // request data from node#
      if(Wire.available() == 2) {

            int chakraReceivedID = Wire.read(); 
            sensor2_read = Wire.read(); 

      }
      
      delay(NODE_READ_DELAY);

/// END READING CHAKRA 2


//// ************************************************************************
//// RANGE SELECTORS (converts sensor reads to ranges for each sensor)
//// ************************************************************************
  
// SENSOR 2
while (1){
  if (sensor2_read <= chacra2_range1_min){ chacra2_range = 1; break;}
  if (sensor2_read <= chacra2_range2_min){ chacra2_range = 2; break;}
  if (sensor2_read <= chacra2_range3_min){ chacra2_range = 3; break;}
  if (sensor2_read <= chacra2_range4_min){ chacra2_range = 4; break;}
  if (sensor2_read <= chacra2_range5_min){ chacra2_range = 5; break;}
  if (sensor2_read <= chacra2_range6_min){ chacra2_range = 6; break;} 
  break;
}


//// ************************************************************************
//// ACTIONS SHOOTING !!!
//// (if range changes, shoot an action: sound and/or Relays
//// ************************************************************************

// CHACRA 2
if(chacra2_range!= last_chacra2_range){

  track = 200 + (chacra2_range-1) ;
  Serial.println(track);

  lasttrack = 200 + last_chacra2_range ;
   if (track == 205) {  
  for (int mutetrack=201; mutetrack <= 205; mutetrack++) {
    tsunami.trackStop(mutetrack);    
  } 
  }
  tsunami.trackPlayPoly(track, 0, true);     // Start Track

  last_chacra2_range = chacra1_range;
}



//////////////////////////////
//// C H A K R A  == 3 ==  ///
//////////////////////////////


/// READING CHAKRA 3 (comm)

      nodeAddress = 3;   // chakra number
      
      Wire.requestFrom(nodeAddress, 2);    // request data from node#
      if(Wire.available() == 2) {

            int chakraReceivedID = Wire.read(); 
            sensor3_read = Wire.read(); 

      }
      
      delay(NODE_READ_DELAY);

/// END READING CHAKRA 3


//// ************************************************************************
//// RANGE SELECTORS (converts sensor reads to ranges for each sensor)
//// ************************************************************************
  
// SENSOR 3
while (1){
  if (sensor3_read <= chacra3_range1_min){ chacra3_range = 1; break;}
  if (sensor3_read <= chacra3_range2_min){ chacra3_range = 2; break;}
  if (sensor3_read <= chacra3_range3_min){ chacra3_range = 3; break;}
  if (sensor3_read <= chacra3_range4_min){ chacra3_range = 4; break;}
  if (sensor3_read <= chacra3_range5_min){ chacra3_range = 5; break;}
  if (sensor3_read <= chacra3_range6_min){ chacra3_range = 6; break;} 
  break;
}


//// ************************************************************************
//// ACTIONS SHOOTING !!!
//// (if range changes, shoot an action: sound and/or Relays
//// ************************************************************************

// CHACRA 3
if(chacra3_range!= last_chacra3_range){

  track = 300 + (chacra3_range-1) ;
  Serial.println(track);

  lasttrack = 300 + last_chacra3_range ;

  if (track == 305) {    
    for (int mutetrack=301; mutetrack <= 305; mutetrack++) {
    tsunami.trackStop(mutetrack);
    }
  }
  
  tsunami.trackPlayPoly(track, 0, true);     // Start Track 1


  last_chacra3_range = chacra3_range;
}



//////////////////////////////
//// C H A K R A  == 4 ==  ///
//////////////////////////////


/// READING CHAKRA 4 (comm)

      nodeAddress = 4;   // chakra number
      
      Wire.requestFrom(nodeAddress, 2);    // request data from node#
      if(Wire.available() == 2) {

            int chakraReceivedID = Wire.read(); 
            sensor4_read = Wire.read(); 

      }
      
      delay(NODE_READ_DELAY);

/// END READING CHAKRA 4


//// ************************************************************************
//// RANGE SELECTORS (converts sensor reads to ranges for each sensor)
//// ************************************************************************
  
// SENSOR 4
while (1){
  if (sensor4_read <= chacra4_range1_min){ chacra4_range = 1; break;}
  if (sensor4_read <= chacra4_range2_min){ chacra4_range = 2; break;}
  if (sensor4_read <= chacra4_range3_min){ chacra4_range = 3; break;}
  if (sensor4_read <= chacra4_range4_min){ chacra4_range = 4; break;}
  if (sensor4_read <= chacra4_range5_min){ chacra4_range = 5; break;}
  if (sensor4_read <= chacra4_range6_min){ chacra4_range = 6; break;} 
  break;
}


//// ************************************************************************
//// ACTIONS SHOOTING !!!
//// (if range changes, shoot an action: sound and/or Relays
//// ************************************************************************

// CHACRA 4
if(chacra4_range!= last_chacra4_range){

  track = 400 + (chacra4_range-1) ;
  Serial.println(track);

  lasttrack = 400 + last_chacra4_range ;

  if (track == 405) {  
  for (int mutetrack=401; mutetrack <= 405; mutetrack++) {
    tsunami.trackStop(mutetrack);    
  } 
  }
  tsunami.trackPlayPoly(track, 0, true);     // Start Track

  last_chacra4_range = chacra4_range;
}



//////////////////////////////
//// C H A K R A  == 5 ==  ///
//////////////////////////////


/// READING CHAKRA 5 (comm)

      nodeAddress = 5;   // chakra number
      
      Wire.requestFrom(nodeAddress, 2);    // request data from node#
      if(Wire.available() == 2) {

            int chakraReceivedID = Wire.read(); 
            sensor5_read = Wire.read(); 

      }
      
      delay(NODE_READ_DELAY);

/// END READING CHAKRA 5


//// ************************************************************************
//// RANGE SELECTORS (converts sensor reads to ranges for each sensor)
//// ************************************************************************
  
// SENSOR 5
while (1){
  if (sensor5_read <= chacra5_range1_min){ chacra5_range = 1; break;}
  if (sensor5_read <= chacra5_range2_min){ chacra5_range = 2; break;}
  if (sensor5_read <= chacra5_range3_min){ chacra5_range = 3; break;}
  if (sensor5_read <= chacra5_range4_min){ chacra5_range = 4; break;}
  if (sensor5_read <= chacra5_range5_min){ chacra5_range = 5; break;}
  if (sensor5_read <= chacra5_range6_min){ chacra5_range = 6; break;} 
  break;
}


//// ************************************************************************
//// ACTIONS SHOOTING !!!
//// (if range changes, shoot an action: sound and/or Relays
//// ************************************************************************

// CHACRA 5
if(chacra5_range!= last_chacra5_range){

  track = 500 + (chacra5_range-1) ;
  Serial.println(track);

  lasttrack = 500 + last_chacra5_range ;

 if (track == 505) {  
  for (int mutetrack=501; mutetrack <= 505; mutetrack++) {
    tsunami.trackStop(mutetrack);    
  } 
 }
  
  tsunami.trackPlayPoly(track, 0, true);     // Start Track

  last_chacra5_range = chacra5_range;
}



//////////////////////////////
//// C H A K R A  == 6 ==  ///
//////////////////////////////


/// READING CHAKRA 6 (comm)

      nodeAddress = 6;   // chakra number
      
      Wire.requestFrom(nodeAddress, 2);    // request data from node#
      if(Wire.available() == 2) {

            int chakraReceivedID = Wire.read(); 
            sensor6_read = Wire.read(); 

      }
      
      delay(NODE_READ_DELAY);

/// END READING CHAKRA 6


//// ************************************************************************
//// RANGE SELECTORS (converts sensor reads to ranges for each sensor)
//// ************************************************************************
  
// SENSOR 6
while (1){
  if (sensor6_read <= chacra6_range1_min){ chacra6_range = 1; break;}
  if (sensor6_read <= chacra6_range2_min){ chacra6_range = 2; break;}
  if (sensor6_read <= chacra6_range3_min){ chacra6_range = 3; break;}
  if (sensor6_read <= chacra6_range4_min){ chacra6_range = 4; break;}
  if (sensor6_read <= chacra6_range5_min){ chacra6_range = 5; break;}
  if (sensor6_read <= chacra6_range6_min){ chacra6_range = 6; break;} 
  break;
}


//// ************************************************************************
//// ACTIONS SHOOTING !!!
//// (if range changes, shoot an action: sound and/or Relays
//// ************************************************************************

// CHACRA 6
if(chacra6_range!= last_chacra6_range){

  track = 600 + (chacra6_range-1) ;
  Serial.println(track);

  lasttrack = 600 + last_chacra6_range ;

 if (track == 605) {  
  for (int mutetrack=601; mutetrack <= 605; mutetrack++) {
    tsunami.trackStop(mutetrack);    
  } 
 }
  
  tsunami.trackPlayPoly(track, 0, true);     // Start Track


  last_chacra6_range = chacra6_range;
}



//////////////////////////////
//// C H A K R A  == 7 ==  ///
//////////////////////////////


/// READING CHAKRA 7 (comm)

      nodeAddress = 7;   // chakra number
      
      Wire.requestFrom(nodeAddress, 2);    // request data from node#
      if(Wire.available() == 2) {

            int chakraReceivedID = Wire.read(); 
            sensor7_read = Wire.read(); 

      }
      
      delay(NODE_READ_DELAY);

/// END READING CHAKRA 7


//// ************************************************************************
//// RANGE SELECTORS (converts sensor reads to ranges for each sensor)
//// ************************************************************************
  
// SENSOR 7
while (1){
  if (sensor7_read <= chacra7_range1_min){ chacra7_range = 1; break;}
  if (sensor7_read <= chacra7_range2_min){ chacra7_range = 2; break;}
  if (sensor7_read <= chacra7_range3_min){ chacra7_range = 3; break;}
  if (sensor7_read <= chacra7_range4_min){ chacra7_range = 4; break;}
  if (sensor7_read <= chacra7_range5_min){ chacra7_range = 5; break;}
  if (sensor7_read <= chacra7_range6_min){ chacra7_range = 6; break;} 
  break;
}


//// ************************************************************************
//// ACTIONS SHOOTING !!!
//// (if range changes, shoot an action: sound and/or Relays
//// ************************************************************************

// CHACRA 7
if(chacra7_range!= last_chacra7_range){

  track = 700 + (chacra7_range-1) ;
  Serial.println(track);

  lasttrack = 700 + last_chacra7_range ;

 if (track == 705) {  
  for (int mutetrack=701; mutetrack <= 705; mutetrack++) {
    tsunami.trackStop(mutetrack);    
  } 
 }

  
  tsunami.trackPlayPoly(track, 0, true);     // Start Track


  last_chacra7_range = chacra7_range;
}





}








////////////////////
// FUNCTIONS
///////////////////
