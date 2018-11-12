#ifndef __ESPNOW_H__
#define __ESPNOW_H__
#define bot 0
#include<M5Stack.h>
#include<WiFi.h>
#include<esp_now.h>
#include<Wire.h>
#include <Preferences.h>


#include "Arduino.h"


#define CHANNEL 1
#define PRINTSCANRESULTS 0


class Espnow {

   
public:
  Espnow();
  void init(void);
  
  
  //void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
  //void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len);

   void OnRemotSent(const uint8_t *mac_addr, esp_now_send_status_t status);
   void OnRemotRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len);
   
   void OnBotSent(const uint8_t *mac_addr, esp_now_send_status_t status); 
   void OnBotRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len);

   void RemoteConnectUpdate(void);
   void BotConnectUpdate(void);
public:

private:
  void ConfigDeviceAP(void);
  void InitEspNow(void);
  void ScanForSlave(void);
  void manageSlave(void); 
  void sendData(void);

  
private:
  Preferences preferences;
  uint8_t peer_addr[6]= {0xB4, 0xE6, 0x2D, 0x8B, 0x7D, 0x42};
  uint8_t sta_addr[6];
  esp_now_peer_info_t slave;
  esp_now_peer_info_t slaves[20] = {};
  int SlaveCnt = 0;
  bool connectflag = false; 
  uint8_t connect_addr[20][6];
  int connect_num;
};
#endif
