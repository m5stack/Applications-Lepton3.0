#include <M5Stack.h>
#include"espnow.h"
Espnow espnow;
uint16_t frameData[160 * 120];
uint8_t TempData[160 * 120];
//byte scale[4] = 0;
float scale;
uint8_t *buf = (uint8_t *)&scale;

// callback when data is sent from Master to Slave
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  
}

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len)
{
   Serial.print("data_len = ");Serial.println(data_len);
   if(bot)
    espnow.OnBotRecv(mac_addr,data,data_len);//delay(100);
    else
    espnow.OnRemotRecv(mac_addr,data,data_len);
    
    if(data_len == 241)
    { Serial.print("data[0] = ");Serial.println(data[0]);
      for(int i = 1; i < 241; i++)   {
      //M5.Lcd.drawBitmap((int)(M5.Lcd.width()-160)/2 , (int)(M5.Lcd.height()-120)/2, (int)160, (int)120,smallBuffer); 
      
      
      frameData[data[0] * 240 + i - 1] = data[i] + 0xe0ff;    }
    }
    if(data_len == 4)
    {
     buf[0] = data[0];  
     buf[1] = data[1]; 
     buf[2] = data[2]; 
     buf[3] = data[3]; 
     //scale = (float)data;
     Serial.print("scale = ");Serial.println(scale);
    }
    
    
}


void setup() {
  // put your setup code here, to run once:
  /*
  for(int i = 0; i < 120 * 160 * 2; i++)
  {
    if(i % 2) 
    frameData[i] = 0x1f;
    else
    frameData[i] = 0x40;
   }
  */
  espnow.init();
  esp_now_register_recv_cb(OnDataRecv);
  esp_now_register_send_cb(OnDataSent);

  
    //frameData[i] = 1f;

  
}
int UpdateFlag = 0;
void loop() {
  // put your main code here, to run repeatedly:
    //Serial.println("Pair success...."); 
    Serial.println("Pair success....");
    //espnow.RemoteConnectUpdate();
    Serial.println("Pair success....");
    Serial.println("Pair success....");
    if(bot)
    espnow.BotConnectUpdate();
    else
    espnow.RemoteConnectUpdate();
    M5.Lcd.drawBitmap((int)(M5.Lcd.width()-160)/2 , (int)(M5.Lcd.height()-120)/2, (int)160, (int)120,frameData);
}
