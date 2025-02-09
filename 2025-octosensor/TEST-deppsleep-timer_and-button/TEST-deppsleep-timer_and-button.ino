/*
  Deep Sleep with External and Timer Wake Up
  ===========================================
  This code adds timer wake-up functionality to the existing
  external wake-up mechanism.

  Hardware Connections
  ======================
  - Push Button to GPIO 33 pulled down with a 10K Ohm resistor.

  NOTE:
  ======
  - Only RTC IO can be used as a source for external wake-up.
    RTC IO Pins: 0, 2, 4, 12-15, 25-27, 32-39.

  Author: Modified by [Your Name] - Based on Public Domain Code
*/

#include "driver/rtc_io.h"

#define BUTTON_PIN_BITMASK(GPIO) (1ULL << GPIO)  // Define GPIO bitmask
#define USE_EXT0_WAKEUP          1               // 1 = EXT0 wakeup, 0 = EXT1 wakeup
#define WAKEUP_GPIO              GPIO_NUM_33     // RTC IO pin for external wake-up

#define TIME_TO_SLEEP 10  // Time in seconds before ESP32 wakes up from deep sleep

RTC_DATA_ATTR int bootCount = 0;  // Store boot count in RTC memory

/*
  Method to print the reason by which ESP32
  has been awakened from sleep.
*/
void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0:
      Serial.println("Wakeup caused by external signal using RTC_IO");
      break;
    case ESP_SLEEP_WAKEUP_EXT1:
      Serial.println("Wakeup caused by external signal using RTC_CNTL");
      break;
    case ESP_SLEEP_WAKEUP_TIMER:
      Serial.println("Wakeup caused by timer");
      break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD:
      Serial.println("Wakeup caused by touchpad");
      break;
    case ESP_SLEEP_WAKEUP_ULP:
      Serial.println("Wakeup caused by ULP program");
      break;
    default:
      Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason);
      break;
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);  // Take some time to open the Serial Monitor

  // Increment boot count and print it
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

  // Print wake-up reason
  print_wakeup_reason();

  /*
    Configure Wake-up Sources
    =========================
    1. External Wake-up (Button Press)
    2. Timer Wake-up (After X Seconds)
  */

  // Enable Timer Wake-up
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * 1000000);  // Convert seconds to microseconds

#if USE_EXT0_WAKEUP
  // Enable External Wake-up (EXT0)
  esp_sleep_enable_ext0_wakeup(WAKEUP_GPIO, 1);  // 1 = HIGH, 0 = LOW
  rtc_gpio_pullup_dis(WAKEUP_GPIO);
  rtc_gpio_pulldown_en(WAKEUP_GPIO);

#else  // EXT1 WAKEUP
  // Enable External Wake-up (EXT1)
  esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK(WAKEUP_GPIO), ESP_EXT1_WAKEUP_ANY_HIGH);
  rtc_gpio_pulldown_en(WAKEUP_GPIO);  // GPIO33 tied to GND in order to wake up on HIGH
  rtc_gpio_pullup_dis(WAKEUP_GPIO);
#endif

  // Print message and enter deep sleep
  Serial.println("ESP32 going to sleep now...");
  esp_deep_sleep_start();

  Serial.println("This will never be printed");  // This line will never execute
}

void loop() {
  // This code never runs since ESP32 enters deep sleep in setup()
}
