#include "esp_camera.h"
#include "fd_forward.h"

// CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22


// Face detection pins
#define FACE_DETECT_PIN    12          // Pin for face detect output signal
#define FACE_DETECT_OFF    HIGH       // Pin state when no face detected - inverted logic to relay
#define FACE_DETECT_ON     LOW       // Pin state when face detected
#define FACE_DETECT_TIME   5000ul      // Duration of face detect signal, after no face detection

// LED Strips
#include <FastLED.h>
#define NUM_LEDS 75
#define DATA_PIN_1 15
#define MAX_BRIGHTNESS 196
CRGB leds[1][NUM_LEDS];
 
mtmn_config_t mtmn_config = {0};

bool initCamera() {
 
  camera_config_t config;
 
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_QVGA;
  config.jpeg_quality = 10;
  config.fb_count = 1;
 
  esp_err_t result = esp_camera_init(&config);
 
  return (result == ESP_OK);
}

 
void setup() {
  Serial.begin(115200);

  Serial.printf("Initializing...\n");

  if (!initCamera()) {
 
    Serial.printf("Failed to initialize camera...\n");
    return;
  }
 
  mtmn_config = mtmn_init_config();

  pinMode(FACE_DETECT_PIN, OUTPUT);
  digitalWrite(FACE_DETECT_PIN, FACE_DETECT_OFF);

  Serial.printf("Ready\n");

  // Initializing leds
  FastLED.addLeds<WS2811, DATA_PIN_1, GRB>(leds[0], NUM_LEDS).setTemperature(OvercastSky);  // GRB ordering is typical  /// WS2812B
  FastLED.setBrightness(MAX_BRIGHTNESS);
  // Tests all 3 LED strips for R,G,B
  // COLOR RED
  fill_solid(leds[0], NUM_LEDS, CRGB(64,0,0));
  FastLED.show();
  delay(1000);

    // COLOR GREEN
  fill_solid(leds[0], NUM_LEDS, CRGB(0,64,0));
  FastLED.show();
  delay(1000);
  
    // COLOR BLUE
  fill_solid(leds[0], NUM_LEDS, CRGB(0,0,64));
  FastLED.show();
  delay(1000);
}

 
void loop() {
  static unsigned long loopcnt = 0ul;
  static unsigned long FaceDetectMillis = millis();
  static bool FaceDetected = false;
  static unsigned int detections = 0;

  loopcnt++;
//  if (loopcnt % 100 == 0) 
    Serial.printf("Loop %lu\n",loopcnt);
  
  camera_fb_t * frame;
  frame = esp_camera_fb_get();
 
  dl_matrix3du_t *image_matrix = dl_matrix3du_alloc(1, frame->width, frame->height, 3);
  fmt2rgb888(frame->buf, frame->len, frame->format, image_matrix->item);
 
  esp_camera_fb_return(frame);
 
  box_array_t *boxes = face_detect(image_matrix, &mtmn_config);
 
  if (boxes != NULL) {
    detections = detections+1;
    Serial.printf("Faces detected %d times\n", detections);

    digitalWrite(FACE_DETECT_PIN, FACE_DETECT_ON);
    FaceDetectMillis = millis();
    FaceDetected = true;
 
    free(boxes->score);
    free(boxes->box);
    free(boxes->landmark);
    free(boxes);
  }
  else if (FaceDetected && (millis() - FaceDetectMillis >= FACE_DETECT_TIME))
  {
    digitalWrite(FACE_DETECT_PIN, FACE_DETECT_OFF);
    FaceDetected = false;
  }

  if (FaceDetected) {

  }
  else {
    EVERY_N_SECONDS(10) {
      fill_solid(leds[0], NUM_LEDS, CRGB(0,0,0));
    }
    ani_breath(0, 12, 0, 24, 2); 
  }
 
  dl_matrix3du_free(image_matrix);
}


void ani_breath(int LED_strip, accum88 breath_BPM, int breathR, int breathG, int breathB ) {


      int breath_bright = beatsin8(breath_BPM, 0, 100);

  fill_solid(leds[LED_strip], NUM_LEDS, CRGB(breathR*breath_bright/100, breathG*breath_bright/100, breathB*breath_bright/100));
      //leds[LED_strip].setBrightness(breath_bright);

}