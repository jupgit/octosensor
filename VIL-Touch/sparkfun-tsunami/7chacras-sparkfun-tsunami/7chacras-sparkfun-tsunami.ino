// ****************************************************************************
//       Sketch: 7 "chacras" sensors TO Tsunami and 2x7 LED strips  
// Date Created: 01jul2018
//
//     Comments: 
//  Programmers: Pedrinho, Juliano Prado, Felipe Bombachini
// ****************************************************************************

#include <Tsunami.h>            // Include the Tsunami library header
#include <AltSoftSerial.h>    // Optional for using AltSoftSerial

//#define LED 13                  // our LED

Tsunami tsunami;                // Our Tsunami object



// ****** C H A C R A S    S E T U P   ******

// Each chacra will play 4 audios (one at a time) according to 4 ranges of proximity
// Each chacra can trigger a special audio, defined by a min and max value
// Each chacra can interfere with volume of the drone sound
// Each chacra can interfere with the overall pitch of the instalation


// CHACRA 1 Setup ***********************

// Proximity ranges
int chacra1_range1_min = 500 ;        // min value for the 1st range >> plays 101_chacra1_range1.wav
int chacra1_range2_min = 700 ;        // min value for the 2nd range >> plays 102_chacra1_range2.wav
int chacra1_range3_min = 850 ;        // min value for the 3rd range >> plays 103_chacra1_range3.wav
int chacra1_range4_min = 950 ;        // min value for the 4th range >> plays 104_chacra1_range4.wav
int chacra1_end_min = 980 ;           // min value for the 5th range >> silence

int chacra1_range = 0  ;             // 0=silence / 1,2,3,4 = four ranges / 5 = silence
int last_chacra1_range = 0 ;


// Trigger range
int chacra1_trigger_min = 0 ;       // min value to trigger the special sound >> 110_chacra1_trigger.wav
int chacra1_trigger_max = 0 ;       // min value to trigger the special sound

// Drone Interference
int chacra1_drone_interference = 0 ;        // from -100 to 100 is the amount of interference of the sensor on the drone's volume

// Pitch Interference
int chacra1_pitch_interference = 0 ;      // from -100 to 100 (2 octaves pitch) is the amount of interference of the sensor on the overall pitch


// CHACRA 2 Setup ***********************

// Proximity ranges
int chacra2_range1_min = 0 ;        // min value for the 1st range >> plays 201_chacra1_range1.wav
int chacra2_range2_min = 0 ;        // min value for the 2nd range >> plays 202_chacra1_range2.wav
int chacra2_range3_min = 0 ;        // min value for the 3rd range >> plays 203_chacra1_range3.wav
int chacra2_range4_min = 0 ;        // min value for the 4th range >> plays 204_chacra1_range4.wav
int chacra2_end_min = 0 ;           // min value for the 5th range >> silence

// Trigger range
int chacra2_trigger_min = 0 ;       // min value to trigger the special sound >> 210_chacra1_trigger.wav
int chacra2_trigger_max = 0 ;       // min value to trigger the special sound

// Drone Interference
int chacra2_drone_interference = 0 ;        // from -100 to 100 is the amount of interference of the sensor on the drone's volume

// Pitch Interference
int chacra2_pitch_interference = 0 ;      // from -100 to 100 (2 octaves pitch) is the amount of interference of the sensor on the overall pitch



// CHACRA 3 Setup ***********************

// Proximity ranges
int chacra3_range1_min = 0 ;        // min value for the 1st range >> plays 301_chacra1_range1.wav
int chacra3_range2_min = 0 ;        // min value for the 2nd range >> plays 302_chacra1_range2.wav
int chacra3_range3_min = 0 ;        // min value for the 3rd range >> plays 303_chacra1_range3.wav
int chacra3_range4_min = 0 ;        // min value for the 4th range >> plays 304_chacra1_range4.wav
int chacra3_end_min = 0 ;           // min value for the 5th range >> silence

// Trigger range
int chacra3_trigger_min = 0 ;       // min value to trigger the special sound >> 310_chacra1_trigger.wav
int chacra3_trigger_max = 0 ;       // min value to trigger the special sound

// Drone Interference
int chacra3_drone_interference = 0 ;        // from -100 to 100 is the amount of interference of the sensor on the drone's volume

// Pitch Interference
int chacra3_pitch_interference = 0 ;      // from -100 to 100 (2 octaves pitch) is the amount of interference of the sensor on the overall pitch


// CHACRA 4 Setup ***********************

// Proximity ranges
int chacra4_range1_min = 0 ;        // min value for the 1st range >> plays 401_chacra1_range1.wav
int chacra4_range2_min = 0 ;        // min value for the 2nd range >> plays 402_chacra1_range2.wav
int chacra4_range3_min = 0 ;        // min value for the 3rd range >> plays 403_chacra1_range3.wav
int chacra4_range4_min = 0 ;        // min value for the 4th range >> plays 404_chacra1_range4.wav
int chacra4_end_min = 0 ;           // min value for the 5th range >> silence

// Trigger range
int chacra4_trigger_min = 0 ;       // min value to trigger the special sound >> 410_chacra1_trigger.wav
int chacra4_trigger_max = 0 ;       // min value to trigger the special sound

// Drone Interference
int chacra4_drone_interference = 0 ;        // from -100 to 100 is the amount of interference of the sensor on the drone's volume

// Pitch Interference
int chacra4_pitch_interference = 0 ;     // from -100 to 100 (2 octaves pitch) is the amount of interference of the sensor on the overall pitch



// CHACRA 5 Setup ***********************

// Proximity ranges
int chacra5_range1_min = 0 ;        // min value for the 1st range >> plays 501_chacra1_range1.wav
int chacra5_range2_min = 0 ;        // min value for the 2nd range >> plays 502_chacra1_range2.wav
int chacra5_range3_min = 0 ;        // min value for the 3rd range >> plays 503_chacra1_range3.wav
int chacra5_range4_min = 0  ;       // min value for the 4th range >> plays 504_chacra1_range4.wav
int chacra5_end_min = 0 ;           // min value for the 5th range >> silence

// Trigger range
int chacra5_trigger_min = 0 ;      // min value to trigger the special sound >> 510_chacra1_trigger.wav
int chacra5_trigger_max = 0 ;       // min value to trigger the special sound

// Drone Interference
int chacra5_drone_interference = 0 ;        // from -100 to 100 is the amount of interference of the sensor on the drone's volume

// Pitch Interference
int chacra5_pitch_interference = 0 ;      // from -100 to 100 (2 octaves pitch) is the amount of interference of the sensor on the overall pitch



// Chacra 6 Setup ***********************

// Proximity ranges
int chacra6_range1_min = 0 ;        // min value for the 1st range >> plays 601_chacra1_range1.wav
int chacra6_range2_min = 0 ;        // min value for the 2nd range >> plays 602_chacra1_range2.wav
int chacra6_range3_min = 0 ;        // min value for the 3rd range >> plays 603_chacra1_range3.wav
int chacra6_range4_min = 0  ;       // min value for the 4th range >> plays 604_chacra1_range4.wav
int chacra6_end_min = 0 ;           // min value for the 5th range >> silence

// Trigger range
int chacra6_trigger_min = 0 ;       // min value to trigger the special sound >> 610_chacra1_trigger.wav
int chacra6_trigger_max = 0 ;       // min value to trigger the special sound

// Drone Interference
int chacra6_drone_interference = 0 ;        // from -100 to 100 is the amount of interference of the sensor on the drone's volume

// Pitch Interference
int chacra6_pitch_interference = 0 ;      // from -100 to 100 (2 octaves pitch) is the amount of interference of the sensor on the overall pitch




// Chacra 7 Setup ***********************

// Proximity ranges
int chacra7_range1_min = 0 ;        // min value for the 1st range >> plays 701_chacra1_range1.wav
int chacra7_range2_min = 0 ;        // min value for the 2nd range >> plays 702_chacra1_range2.wav
int chacra7_range3_min = 0 ;        // min value for the 3rd range >> plays 703_chacra1_range3.wav
int chacra7_range4_min = 0 ;        // min value for the 4th range >> plays 704_chacra1_range4.wav
int chacra7_end_min = 0 ;           // min value for the 5th range >> silence

// Trigger range
int chacra7_trigger_min = 0 ;       // min value to trigger the special sound >> 710_chacra1_trigger.wav
int chacra7_trigger_max = 0 ;       // min value to trigger the special sound

// Drone Interference
int chacra7_drone_interference = 0 ;        // from -100 to 100 is the amount of interference of the sensor on the drone's volume

// Pitch Interference
int chacra7_pitch_interference = 0 ;      // from -100 to 100 (2 octaves pitch) is the amount of interference of the sensor on the overall pitch


// ****** E N D   O F   C H A C R A S    S E T U P   ******



/// SIMULATION SETUP

//// SETUP DA SIMULACAO DE SENSORES  -  APAGAR DEPOIS ////


//// INITIALIZE SENSOR READINGS - SIMULATION


int sensor1_read = 0 ;
int sensor2_read = 0 ;
int sensor3_read = 0 ;
int sensor4_read = 0 ;
int sensor5_read = 0 ;
int sensor6_read = 0 ;
int sensor7_read = 0 ;





// ****************************************************************************


void setup() {
  
  // Serial monitor
  Serial.begin(9600);
 
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


//




}



void loop() {


//Serial.println(sensor1_read);

/// MECANISMO DE SIMULACAO DO SENSORES VIA POTENCIOMETRO --- APAGAR

int sensor1_read = analogRead(A0);
int sensor2_read = analogRead(A1);
int sensor3_read = analogRead(A2);
int sensor4_read = analogRead(A3);
int sensor5_read = analogRead(A4);
int sensor6_read = analogRead(A5);

// Chacra Range Selector
// 0=silence / 1,2,3,4 = four ranges / 5 = silence
if (sensor1_read <= chacra1_range1_min ) { int chacra1_state = 0; };
if (sensor1_read > chacra1_range1_min && sensor1_read <= chacra1_range2_min ) { chacra1_range = 1; };
if (sensor1_read > chacra1_range2_min && sensor1_read <= chacra1_range3_min ) { chacra1_range = 2; };
if (sensor1_read > chacra1_range3_min && sensor1_read <= chacra1_range4_min ) { chacra1_range = 3; };
if (sensor1_read > chacra1_range4_min && sensor1_read <= chacra1_end_min ) { chacra1_range = 4; };
if (sensor1_read > chacra1_end_min ) { int chacra1_range = 5; };

Serial.print(sensor1_read);
Serial.print(" > ");
Serial.println(chacra1_range);


if (chacra1_range == last_chacra1_range) { 
  int last_chacra1_range = chacra1_range;
  Serial.println("********");
 
  //updateChacraTsunami (1, chacra1_range) ;
  }




  //*******
  

}




void updateChacraTsunami (int chacra, int range)  {



if ( range == 0 ) { 

  tsunami.trackStop(chacra*100+1);
  tsunami.trackStop(chacra*100+2); 
  tsunami.trackStop(chacra*100+3); 
  tsunami.trackStop(chacra*100+4); 

}



if ( range == 1 ) { 

  tsunami.trackPlayPoly(chacra*100+1, 0, true);     // Start Track 1
  tsunami.trackStop(chacra*100+2); 
  tsunami.trackStop(chacra*100+3); 
  tsunami.trackStop(chacra*100+4); 

}


if ( range == 2 ) { 

  tsunami.trackStop(chacra*100+1);
  tsunami.trackPlayPoly(chacra*100+2, 0, true);     // Start Track 2
  tsunami.trackStop(chacra*100+3); 
  tsunami.trackStop(chacra*100+4); 

}

if ( range == 3 ) { 

  tsunami.trackStop(chacra*100+1);
  tsunami.trackStop(chacra*100+2); 
  tsunami.trackPlayPoly(chacra*100+3, 0, true);     // Start Track 3
  tsunami.trackStop(chacra*100+4); 

}


if ( range == 4 ) { 

  tsunami.trackStop(chacra*100+1);
  tsunami.trackStop(chacra*100+2); 
  tsunami.trackStop(chacra*100+3); 
  tsunami.trackPlayPoly(chacra*100+4, 0, true);     // Start Track 4

}

if ( range == 5 ) { 

  tsunami.trackStop(chacra*100+1);
  tsunami.trackStop(chacra*100+2); 
  tsunami.trackStop(chacra*100+3); 
  tsunami.trackStop(chacra*100+4); 

}

  tsunami.update();


}










//
//  WARNING: The Tsunami is a 3.3V device. Attempting to connect it to a 5V
//   Arduino (such as an UNO) without level-shifters, will likely damage your
//   Tsunami!
//
// ****************************************************************************
//
// To use this sketch with an Arduino, you'll need:
//
// 1) An Arduino with an operating voltage of 3.3V, *** NOT 5V ***!
// 2) Download and install the Metro library.
// 3) Connect 3 wires from the Arduino to the Tsunami's serial connector:
//
//    Arduino       Tsunami
//    =======       =======
//    GND  <------> GND
//    TXn  <------> RX
//    RXn  <------> TX
//
//    If you're using an Arduino with at least one extra hardware serial port,
//    such as an M0 or Teensy, the Tsunami library is already set to use
//    Serial1. If you need to change that, or want to use AltSoftSerial, you
//    must make a small edit the Tsunami.h file, according the instructions in
//    that file.
//
// 4) Download and install the demo wav files onto the Tsunami's microSD
//    card. You can find them here:
//
//    http://robertsonics.com/2015/04/25/arduino-serial-control-tutorial/
//
//    You can certainly use your own tracks instead, although the demo may
//    not make as much sense. If you do, make sure your tracks are at least
//    10 to 20 seconds long and have no silence at the start of the file.
//
// 5) Open the Arduino Serial Monitor window to see status messages.

