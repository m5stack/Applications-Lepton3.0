#include <M5Stack.h>
#include <Wire.h>
#include <SPI.h>
#include "Lepton.h"
#include "espnow.h"

Espnow espnow;
//uint8_t frameData[19280];
//extern unsigned char temp[240][4];
extern unsigned short  smallBuffer[160 * 120];
extern uint16_t  fpa_temp,aux_temp;
uint8_t frameData[241] = {0};

// Initialize Lepton
// Pin 21 is connected to SDA
// Pin 22 is connected to SCL
// Pin 5 is connected to SPI CS
Lepton lepton(21, 22, 5);
// callback when data is sent from Master to Slave
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  //char macStr[18];
 // snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
    //       mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  // Serial.print("Last Packet Sent to: "); Serial.println(macStr);
  //Serial.print("Last Packet Send Status: ");
  // Serial.println(status == ESP_NOW_SEND_SUCCESS ? "S" : "F");
}

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len)
{
   Serial.print("data_len = ");Serial.println(data_len);
   if(bot)
    espnow.OnBotRecv(mac_addr,data,data_len);//delay(100);
    else
    espnow.OnRemotRecv(mac_addr,data,data_len);    
}

void setup() {
  M5.begin();  
  Serial.begin(115200);
  
  espnow.init();
  esp_now_register_recv_cb(OnDataRecv);
  esp_now_register_send_cb(OnDataSent);
  
  pinMode(13, INPUT);
  digitalWrite(13, HIGH);
  lepton.begin();
  lepton.syncFrame();
  uint16_t SYNC = 5,DELAY = 3,dat;
  lepton.doSetCommand(0x4854, &SYNC, 1);
  lepton.doSetCommand(0x4858, &DELAY, 1);
  //lepton.doGetCommand(0x4858, &dat);
  //Serial.print(" gpio6 = ");Serial.println(dat);
  lepton.end();

  //M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  //M5.Lcd.setTextSize(1);
   M5.Lcd.setCursor(0, 0, 4);
   M5.Lcd.print("Temp = ");
   M5.Lcd.setCursor(150, 0);
   M5.Lcd.print("C");
 // M5.Lcd.print(lidarSpeed);
   //M5.Lcd.print("S/");
  //M5.Lcd.print(packCount);
  //M5.Lcd.setCursor(280, 0, 2);
  
  for(int number = 0; number < 120 * 160; number++)
  {
    smallBuffer[number] =  8000;
  }

  //frameData = (uint8_t *)smallBuffer;
}

void loop() {
#if 0
    lepton.syncFrame();
    lepton.readFrame(frameData);
    lepton.end();
#else
    lepton.getRawValues();
    espnow.BotConnectUpdate();
#endif

#if 1

   uint16_t minValue = 65535, maxValue = 0;
   uint16_t value;
   for(int num = 0; num < 120 * 160; num++)
   {
      if(smallBuffer[num] > maxValue) {
        maxValue = smallBuffer[num];
      }
      if(smallBuffer[num] < minValue) {
         minValue = smallBuffer[num];
      }   
   }

   float diff = (maxValue - minValue);
   
   //float scale = diff/300.0;0x04df
   float scale = (float)(0xf813 - 0xe0ff)/diff ;
   Serial.printf("maxValue %d\n",maxValue);Serial.printf("minValue %d\n",minValue);Serial.printf(" %f\n",diff);Serial.printf(" %f\n",scale);
   float *buf = &scale;
   //Serial.printf("scale %d\n",scale);
   for(int num = 0; num < 120 * 160; num++)
   {
     //frameData[num] = (smallBuffer[num] - minValue) * scale + 0xe0ff;
      smallBuffer[num] = (smallBuffer[num] - minValue) * scale + 0xe0ff;
   }
   
    // M5.Lcd.drawBitmap((int)(M5.Lcd.width()-160)/2 , (int)(M5.Lcd.height()-120)/2, (int)160, (int)120,frameData);
     M5.Lcd.drawBitmap((int)(M5.Lcd.width()-160)/2 , (int)(M5.Lcd.height()-120)/2, (int)160, (int)120,smallBuffer);
      //Serial.printf("fpa_temp = %d,aux_temp = %d\n",fpa_temp, aux_temp);
      float fpatemp = fpa_temp/ 100.0f;  
      float temp=fpatemp-273.15;         // <-----------センサー温度　単位＝℃
      //Serial.printf("fpa_temp = %f\n",temp);
      M5.Lcd.setCursor(0,0);
      M5.Lcd.print("Temp = ");
      M5.Lcd.setCursor(150, 0);
      M5.Lcd.print("C");
      M5.Lcd.setCursor(85, 0);
      M5.Lcd.print(temp);
      /*
      for (int ii = 0; ii < 6; ++ii ) {
        Serial.print((uint8_t) espnow.peer_addr[ii], HEX);
        if (ii != 5) Serial.print(":");
     }
     */
     //esp_err_t result = esp_now_send(slave.peer_addr, frameData, sizeof(ack));
      esp_err_t result;
      result = esp_now_send(espnow.peer_addr, (uint8_t *)buf, sizeof(scale));
      for(int frame = 0; frame < 10 * 8; frame++){
        frameData[0] = frame;
        for(int i = 1; i < 241; i++)
        {
          frameData[i] = (uint8_t)(smallBuffer[frame * 240 + i - 1] - 0xe0ff)/scale;
        }
        while(esp_now_send(espnow.peer_addr, frameData,241) != ESP_OK);
     // esp_err_t result = esp_now_send(slave.peer_addr, ack, sizeof(ack));
         
          Serial.print("Send Status: ");
          if (result == ESP_OK) {
            Serial.println("Success");
          } else if (result == ESP_ERR_ESPNOW_NOT_INIT) {
          // How did we get so far!!
          Serial.println("ESPNOW not Init.");
          } else if (result == ESP_ERR_ESPNOW_ARG) {
          Serial.println("Invalid Argument");
          } else if (result == ESP_ERR_ESPNOW_INTERNAL) {
          Serial.println("Internal Error");
          } else if (result == ESP_ERR_ESPNOW_NO_MEM) {
          Serial.println("ESP_ERR_ESPNOW_NO_MEM");
          } else if (result == ESP_ERR_ESPNOW_NOT_FOUND) {
          Serial.println("Peer not found.");
          } else {
          Serial.println("Not sure what happened");
          }
         
      }
      
#endif

 }
