// ****************************************************************************
//       Sketch: 7 "chacras" sensors TO Tsunami and 2x7 LED strips  
// Date Created: 01jul2018
//
//     Comments: 
//  Programmers: Pedrinho, Juliano Prado, Felipe Bombachini
// ****************************************************************************


#include <Tsunami.h>            // Include the Tsunami library header
#include <AltSoftSerial.h>    // Optional for using AltSoftSerial



/// TSUNAMI

Tsunami tsunami;                // Our Tsunami object

int track = 0 ;
int lasttrack = 0 ;



// ****************************************************************************



void setup() {
  
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


}



void loop() {





//// ************************************************************************
//// ACTIONS SHOOTING !!!
//// (if range changes, shoot an action: sound and/or LED strips)
//// ************************************************************************


//  Serial.println(chacra1_range);

  track = 001;
  Serial.println(track);

  tsunami.trackPlayPoly(track, 0, true);     // Start Track 1
  //tsunami.trackFade(track, 0, 50, false);  // Fade IN Track 1 up to 0db over 50 msecs
  //last_chacra1_range = chacra1_range;

 delay(150000);
  
}
