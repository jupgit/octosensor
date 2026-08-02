///////////////////////////////////////////////////////////
//
//  E S P - N O W   S N I F F E R
//
//  Puts the ESP32 into Wi-Fi promiscuous mode and prints
//  every ESP-NOW packet it hears:
//     source MAC  ->  destination MAC   (+ channel, RSSI)
//
//  Use it to discover which board is which, and to see what
//  address a node is actually transmitting to.
//
//  Board:  any ESP32   (Arduino core 3.x)
//  Serial Monitor @ 115200
//
//  NOTE: the sniffer must be on the SAME Wi-Fi channel as the
//  transmitter. Channel hopping is ON by default so it sweeps
//  1..13 automatically. If you already know the channel, set
//  CHANNEL_HOP to false and START_CHANNEL to that channel to
//  avoid missing packets while hopping.
//
///////////////////////////////////////////////////////////

#include <WiFi.h>
#include "esp_wifi.h"

// ---------------- CONFIG ----------------
#define CHANNEL_HOP      true   // false = lock to START_CHANNEL
#define HOP_INTERVAL_MS  2500   // dwell time per channel when hopping
#define START_CHANNEL    1
#define MAX_CHANNEL      13     // use 11 in the US, 13 elsewhere
// ----------------------------------------

// Espressif OUI carried by ESP-NOW vendor-specific action frames
const uint8_t ESPNOW_OUI[3] = { 0x18, 0xFE, 0x34 };

// 802.11 MAC header layout
typedef struct {
  uint16_t frame_ctrl;
  uint16_t duration;
  uint8_t  addr1[6];   // destination / receiver  ("transmitting TO")
  uint8_t  addr2[6];   // source / transmitter     (the sender)
  uint8_t  addr3[6];   // BSSID
  uint16_t seq_ctrl;
} __attribute__((packed)) wifi_hdr_t;

uint8_t       curChannel = START_CHANNEL;
unsigned long lastHop    = 0;

void printMac(const uint8_t *m) {
  Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X",
                m[0], m[1], m[2], m[3], m[4], m[5]);
}

bool isBroadcast(const uint8_t *m) {
  for (int i = 0; i < 6; i++) if (m[i] != 0xFF) return false;
  return true;
}

// Promiscuous RX callback
void sniffer_cb(void *buf, wifi_promiscuous_pkt_type_t type) {
  if (type != WIFI_PKT_MGMT) return;                 // ESP-NOW = mgmt action frame

  const wifi_promiscuous_pkt_t *pkt = (wifi_promiscuous_pkt_t *)buf;
  const uint8_t *p = pkt->payload;
  int len = pkt->rx_ctrl.sig_len;

  if (len < 30)                     return;           // too short to be one
  if (p[0] != 0xD0)                 return;           // management / action subtype
  if (p[24] != 0x7F)                return;           // vendor-specific action category
  if (memcmp(&p[25], ESPNOW_OUI, 3) != 0) return;     // Espressif ESP-NOW only

  const wifi_hdr_t *hdr = (const wifi_hdr_t *)p;

  Serial.printf("[CH %2d  RSSI %4d]  src ",
                pkt->rx_ctrl.channel, pkt->rx_ctrl.rssi);
  printMac(hdr->addr2);
  Serial.print("  ->  dst ");
  printMac(hdr->addr1);
  if (isBroadcast(hdr->addr1)) Serial.print("   (BROADCAST)");
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  delay(300);
  Serial.println();
  Serial.println("=========================================");
  Serial.println("        ESP-NOW SNIFFER");
  Serial.println("=========================================");

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  Serial.print("Sniffer own MAC: ");
  Serial.println(WiFi.macAddress());

  // promiscuous mode, management frames only
  esp_wifi_set_promiscuous(false);
  wifi_promiscuous_filter_t filter = {};
  filter.filter_mask = WIFI_PROMIS_FILTER_MASK_MGMT;
  esp_wifi_set_promiscuous_filter(&filter);
  esp_wifi_set_promiscuous_rx_cb(&sniffer_cb);
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(curChannel, WIFI_SECOND_CHAN_NONE);

  Serial.printf("Channel hop: %s   (start CH %d)\n",
                CHANNEL_HOP ? "ON, sweeping 1..13" : "OFF", curChannel);
  Serial.println("Listening for ESP-NOW traffic...");
  Serial.println("-----------------------------------------");
  lastHop = millis();
}

void loop() {
  if (CHANNEL_HOP && (millis() - lastHop > HOP_INTERVAL_MS)) {
    curChannel++;
    if (curChannel > MAX_CHANNEL) curChannel = START_CHANNEL;
    esp_wifi_set_channel(curChannel, WIFI_SECOND_CHAN_NONE);
    lastHop = millis();
    Serial.printf("... now scanning channel %d\n", curChannel);
  }
}
