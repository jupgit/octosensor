///////////////////////////////////////////////////////////
//
//  O C T O  S E N S O R
//  A wireless system with 8 laser range finder modules
//  and a central unit to be used in interactive
//  art installations.
//
//  Project by Juliano Prado (2021) - Brazil
//
//  Revision: 03.2023
//  Display port to LilyGO T-Embed (ST7789 320x170): 08.2026
//
//  https://github.com/jupgit/octo-sensor
//
//  TF LUNA I2C VERSION
//
//  Colaborators:
//   Paulo Cesar Teles (PhD)
//   Klebert
//   Mariana Arias
//   Igor
//
//
///////////////////////////////////////////////////////////

///
///     C E N T R A L   -   L I L Y G O   T - E M B E D
///
//  NOTA: depois de gravar entrando no modo bootloader (BOOT+RESET),
//  aperte RESET uma vez para o sketch rodar.

// ARRAY WITH 8 SENSORS READINGS

int octoSensor[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int previousOctoSensor[8] = {1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023};

#include <esp_now.h>
#include <WiFi.h>

// ===========================================================
// DISPLAY  ->  LilyGO T-Embed  (ST7789 320x170, SPI)
// Substitui o antigo OLED SSD1306 (I2C).
// Os pinos do TFT sao definidos no User_Setup.h da TFT_eSPI
// (o mesmo que voce usou no exemplo TFT_Rainbow).
// ===========================================================
#include <SPI.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

// Pinos de controle da placa T-Embed
#define PIN_POWER_ON   46   // habilita o trilho de alimentacao da placa
#define PIN_LCD_BL     15   // backlight do LCD

// Cores do layout
#define COL_TITLE   TFT_CYAN
#define COL_LABEL   TFT_WHITE
#define COL_VALUE   TFT_GREEN
#define COL_BG      TFT_BLACK

// REPLACE WITH THE MAC Address of your receiver
//uint8_t broadcastAddress[] = {0x7C, 0x9E, 0xBD, 0x45, 0xB3, 0xC0}; // OCTOSENSOR #4
uint8_t broadcastAddress[] = {0xC4, 0x4F, 0x33, 0x3E, 0xE7, 0x6D};

// CENTRAL T-EMBED


// Define variables to store readings to be sent
int octoCommand;
int octoNode;
int octoValue;

// Define variables to store incoming readings
int incomingCommand;
int incomingNode;
int incomingValue;

// Variable to store if sending data was successful
String success;

//Structure example to send data
//Must match the receiver structure
typedef struct struct_message {
    int comm;
    int node;
    int value;
} struct_message;

// Create a struct_message called BME280Readings to hold sensor readings
struct_message OCTOReadings;

// Create a struct_message to hold incoming sensor readings
struct_message incomingReadings;

esp_now_peer_info_t peerInfo;


//   TTTTT    X   X
//     T       X X
//     T        X
//     T       X X
//     T      X   X


// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  //Serial.print("\r\nLast Packet Send Status:\t");
  //Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status ==0){
    success = "TX OK";
  }
  else{
    success = "TX Fail :(";
  }
}

//   RRRR    X   X
//   R   R    X X
//   RRRR      X
//   R R      X X
//   R  R    X   X

// Callback when data is received
// Assinatura compativel com core 2.0.x (mac) e 3.x (esp_now_recv_info_t)
#if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0)
void OnDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *incomingData, int len) {
#else
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
#endif
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  //Serial.print("Bytes received: ");
  //Serial.println(len);
  incomingCommand = incomingReadings.comm;
  incomingNode = incomingReadings.node;
  incomingValue = incomingReadings.value;


//////////////////////////////////////////////////////////////////////////////////////
// REMOTE SENSOR VALUE into its position in Array
   octoSensor[incomingNode-1] = incomingValue;
//////////////////////////////////////////////////////////////////////////////////////

}





// *******************************************************
//
//                  S   E   T   U   P
//
// *******************************************************


void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

  // ---- Display T-Embed ----
  pinMode(PIN_POWER_ON, OUTPUT);
  digitalWrite(PIN_POWER_ON, HIGH);   // liga a alimentacao da placa
  pinMode(PIN_LCD_BL, OUTPUT);
  digitalWrite(PIN_LCD_BL, HIGH);     // liga o backlight

  tft.init();
  tft.setRotation(1);                 // 320x170 paisagem
  drawStaticUI();                     // desenha titulo e rotulos fixos

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    //Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    //Serial.println("Failed to add peer");
    return;
  }
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}







// *******************************************************
//
//                    L  O   O   P
//
// *******************************************************



void loop() {

  /// TROCAR POR MILLIS
  getReadings();

  // Set values to send
  OCTOReadings.comm = octoCommand;
  OCTOReadings.node = octoNode;
  OCTOReadings.value = octoValue;

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) & OCTOReadings, sizeof(OCTOReadings));

  if (result == ESP_OK) {
    //Serial.println("Sent with success");
  }
  else {
    //Serial.println("Error sending the data");
  }

  isadoraOutput();

  updateDisplay();



  delay(25);
}





////
////
////  F U N C T I O N S
////
////



void getReadings(){
  octoCommand = 1;
  octoNode = 4;
  octoValue = int(millis()/1000);
  octoValue = random(200,1000);
}


void isadoraOutput() {

  //ISADORA

      for (int i=0; i<8; i++) {
        Serial.print(i+1,DEC); // Canal 1 do Isadora
        Serial.print(octoSensor[i]); // Envia sensor 1
        Serial.println(); //Send a value to eom

      }

      /*
      Serial.print(1,DEC); // Canal 1 do Isadora
      Serial.print(octoSensor[0]); // Envia sensor 1
      Serial.println(); //Send a value to eom

      Serial.print(2,DEC); // Canal 2 do Isadora
      Serial.print(octoSensor[1]); // Envia sensor 2
      Serial.println(); //Send a value to eom

      Serial.print(3,DEC); // Canal 2 do Isadora
      Serial.print(octoSensor[2]); // Envia sensor 2
      Serial.println(); //Send a value to eom


      Serial.print(4,DEC); // Canal 2 do Isadora
      Serial.print(octoSensor[3]); // Envia sensor 2
      Serial.println(); //Send a value to eom

      Serial.print(5,DEC); // Canal 2 do Isadora
      Serial.print(octoSensor[4]); // Envia sensor 2
      Serial.println(); //Send a value to eom

      Serial.print(6,DEC); // Canal 2 do Isadora
      Serial.print(octoSensor[5]); // Envia sensor 2
      Serial.println(); //Send a value to eom

      Serial.print(7,DEC); // Canal 2 do Isadora
      Serial.print(octoSensor[6]); // Envia sensor 2
      Serial.println(); //Send a value to eom

      Serial.print(8,DEC); // Canal 2 do Isadora
      Serial.print(octoSensor[7]); // Envia sensor 2
      Serial.println(); //Send a value to eom
*/


}




// ===========================================================
//  DISPLAY  (T-Embed / TFT_eSPI 320x170)
// ===========================================================

// Posicoes das duas colunas (1-4 na esquerda, 5-8 na direita)
#define ROW_Y0     44    // y da primeira linha
#define ROW_STEP   30    // espacamento vertical entre linhas
#define COL_L_LBL  10    // x do rotulo, coluna esquerda
#define COL_L_VAL  55    // x do valor,  coluna esquerda
#define COL_R_LBL  180   // x do rotulo, coluna direita
#define COL_R_VAL  225   // x do valor,  coluna direita

// Desenha titulo e rotulos fixos uma unica vez (evita flicker)
void drawStaticUI(){
  tft.fillScreen(COL_BG);

  tft.setTextSize(2);
  tft.setTextColor(COL_TITLE, COL_BG);
  tft.setCursor(6, 8);
  tft.print("OCTOsensor READINGS");

  tft.setTextColor(COL_LABEL, COL_BG);
  for (int i = 0; i < 4; i++) {
    tft.setCursor(COL_L_LBL, ROW_Y0 + i * ROW_STEP);
    tft.printf("%d:", i + 1);            // 1..4
    tft.setCursor(COL_R_LBL, ROW_Y0 + i * ROW_STEP);
    tft.printf("%d:", i + 5);            // 5..8
  }
}

// Escreve um valor com fundo, alinhado a direita e largura fixa
// (os espacos apagam digitos antigos -> sem flicker, sem clearScreen)
void printValue(int x, int y, int v){
  char buf[8];
  snprintf(buf, sizeof(buf), "%4d", v);
  tft.setCursor(x, y);
  tft.print(buf);
}

void updateDisplay(){
  // Apenas os valores sao redesenhados a cada loop
  tft.setTextSize(2);
  tft.setTextColor(COL_VALUE, COL_BG);

  for (int i = 0; i < 4; i++) {
    printValue(COL_L_VAL, ROW_Y0 + i * ROW_STEP, octoSensor[i]);       // 1..4
    printValue(COL_R_VAL, ROW_Y0 + i * ROW_STEP, octoSensor[i + 4]);   // 5..8
  }
}
