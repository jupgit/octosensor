#include <FastLED.h>
#define NUM_LEDS 50
#define DATA_PIN 7
#define NUM_RUNNING_LEDS 4
CRGB leds[NUM_LEDS];
//hue 0-255, saturating 0-255, and brightness (value) 0-255
/*HSV colors: 
 * hue 0: pure green
 * hue 96: pure red
 * hue 255: pure blue
 */
void fillAllLeds(CRGB color);

void setup() {
  // put your setup code here, to run once:
   delay(3000);//sanity
   Serial.begin(9600);
   FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
   Serial.print("Start\n");
   fillAllLeds(CRGB(50,80,15));
   
   
   
}


void loop() {
  CRGB blue = CRGB(0,0,50);
  CRGB green = CRGB(100,0,100);
  CRGB red = CRGB(50, 80, 15);
  static uint16_t distance =  0;
  static int flag = 0;
  if(distance <= 301){
    distance+=5;  
    delay(200);
  }
  Serial.print(distance);
   //fillAllLeds(blue);
//  fadeTowardColors(red, green);
 // stackingStripeBackwards(green,  red,  distance);
  firstChakra(green, red, distance);
  //fillIn(red, green, distance);
  delay(200);
  
}

void fillAllLeds(CRGB color){
  for(int dot = 0; dot < NUM_LEDS; dot++) { 
    leds[dot] = color;
  }
  FastLED.show();
}

void  fillIn(CRGB transientColor, CRGB bgColor, uint16_t distance){
      Serial.print("entrando\n");
      Serial.print(distance);
      for(uint16_t i = 0; i < distance ; i++){
        leds[i] = transientColor;
      }
    for(uint16_t i = distance; i < NUM_LEDS ; i++){
        leds[i] = bgColor;  
      }
  FastLED.show();
}

void runningPixel(CRGB color){
  static uint8_t i = 0;
  CRGB temp;
  temp = leds[min(i+NUM_RUNNING_LEDS, NUM_LEDS-1)];
  //temp = CRGB(0,0,255);
  if (i >= NUM_LEDS) i = 0;
  for (uint8_t j = 0; j < NUM_RUNNING_LEDS; j++) { 
    leds[min(i + j, NUM_LEDS - 1)] = color;
  }
  leds[min(i-1, NUM_LEDS - 1)] = temp;
  i++;
  FastLED.show();    
}


void fillInStriped(CRGB stripeColor, CRGB bgColor){
  fillAllLeds(bgColor);
  for(uint16_t i = 0; i < NUM_LEDS; i+=2*NUM_RUNNING_LEDS){
    for(uint16_t j = 0; j < NUM_RUNNING_LEDS; j++){
        leds[i+j] = stripeColor;
    }
  }
  FastLED.show();
}

void walkingStripes(CRGB stripeColor_1, CRGB stripeColor_2){
    static uint16_t numLeds = NUM_LEDS;
   if(numLeds == NUM_LEDS){
    for(uint16_t j = 0; j < NUM_RUNNING_LEDS; j++){
      leds[numLeds - 1 - j] = stripeColor_1;
    }
   }
    if(numLeds >= 0){
      for(uint16_t i = 0; i < numLeds-NUM_RUNNING_LEDS; i++){
        if(leds[i] == stripeColor_1){
          leds[i] = stripeColor_2;
        }
        else{
          leds[i] = stripeColor_1;
        }
    }
    numLeds-=NUM_RUNNING_LEDS;
    }
    
    FastLED.show();
}
void stackingStripe(CRGB stripeColor, CRGB bgColor, uint16_t distance){
  static uint16_t current = 0;
  uint16_t numLeds = distance;
  if(current < NUM_LEDS){
      if(current+2*NUM_RUNNING_LEDS < distance){
     for(uint16_t j = 0; j < NUM_RUNNING_LEDS; j++){
          leds[current+j] = stripeColor;
      }
      distance = distance + (distance % NUM_RUNNING_LEDS);
      current = min(current+2*NUM_RUNNING_LEDS, distance);
      }
      
  }
  else{
    EVERY_N_MILLISECONDS(100){
      walkingStripes(stripeColor, bgColor);
    }
  }
  FastLED.show();
}

void stackingStripeBackwards(CRGB stripeColor, CRGB bgColor, uint16_t distance){
  static uint16_t current = NUM_LEDS - 1;
  Serial.print(current);
  Serial.print('\n');
  distance = 300 - distance;
  if(current >= 0 ){
      //if(current+2*NUM_RUNNING_LEDS < distance){
       for(uint16_t j = 0; j < NUM_RUNNING_LEDS; j++){
            leds[current-j] = stripeColor;
        }
        distance = distance - (distance % NUM_RUNNING_LEDS);
        current = max(current-2*NUM_RUNNING_LEDS, distance);
      //}
      
  }
  else{
    EVERY_N_MILLISECONDS(100){
      walkingStripes(stripeColor, bgColor);
    }
  }
  FastLED.show();
}

void fadeTowardColors(CRGB colorOne, CRGB bgColor){
  static float j = 0;
  if(j >= -5){
    j-=0.2;
    for(uint16_t i = 0; i < NUM_LEDS; i++){
        leds[i].r =uint8_t(min(float(bgColor.r)*exp(-5-j),bgColor.r)) + uint8_t(max(float(colorOne.r)*exp(j),0));
        leds[i].g =uint8_t(min(float(bgColor.g)*exp(-5-j),bgColor.g)) + uint8_t(max(float(colorOne.g)*exp(j),0));
        leds[i].b =uint8_t(min(float(bgColor.b)*exp(-5-j),bgColor.b)) + uint8_t(max(float(colorOne.b)*exp(j),0));
    }
    FastLED.show();
  }
  return;
}


void firstChakra(CRGB transientColor, CRGB bgColor, uint16_t distance){
  if(distance >= 300){
    fadeTowardColors(transientColor, bgColor);
  }
  else    fillIn(transientColor, bgColor, distance);
}


void secondChakra(CRGB transientColor, CRGB bgColor, uint16_t distance){
  if(distance >= 300 && leds[0] == leds[NUM_RUNNING_LEDS]){
    EVERY_N_MILLISECONDS(100){
      fadeTowardColors(transientColor, bgColor);
    }
  }else {
      stackingStripe( transientColor, bgColor, distance);
  }
}

void thirdChakra(CRGB transientColor, CRGB bgColor, uint16_t distance){
  
}



void backwardsFillStriped(CRGB transientColor, CRGB bgColor,  int distance){
  static int i = 0;
  static bool inicialized = false;
  static bool alreadyFilled = false;

  /* 

    Test if the leds alread are inicialized i.e. 
    if all leds have been filled with the bg color once


  */
  for(i = NUM_LEDS - 1; i >=  0 ; i-- ){
    if(leds[i] != bgColor && leds[i] != NULL){
      inicialized = true;
      break;
    }
  }
  if(!inicialized) {
    fillAllLeds(bgColor);
    FastLED.show();
  }


  /*
    Start testing if the distance is less than NUM_LEDS.
    If it is fill striped style the led strip from the end
    till the number of size (NUM_LEDS - distance) - (NUM_LEDS - distance)%2*NUM_RUNNING_LEDS
  */
  if(distance <= 30){
    int filledSyze = NUM_LEDS - (float)((NUM_LEDS/30)*distance) ;
    for(i = NUM_LEDS - 1; i >= max(filledSyze, 0); i -= 2*NUM_RUNNING_LEDS){
      int j = 0;
      for(j = 0; j < NUM_RUNNING_LEDS; j++){
        if(i-j >= 0) leds[i-j] = transientColor;
      }
    }
    FastLED.show();
  }
  return;
}

void fillAllLeds(CRGB bgColor, CRGB leds[]){
  int i = 0;

  for(i = 0; i < NUM_LEDS; i++){
    leds[i] = bgColor;
  }
  return;
}

void runningBackwardsStripes(CRGB transientColor, CRGB bgColor,  int distance){
  
  int j = 0;
  CRGB prev = (CRGB) 0;
  CRGB current = (CRGB) 0;
  
  int filledSyze = (NUM_LEDS/30)*distance%30;   
  filledSyze -= filledSyze%NUM_RUNNING_LEDS;    
  for(j = 0; j < NUM_RUNNING_LEDS; j++){
    leds[filledSyze+j] = transientColor;
  }
  for(j = filledSyze; j < NUM_LEDS; j++ ){
    if(j == filledSyze){
      prev = leds[j];
    }
    else{
      current = leds[j];
      leds[j] = prev;
      prev = current;
    }
  } 
  FastLED.show();
  return;
}

void runningBackwardsStripes1(CRGB transientColor, CRGB bgColor,  int distance){
  
  int j = 0;
  CRGB prev = (CRGB) 0;
  CRGB current = (CRGB) 0;
  static bool inicialized = false;
  int filledSyze = ((float)NUM_LEDS/30.0)*(distance%30);  


  if(!inicialized){
     fillInStriped(transientColor, bgColor, distance);
     inicialized = true;
  }

  //printf("filledSyze %d\n", filledSyze);  
  filledSyze -= filledSyze%NUM_RUNNING_LEDS;    
  for(j = 0; j < NUM_RUNNING_LEDS; j++){
    leds[filledSyze+j] = transientColor;
  }
  for(j = filledSyze; j < NUM_LEDS; j++ ){
    if(j == filledSyze){
      prev = leds[j];
    }
    else{
      current = leds[j];
      leds[j] = prev;
      prev = current;
    }
  } 
  FastLED.show();
  return;
}

void ThirdChakra(CRGB transientColor, CRGB bgColor,  int distance){
  if(distance == 0 || distance > 90) {
    fillAllLeds(bgColor);
    FastLED.show();
  }
  /*

    Distance measure the percentage of the 
    whole path walked by some person.
    for exemple
    distance used for the ping sensor goes from 300(3m) to 45(45cm) f
    them distance = 0 means ping = 300 because the path has not been walked;
    and if ping = 45 then distance = 100 because all the way has been walked
    and if ping =  172 -- halfway ((300+45)/2)
    then distance == 50 


  */



  /*
    the paths explained
  */
  if(distance < 31){
    backwardsFillStriped(transientColor, bgColor, distance); 
  }
  else if(distance < 61){
    runningBackwardsStripes(transientColor, bgColor, distance);
  }
  else if(distance < 71){
    EVERY_N_MILLISECONDS(200){
      if(leds[0] == bgColor) fillAllLeds(transientColor);
      else fillAllLeds(bgColor);
      FastLED.show();
    }
  }
  else if(distance < 81){
    EVERY_N_MILLISECONDS(100){
      if(leds[0] == bgColor) fillAllLeds(transientColor);
      else fillAllLeds(bgColor);
      FastLED.show();
    }
  }
  else if (distance < 91){
    EVERY_N+MILLISECONDS(50){
      if(leds[0] == bgColor) fillAllLeds(transientColor);
      else fillAllLeds(bgColor);
      FastLED.show();
    }
    fillAllLeds(bgColor);
    FastLED.show();
  }
  return;
}



void fourthChakra(CRGB transientColor, CRGB bgColor,  int distance){
  // todo in arduino enviroment
}


void fifthChakra(CRGB transientColor, CRGB bgColor,  int distance){
  if(distance < 60){
    pingPongStripes(transientColor,bgColor,leds, distance);
  }
  else if(distance < 90){
    runningBackwardsStripes1(transientColor,bgColor,leds, distance);
  }
  else{
    int i = 0;
    static bool inicialized = false;
    if(!inicialized){
        fillInStriped(bgColor, transientColor,leds, i);
        sleep(0.2);
      inicialized = true;
      
    }
    for(i = 0; i < 100; i++){ 
      runningBackwardsStripes1(bgColor, transientColor,leds, i);  
      sleep(0.2);
    }

  }
  
}

void pingPongStripes(CRGB transientColor, CRGB bgColor,  int distance){
  static int state = 0;
  static int numOfStripes = 0;
  //array stripe sense and position
  static int stripes [NUM_LEDS][2];
  static bool inicialized = false;
  static bool alreadyFilled = false;

  int queueZeroIndex = 2*NUM_LEDS - stripes[0][1];
  static int stackTopIndex = 0;
  int i=0;
  int j=0;

  /*
    init array of stripes
  */
  if(!inicialized){
    for(i = 0; i < NUM_LEDS; i++){
      for(j = 0; j < 2; j++){
        stripes[i][j] = 0;
      }
    }
  }
   /*

    Test if the leds alread are inicialized i.e. 
    if all leds have been filled with the bg color once

  */
  for(i = NUM_LEDS - 1; i >=  0 ; i-- ){
    if(leds[i] != bgColor && leds[i] != NULL){
      inicialized = true;
      break;
    }
  }
  
    int filledSyze = (float)((float)(NUM_LEDS/30.0)*distance);
    stripes[numOfStripes][1] = filledSyze - (int)((float)numOfStripes*(2.5*(float)NUM_RUNNING_LEDS)) ;
    if((stripes[numOfStripes][1] - NUM_RUNNING_LEDS) >= (int)(2.5*NUM_RUNNING_LEDS) ){
      numOfStripes++;
    }
    if(queueZeroIndex <= stackTopIndex){
        stackTopIndex += NUM_RUNNING_LEDS;
        for(i = 0; i < numOfStripes; i++){
        stripes[i+1][1]);
        stripes[i][1] = stripes[i+1][1];
      }
      numOfStripes--;

    }
    fillAllLeds(bgColor);
    for(i = 0; i < stackTopIndex; i++) leds[i] = transientColor;
    for(i = stackTopIndex; i <= numOfStripes; i++){
      stripes[i][0] = 0;
      stripes[i][1] = filledSyze - (int)((float)i*(2.5*(float)NUM_RUNNING_LEDS));
        if(i < stackTopIndex){
          leds[i] = transientColor;
        }
        if(stripes[i][1]-j >= 0 && stripes[i][1]-j < NUM_LEDS) {
          
          for(j = 0; j < NUM_RUNNING_LEDS; j++){
            leds[stripes[i][1]-j] = transientColor;
          }
        }
        else if(stripes[i][1] >= NUM_LEDS && stripes[i][1] < 2*NUM_LEDS ){
          
          for(j = 0; j < NUM_RUNNING_LEDS; j++){
            int backwardsIndex = 2*NUM_LEDS - stripes[i][1];
            leds[backwardsIndex - j] = transientColor;
          }
        }
    }
}



void fillInStriped(CRGB transientColor, CRGB bgColor, int distance){
  fillAllLeds(bgColor);
  int i = 0;
  int j = 0;
  for(i = 0; i < NUM_LEDS; i += (int)(2.5*(float)NUM_RUNNING_LEDS) ){
    for(j = 0; j < NUM_RUNNING_LEDS; j++){
      leds[i+j] = transientColor;
    }
  }
  FastLED.show();
}

