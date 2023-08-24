// ****************************************************************************
//       Sketch: 7 "chacras" sensors TO Tsunami and 2x7 LED strips  
// Date Created: 01jul2018
//
//     Comments: 
//  Programmers: Pedrinho, Juliano Prado, Felipe Bombachini
// ****************************************************************************


//#include <Metro.h>              // Include the Metro library header
#include <Tsunami.h>            // Include the Tsunami library header
#include <AltSoftSerial.h>    // Optional for using AltSoftSerial

//#define LED 13                  // our LED







/// TSUNAMI

Tsunami tsunami;                // Our Tsunami object

int track = 0 ;
int lasttrack = 0 ;

// ****** C H A C R A S    S E T U P   ******

// Each chacra will play 4 audios (one at a time) acording to 4 ranges of proximity
// Each chacra can trigger a special audio, defined by a min and max value
// Each chacra can interfere with volume of the drone sound
// Each chacra can interfere with the overall pitch of the instalation


// CHACRA 1 Setup ***********************

// Proximity ranges
int chacra1_range1_min = 200 ;        // min value for the 1th range >> silence
int chacra1_range2_min = 960 ;        // min value for the 2st range >> plays 101_chacra1_range1.wav
int chacra1_range3_min = 1000 ;        // min value for the 3nd range >> plays 102_chacra1_range2.wav
int chacra1_range4_min = 1010 ;        // min value for the 4rd range >> plays 103_chacra1_range3.wav
int chacra1_range5_min = 1022 ;        // min value for the 5th range >> plays 104_chacra1_range4.wav
int chacra1_range6_min = 1024 ;        // min value for the 6th range >> silence

int chacra1_range = 0  ;             // 0=silence / 1,2,3,4 = four ranges / 5 = silence
int last_chacra1_range = 0 ;


// Trigger range
int chacra1_trigger_min = 0 ;       // min value to trigger the special sound >> 110_chacra1_trigger.wav
int chacra1_trigger_max = 0 ;       // min value to trigger the special sound

// Drone Interference
int chacra1_drone_interference = 0 ;        // from -100 to 100 is the amount of interference of the sensor on the drone's volume

// Pitch Interference
int chacra1_pitch_interference = 0 ;      // from -100 to 100 (2 octaves pitch) is the amount of interference of the sensor on the overall pitch



// ****** E N D   O F   C H A C R A S    S E T U P   ******


/// SIMULATION SETUP

//// SETUP DA SIMULACAO DE SENSORES  -  APAGAR DEPOIS ////


//// INITIALIZE SENSOR READINGS - SIMULATION


int sensor1_read = 0 ;


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

tsunami.trackGain(101, 0);             // Preset Track 101 gain to -40dB
tsunami.trackGain(102, -40);             // Preset Track 101 gain to -40dB
tsunami.trackGain(103, -40);             // Preset Track 101 gain to -40dB
tsunami.trackGain(104, -40);             // Preset Track 101 gain to -40dB


}



void loop() {


//Serial.println(sensor1_read);

/// MECANISMO DE SIMULACAO DO SENSORES VIA POTENCIOMETRO --- APAGAR

  int sensor1_read = analogRead(A0);


  

while (1){
  if (sensor1_read < chacra1_range1_min){ chacra1_range = 1; break;}
  if (sensor1_read < chacra1_range2_min){ chacra1_range = 2; break;}
  if (sensor1_read < chacra1_range3_min){ chacra1_range = 3; break;}
  if (sensor1_read < chacra1_range4_min){ chacra1_range = 4; break;}
  if (sensor1_read < chacra1_range5_min){ chacra1_range = 5; break;}
  if (sensor1_read < chacra1_range6_min){ chacra1_range = 6; break;} 
  break;
}




if(chacra1_range!= last_chacra1_range){
  //SENSOR 1
  //POSIÇÃO DO SENSOR "POSI_SENSE1"
  //VALOR DIRETO "sensor1_read"


  Serial.println(chacra1_range);

  
  track = 700 + chacra1_range ;
  lasttrack = 700 + last_chacra1_range ;
  tsunami.trackFade(lasttrack, -40, 300, false);  // Fade OUT LAST Track down to -40db over 300 msecs
  tsunami.trackPlayPoly(track, 0, true);     // Start Track 1
  tsunami.trackFade(track, 0, 300, false);  // Fade IN Track 1 up to 0db over 300 msecs
  tsunami.update();



  
 
}

last_chacra1_range = chacra1_range;




  
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

