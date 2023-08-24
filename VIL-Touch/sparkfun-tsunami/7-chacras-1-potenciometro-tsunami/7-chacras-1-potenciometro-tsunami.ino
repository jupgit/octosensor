// ****************************************************************************
//       Sketch: 7 "chacras" sensors TO Tsunami and 2x7 LED strips  
// Date Created: 01jul2018
//
//     Comments: 
//  Programmers: Pedrinho, Juliano Prado, Felipe Bombachini
// ****************************************************************************


#include <Tsunami.h>            // Include the Tsunami library header
#include <AltSoftSerial.h>    // Optional for using AltSoftSerial


// SENSORES SONAR E PING

/// PROFESSOR PAULO:
/// CONFIGURE AQUI AS PORTAS QUE ESTARAO ATIVAS
/// USE: 1 = ATIVA | 0 = INATIVA

/// SENSORES SONAR
int ativosonar1 = 1;
int ativosonar2 = 1;
int ativosonar3 = 1;
int ativosonar4 = 1;
int ativosonar5 = 1;
int ativosonar6 = 1;
int ativosonar7 = 1;


/// SENSORES PING
int ativoping1 = 0;
int ativoping2 = 0;
int ativoping3 = 0;
int ativoping4 = 0;
int ativoping5 = 0;
int ativoping6 = 0;
int ativoping7 = 0;

/// 

// DEFINE OS PINOS DAS CONEXOES DE CADA SENSOR NO ARDUINO
int pwPin1 = 2; // = SENSOR 1 NO PINO 2 DO ARDUINO
int pwPin2 = 3;
int pwPin3 = 4;
int pwPin4 = 5;
int pwPin5 = 6;
int pwPin6 = 7;
int pwPin7 = 8;


// PING :: CONEXOES DE CADA SENSOR NO ARDUINO (não precisa...)
//int ping1Pin = 2;
//int ping2Pin = 3;
//int ping3Pin = 4;
//int ping4Pin = 5;
//int ping5Pin = 6;
//int ping6Pin = 7;
//int ping7Pin = 8;


// CONEXÕES DO SHIELD DE RELÊS

int relay1Pin = 10;
int relay2Pin = 11;
int relay3Pin = 12;
int relay4Pin = 13;







//variables needed to store values
long pulse, inches, cm;
int val1;
int val2;
int val3;
int val4;

int duration;
int var=0;
int var1=0;
int val; //Declare variables. any words are fine.
int ana = 0; // Value must be same as a number of the analog pin





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
int chacra1_range1_min = 200 ;        // start value for the 1th range >> silence
int chacra1_range2_min = 300 ;        // start value for the 2st range >> plays 101_chacra1_range1.wav
int chacra1_range3_min = 400 ;        // start value for the 3nd range >> plays 102_chacra1_range2.wav
int chacra1_range4_min = 500 ;        // start value for the 4rd range >> plays 103_chacra1_range3.wav
int chacra1_range5_min = 600 ;        // start value for the 5th range >> plays 104_chacra1_range4.wav
int chacra1_range6_min = 800 ;        // start value for the 6th range >> silence

int chacra1_range = 0  ;             // 0=silence / 1,2,3,4 = four ranges / 5 = silence
int last_chacra1_range = 0 ;


// RELAY Trigger range
int chacra1_trigger_min = 300 ;       // min value to trigger the special sound >> 110_chacra1_trigger.wav
int chacra1_trigger_max = 700 ;       // min value to trigger the special sound

// Drone Interference
//int chacra1_drone_interference = 0 ;        // from -100 to 100 is the amount of interference of the sensor on the drone's volume

// Pitch Interference
//int chacra1_pitch_interference = 0 ;      // from -100 to 100 (2 octaves pitch) is the amount of interference of the sensor on the overall pitch



// CHACRA 2 Setup ***********************

// Proximity ranges
int chacra2_range1_min = 200 ;        // start value for the 1th range >> silence
int chacra2_range2_min = 300 ;        // start value for the 2st range >> plays 101_chacra1_range1.wav
int chacra2_range3_min = 400 ;        // start value for the 3nd range >> plays 102_chacra1_range2.wav
int chacra2_range4_min = 500 ;        // start value for the 4rd range >> plays 103_chacra1_range3.wav
int chacra2_range5_min = 600 ;        // start value for the 5th range >> plays 104_chacra1_range4.wav
int chacra2_range6_min = 800 ;        // start value for the 6th range >> silence

int chacra2_range = 0  ;             // STATUS ATUAL0=silence / 1,2,3,4 = four ranges / 5 = silence
int last_chacra2_range = 0 ;          // ULTIMO STATUS

// CHACRA 3 Setup ***********************

// Proximity ranges
int chacra3_range1_min = 200 ;        // start value for the 1th range >> silence
int chacra3_range2_min = 300 ;        // start value for the 2st range >> plays 101_chacra1_range1.wav
int chacra3_range3_min = 400 ;        // start value for the 3nd range >> plays 102_chacra1_range2.wav
int chacra3_range4_min = 500 ;        // start value for the 4rd range >> plays 103_chacra1_range3.wav
int chacra3_range5_min = 600 ;        // start value for the 5th range >> plays 104_chacra1_range4.wav
int chacra3_range6_min = 800 ;        // start value for the 6th range >> silence

int chacra3_range = 0  ;             // 0=silence / 1,2,3,4 = four ranges / 5 = silence
int last_chacra3_range = 0 ;

// CHACRA 4 Setup ***********************

// Proximity ranges
int chacra4_range1_min = 200 ;        // start value for the 1th range >> silence
int chacra4_range2_min = 300 ;        // start value for the 2st range >> plays 101_chacra1_range1.wav
int chacra4_range3_min = 400 ;        // start value for the 3nd range >> plays 102_chacra1_range2.wav
int chacra4_range4_min = 500 ;        // start value for the 4rd range >> plays 103_chacra1_range3.wav
int chacra4_range5_min = 600 ;        // start value for the 5th range >> plays 104_chacra1_range4.wav
int chacra4_range6_min = 800 ;        // start value for the 6th range >> silence

int chacra4_range = 0  ;             // 0=silence / 1,2,3,4 = four ranges / 5 = silence
int last_chacra4_range = 0 ;

// CHACRA 5 Setup ***********************

// Proximity ranges
int chacra5_range1_min = 200 ;        // start value for the 1th range >> silence
int chacra5_range2_min = 300 ;        // start value for the 2st range >> plays 101_chacra1_range1.wav
int chacra5_range3_min = 400 ;        // start value for the 3nd range >> plays 102_chacra1_range2.wav
int chacra5_range4_min = 500 ;        // start value for the 4rd range >> plays 103_chacra1_range3.wav
int chacra5_range5_min = 600 ;        // start value for the 5th range >> plays 104_chacra1_range4.wav
int chacra5_range6_min = 800 ;        // start value for the 6th range >> silence

int chacra5_range = 0  ;             // 0=silence / 1,2,3,4 = four ranges / 5 = silence
int last_chacra5_range = 0 ;

// CHACRA 6 Setup ***********************

// Proximity ranges
int chacra6_range1_min = 200 ;        // start value for the 1th range >> silence
int chacra6_range2_min = 300 ;        // start value for the 2st range >> plays 101_chacra1_range1.wav
int chacra6_range3_min = 400 ;        // start value for the 3nd range >> plays 102_chacra1_range2.wav
int chacra6_range4_min = 500 ;        // start value for the 4rd range >> plays 103_chacra1_range3.wav
int chacra6_range5_min = 600 ;        // start value for the 5th range >> plays 104_chacra1_range4.wav
int chacra6_range6_min = 700 ;        // start value for the 6th range >> silence

int chacra6_range = 0  ;             // 0=silence / 1,2,3,4 = four ranges / 5 = silence
int last_chacra6_range = 0 ;


// CHACRA 7 Setup ***********************

// Proximity ranges
int chacra7_range1_min = 200 ;        // start value for the 1th range >> silence
int chacra7_range2_min = 300 ;        // start value for the 2st range >> plays 101_chacra1_range1.wav
int chacra7_range3_min = 400 ;        // start value for the 3nd range >> plays 102_chacra1_range2.wav
int chacra7_range4_min = 500 ;        // start value for the 4rd range >> plays 103_chacra1_range3.wav
int chacra7_range5_min = 600 ;        // start value for the 5th range >> plays 104_chacra1_range4.wav
int chacra7_range6_min = 800 ;        // start value for the 6th range >> silence

int chacra7_range = 0  ;             // 0=silence / 1,2,3,4 = four ranges / 5 = silence
int last_chacra7_range = 0 ;

// RELAY Trigger range
int chacra7_trigger_min = 300 ;       // min value to trigger the special sound >> 110_chacra1_trigger.wav
int chacra7_trigger_max = 700 ;       // min value to trigger the special sound


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


// SENSORS  

// Initialiize INPUTs
  pinMode(pwPin1, INPUT);
  pinMode(pwPin2, INPUT);  
  pinMode(pwPin3, INPUT);  
  pinMode(pwPin4, INPUT);
  pinMode(pwPin1, INPUT);
  pinMode(pwPin2, INPUT);  
  pinMode(pwPin3, INPUT);  
  pinMode(pwPin4, INPUT); 


/// RELAYS

// Initialiize OUTPUTs
  pinMode(relay1Pin, OUTPUT);
  pinMode(relay2Pin, OUTPUT);  
  pinMode(relay3Pin, OUTPUT);  
  pinMode(relay4Pin, OUTPUT);


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


}



void loop() {









/// MECANISMO DE SIMULACAO DO SENSORES VIA POTENCIOMETRO --- APAGAR

int sensor1_read = 0;  //analogRead(A0);
int sensor2_read = 0; //1024 - analogRead(A0);
int sensor3_read = 0; //analogRead(A0);
int sensor4_read = analogRead(A0);
int sensor5_read = 0; //analogRead(A0);
int sensor6_read = 0; //analogRead(A0);
int sensor7_read = 0;



//// ************************************************************************
//// RANGE SELECTORS (converts sensor reads to ranges for each sensor)
//// ************************************************************************
  
// SENSOR 1
while (1){
  if (sensor1_read < chacra1_range1_min){ chacra1_range = 1; break;}
  if (sensor1_read < chacra1_range2_min){ chacra1_range = 2; break;}
  if (sensor1_read < chacra1_range3_min){ chacra1_range = 3; break;}
  if (sensor1_read < chacra1_range4_min){ chacra1_range = 4; break;}
  if (sensor1_read < chacra1_range5_min){ chacra1_range = 5; break;}
  if (sensor1_read < chacra1_range6_min){ chacra1_range = 6; break;} 
  break;
}

// SENSOR 2
while (2){
  if (sensor2_read < chacra2_range1_min){ chacra2_range = 1; break;}
  if (sensor2_read < chacra2_range2_min){ chacra2_range = 2; break;}
  if (sensor2_read < chacra2_range3_min){ chacra2_range = 3; break;}
  if (sensor2_read < chacra2_range4_min){ chacra2_range = 4; break;}
  if (sensor2_read < chacra2_range5_min){ chacra2_range = 5; break;}
  if (sensor2_read < chacra2_range6_min){ chacra2_range = 6; break;} 
  break;
}

// SENSOR 3
while (1){
  if (sensor3_read < chacra3_range1_min){ chacra3_range = 1; break;}
  if (sensor3_read < chacra3_range2_min){ chacra3_range = 2; break;}
  if (sensor3_read < chacra3_range3_min){ chacra3_range = 3; break;}
  if (sensor3_read < chacra3_range4_min){ chacra3_range = 4; break;}
  if (sensor3_read < chacra3_range5_min){ chacra3_range = 5; break;}
  if (sensor3_read < chacra3_range6_min){ chacra3_range = 6; break;} 
  break;
}

// SENSOR 4
while (1){
  if (sensor4_read < chacra4_range1_min){ chacra4_range = 1; break;}
  if (sensor4_read < chacra4_range2_min){ chacra4_range = 2; break;}
  if (sensor4_read < chacra4_range3_min){ chacra4_range = 3; break;}
  if (sensor4_read < chacra4_range4_min){ chacra4_range = 4; break;}
  if (sensor4_read < chacra4_range5_min){ chacra4_range = 5; break;}
  if (sensor4_read < chacra4_range6_min){ chacra4_range = 6; break;} 
  break;
}

// SENSOR 5
while (1){
  if (sensor5_read < chacra5_range1_min){ chacra5_range = 1; break;}
  if (sensor5_read < chacra5_range2_min){ chacra5_range = 2; break;}
  if (sensor5_read < chacra5_range3_min){ chacra5_range = 3; break;}
  if (sensor5_read < chacra5_range4_min){ chacra5_range = 4; break;}
  if (sensor5_read < chacra5_range5_min){ chacra5_range = 5; break;}
  if (sensor5_read < chacra5_range6_min){ chacra5_range = 6; break;} 
  break;
}

// SENSOR 6
while (1){
  if (sensor6_read < chacra6_range1_min){ chacra6_range = 1; break;}
  if (sensor6_read < chacra6_range2_min){ chacra6_range = 2; break;}
  if (sensor6_read < chacra6_range3_min){ chacra6_range = 3; break;}
  if (sensor6_read < chacra6_range4_min){ chacra6_range = 4; break;}
  if (sensor6_read < chacra6_range5_min){ chacra6_range = 5; break;}
  if (sensor6_read < chacra6_range6_min){ chacra6_range = 6; break;} 
  break;
}

// SENSOR 7
while (1){
  if (sensor7_read < chacra7_range1_min){ chacra7_range = 1; break;}
  if (sensor7_read < chacra7_range2_min){ chacra7_range = 2; break;}
  if (sensor7_read < chacra7_range3_min){ chacra7_range = 3; break;}
  if (sensor7_read < chacra7_range4_min){ chacra7_range = 4; break;}
  if (sensor7_read < chacra7_range5_min){ chacra7_range = 5; break;}
  if (sensor7_read < chacra7_range6_min){ chacra7_range = 6; break;} 
  break;
}








//// ************************************************************************
//// ACTIONS SHOOTING !!!
//// (if range changes, shoot an action: sound and/or LED strips)
//// ************************************************************************



// CHACRA 1
if(chacra1_range!= last_chacra1_range){

//  Serial.println(chacra1_range);

  track = 100 + (chacra1_range-1) ;
  Serial.println(track);

  lasttrack = 100 + last_chacra1_range ;
  tsunami.trackFade(lasttrack, -40, 100, false);  // Fade OUT LAST Track down to -40db over 300 msecs
  tsunami.trackPlayPoly(track, 0, true);     // Start Track 1
  tsunami.trackFade(track, 0, 50, false);  // Fade IN Track 1 up to 0db over 300 msecs
  last_chacra1_range = chacra1_range;
}

// Relay Chacra 1
  if(sensor1_read >= chacra1_trigger_min && sensor1_read <= chacra1_trigger_max) {
   digitalWrite(relay1Pin, HIGH);
   digitalWrite(relay2Pin, HIGH);   
  } else {
   digitalWrite(relay1Pin, LOW);
   digitalWrite(relay2Pin, LOW);  
  }

  

// CHACRA 2
if(chacra2_range!= last_chacra2_range){

//  Serial.println(chacra1_range);

  track = 200 + (chacra2_range-1)  ;
    Serial.println(track);

  lasttrack = 200 + last_chacra2_range ;
  tsunami.trackFade(lasttrack, -40, 100, false);  // Fade OUT LAST Track down to -40db over 300 msecs
  tsunami.trackPlayPoly(track, 0, true);     // Start Track 1
  tsunami.trackFade(track, 0, 50, false);  // Fade IN Track 1 up to 0db over 300 msecs
  last_chacra2_range = chacra2_range;
}


// CHACRA 3
if(chacra3_range!= last_chacra3_range){

//  Serial.println(chacra1_range);

  track = 300 + (chacra3_range-1)  ;
  lasttrack = 300 + last_chacra3_range ;
  tsunami.trackFade(lasttrack, -40, 100, false);  // Fade OUT LAST Track down to -40db over 300 msecs
  tsunami.trackPlayPoly(track, 0, true);     // Start Track 1
  tsunami.trackFade(track, 0, 50, false);  // Fade IN Track 1 up to 0db over 300 msecs
  last_chacra3_range = chacra3_range;
}


// CHACRA 4
if(chacra4_range!= last_chacra4_range){

//  Serial.println(chacra1_range);

  track = 400 + (chacra4_range-1)  ;
    Serial.println(track);

  lasttrack = 400 + last_chacra4_range ;
  tsunami.trackFade(lasttrack, -40, 100, false);  // Fade OUT LAST Track down to -40db over 300 msecs
  tsunami.trackPlayPoly(track, 0, true);     // Start Track 1
  tsunami.trackFade(track, 0, 50, false);  // Fade IN Track 1 up to 0db over 300 msecs
  last_chacra4_range = chacra4_range;
}



// CHACRA 5
if(chacra5_range!= last_chacra5_range){

//  Serial.println(chacra1_range);

  track = 500 + (chacra5_range-1)  ;
    Serial.println(track);

  lasttrack = 500 + last_chacra5_range ;
  tsunami.trackFade(lasttrack, -40, 100, false);  // Fade OUT LAST Track down to -40db over 300 msecs
  tsunami.trackPlayPoly(track, 0, true);     // Start Track 1
  tsunami.trackFade(track, 0, 50, false);  // Fade IN Track 1 up to 0db over 300 msecs
  last_chacra5_range = chacra5_range;
}



// CHACRA 6
if(chacra6_range!= last_chacra6_range){

//  Serial.println(chacra1_range);

  track = 600 + (chacra6_range-1)  ;
    Serial.println(track);

  lasttrack = 600 + last_chacra6_range ;
  tsunami.trackFade(lasttrack, -40, 100, false);  // Fade OUT LAST Track down to -40db over 300 msecs
  tsunami.trackPlayPoly(track, 0, true);     // Start Track 1
  tsunami.trackFade(track, 0, 50, false);  // Fade IN Track 1 up to 0db over 300 msecs
  last_chacra6_range = chacra6_range;
}



// CHACRA 7
if(chacra7_range!= last_chacra7_range){

//  Serial.println(chacra1_range);

  track = 700 + (chacra7_range-1)  ;
    Serial.println(track);

  lasttrack = 700 + last_chacra7_range ;
  tsunami.trackFade(lasttrack, -40, 100, false);  // Fade OUT LAST Track down to -40db over 300 msecs
  tsunami.trackPlayPoly(track, 0, true);     // Start Track 1
  tsunami.trackFade(track, 0, 50, false);  // Fade IN Track 1 up to 0db over 300 msecs
  last_chacra7_range = chacra7_range;
}

// Relay Chacra 7
  if(sensor7_read >= chacra7_trigger_min && sensor7_read <= chacra7_trigger_max) {
   digitalWrite(relay3Pin, HIGH);
   digitalWrite(relay4Pin, HIGH);   
  } else {
   digitalWrite(relay3Pin, LOW);
   digitalWrite(relay4Pin, LOW);  
  }

  
}
