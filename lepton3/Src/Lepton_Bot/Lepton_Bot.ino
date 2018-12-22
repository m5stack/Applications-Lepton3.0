#include <M5Stack.h>
#include <Wire.h>
#include <SPI.h>
#include "Lepton.h"
#include "espnow.h"
#define expd 0
Espnow espnow;
char Title[] = {0x08,0x40,0x08,0x40,0x08,0x48,0x7F,
                0xFC,0x08,0x48,0x0A,0x48,0x1C,0xC8,
                0x68,0x48,0x08,0xA8,0x08,0x8A,0x29,
                0x0A,0x12,0x04,0x00,0x00,0x48,0x88,
                0x44,0x46,0x84,0x42,
                0x00,0x80,0x00,0xA0,0x00,0x90,0x3F,
                0xFC,0x20,0x80,0x20,0x80,0x20,0x84,
                0x3E,0x44,0x22,0x48,0x22,0x48,0x22,
                0x30,0x2A,0x20,0x24,0x62,0x40,0x92,
                0x81,0x0A,0x00,0x06,
                0x09,0x00,0x09,0xF0,0x0A,0x24,0x17,
                0xFE,0x1C,0x44,0x37,0xFC,0x50,0x80,
                0x91,0x44,0x16,0x68,0x10,0xB0,0x17,
                0x30,0x10,0x68,0x11,0xA6,0x16,0x20,
                0x10,0xA0,0x10,0x40,
                0x08,0x00,0x08,0x80,0x08,0x64,0x12,
                0x24,0x12,0x04,0x32,0x08,0x51,0x08,
                0x91,0x10,0x10,0x90,0x10,0xA0,0x10,
                0x40,0x10,0xA0,0x11,0x10,0x12,0x08,
                0x14,0x0E,0x18,0x04};
uint8_t IndexData[19280];
extern unsigned short  smallBuffer[160 * 120];
unsigned short expframeData[320 * 9];
extern uint16_t  fpa_temp,aux_temp;
uint8_t frameData[241] = {0};
extern const unsigned short camColors[];
float temp;
float value_max;
float value_min;
Lepton lepton(21, 22, 5);
void display_ch(int16_t x0, int16_t y0,
                            int16_t w, int16_t h){
  uint16_t posX = x0, posY = y0;

   for(int count = 0; count < 4;count++){
    int k,j,i; 
    byte flag;
    for(k=0; k<16; k++){
        for(j=0; j<2; j++){
            for(i=0; i<8; i++){
                flag = Title[count * 32 +k*2+j]&(0x80 >> i);
                if(flag)
                  M5.Lcd.drawPixel(posX + 8 * j + i, posY + k, WHITE);
                else
                  M5.Lcd.drawPixel(posX + 8 * j + i, posY + k, BLACK);
            }
        }
        M5.Lcd.println();
    }
    posX += 20;
  }
}
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{

}

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len)
{
  // Serial.print("data_len = ");Serial.println(data_len);
   if(bot)
    espnow.OnBotRecv(mac_addr,data,data_len);
    else
    espnow.OnRemotRecv(mac_addr,data,data_len);    
}
/***infodisplay()*****/
void infodisplay(void) {

  M5.Lcd.setCursor(58, 0, 4);
  //M5.Lcd.print("THERMOGRAPHY");
  display_ch(120, 3,122, 24);

  M5.Lcd.fillRect(0, 206, 320, 4, TFT_WHITE);
  M5.Lcd.setTextColor(TFT_WHITE);
  M5.Lcd.fillRect(0, 22, 320, 4, TFT_WHITE);
  M5.Lcd.setTextColor(TFT_WHITE);
}
int max_num,old_max_num;
void task1(void * pvParameters) {

    for(;;) {
        delay(100);
#if 1
        
        uint8_t data[10];
        data[0] = (uint8_t)(max_num>>8);
        data[1] = (uint8_t)(max_num&0xff);
        if(temp >= 120) return;
        data[2] = (uint8_t)((uint16_t)(temp * 10)>>8);
        data[3] = (uint8_t)((uint16_t)(temp * 10)&0xff);
        if(value_max >= 120) return;
        data[4] = (uint8_t)((uint16_t)(value_max * 10)>>8);
        data[5] = (uint8_t)((uint16_t)(value_max * 10)&0xff);
        if(value_min >= 120) return;
        data[6] = (uint8_t)((uint16_t)(value_min * 10)>>8);
        data[7] = (uint8_t)((uint16_t)(value_min * 10)&0xff);
        esp_now_send(espnow.peer_addr, data,10);


      for(byte frame = 0; frame < 10 * 8; frame++){
        frameData[0] = frame;
        for(int i = 1; i < 241; i++){
          frameData[i] =  IndexData[frame * 240 + i - 1];
        }
        while(esp_now_send(espnow.peer_addr, frameData,241) !=ESP_OK) ;
      }
      #endif 
    }
}

void setup() {
  delay(100);
  M5.begin();  
  espnow.BotInit();
  esp_now_register_recv_cb(OnDataRecv);
  esp_now_register_send_cb(OnDataSent);
  pinMode(13, INPUT);
  digitalWrite(13, HIGH);
  lepton.begin();
  lepton.syncFrame();
  uint16_t SYNC = 5,DELAY = 3,oen = 1,dat;
  lepton.doSetCommand(0x4854, &SYNC, 1);
  lepton.doSetCommand(0x4858, &DELAY, 1);
  //lepton.doSetCommand(0x4E10, &oen, 1);
  //lepton.doGetCommand(0x4E10, &dat);
  //Serial.print(" gpio6 = ");Serial.println(dat);
  lepton.end();
  infodisplay();

   // Task 1
  xTaskCreatePinnedToCore(
                    task1,     /* Function to implement the task */
                    "task1",   /* Name of the task */
                    4096,      /* Stack size in words */
                    NULL,      /* Task input parameter */
                    1,         /* Priority of the task */
                    NULL,      /* Task handle. */
                    0); 
}
void CursorLocation(uint16_t num, float fpa_temp,float max_temp,float min_temp){
#if expd
  int centre_x = (int)(M5.Lcd.width()-160)/2 +  num % 160;
  int centre_y = (int)(M5.Lcd.height()-120)/2 + num / 160; 
  
  if(centre_x <= (int)(M5.Lcd.width()-160)/2+10)
   centre_x = (int)(M5.Lcd.width()-160)/2+10;
  else if(centre_x >= (int)(M5.Lcd.width()-160)/2 + 160 - 12)
   centre_x = (int)(M5.Lcd.width()-160)/2 + 160 - 12;


   if(centre_y <= (int)(M5.Lcd.height()-120)/2+10)
   centre_y = (int)(M5.Lcd.height()-120)/2+10;
  else if(centre_y >= (int)(M5.Lcd.height()-120)/2 + 120 - 11)
   centre_y = (int)(M5.Lcd.height()-120)/2 + 120 - 11;
 #else
  int centre_x = num % 160;
  int centre_y = num / 160; 
  centre_x = centre_x * 2;
  centre_y = 27 + centre_y * 3/2;

  if(centre_y <= (int)(27+10))
   centre_y = 27+10;
  else if(centre_y >= (int)(207 - 11))
   centre_y = 207 - 11;
 #endif
  M5.Lcd.drawCircle(centre_x, centre_y, 6, TFT_WHITE);     // update center spot icon
  M5.Lcd.drawLine(centre_x,    centre_y-10, centre_x, centre_y+10, TFT_WHITE); // vertical line
  M5.Lcd.drawLine(centre_x-10, centre_y, centre_x+10, centre_y, TFT_WHITE); // horizontal lin

  if(centre_x >= ((int)(M5.Lcd.width()-160)/2 + 160 - 12 - 45))
  M5.Lcd.setCursor(centre_x-55, centre_y-10,2); 
  else
  M5.Lcd.setCursor(centre_x+20, centre_y-10,2); 
  M5.Lcd.print(max_temp, 1);
  M5.Lcd.printf("C" ,1);
#if 0
  M5.Lcd.fillRect(270, 160, 60, 15, TFT_BLACK); //Clear fps text area
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(250, 160);
  M5.Lcd.print("aux:" + String(max_temp)+"C");
#endif
  //M5.Lcd.fillRect(270, 180, 60, 15, TFT_BLACK); //Clear fps text area
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(250, 190);
  M5.Lcd.print("fpa:" + String(max_temp)+"C");
  int icolor = 0;
  for (int icol = 0; icol <= 360;  icol++){
    //彩色条
    //M5.Lcd.drawRect(36, 208, icol, 284 , camColors[icolor]);
    M5.Lcd.drawRect(0, 210, icol, 284 , camColors[icolor]);
    if(icolor <= 250)
    icolor++;
  }
  M5.Lcd.setCursor(30, 215,4); 
  M5.Lcd.printf("min = %0.1fC max = %0.1fC",min_temp,max_temp);
}
void loop() {
  espnow.BotConnectUpdate();
#if 0
    lepton.syncFrame();
    lepton.readFrame(frameData);
    lepton.end();
#else
error1:
    lepton.getRawValues();
#endif

#if 1
   float fpatemp = fpa_temp/100.0f;  
   temp = fpatemp-273.15;     
   uint16_t minValue = 65535, maxValue = 0;
   uint16_t value;
   for(int num = 0; num < 120 * 160; num++)
   {
      if(smallBuffer[num] > maxValue) {
        maxValue = smallBuffer[num];
        max_num = num;
      }
      if(smallBuffer[num] < minValue) {
         minValue = smallBuffer[num];
      }   
   }
   //delay(10); 
   //Serial.printf("maxValue %d\n",maxValue);Serial.printf("minValue %d\n",minValue);
#if 1
   float diff = (maxValue - minValue);
   diff = diff / 256.0f;
   if(diff < 0.56f) diff = 0.56f; 
   float fpatemp_f = fpatemp * 1.8f - 459.67f;
   value_min = ((0.05872 * (float)minValue - 472.22999f + fpatemp_f));
   value_min= (value_min - 32.0f) / 1.8f;
   value_max = ((0.05872 * (float)maxValue - 472.22999f + fpatemp_f));
   value_max = (value_max - 32.0f) / 1.8f;
   if(value_min < -100 || value_max > 500){
      Serial.println("Error!");
      goto error1;
   }

   value_min = 0.0217 *((float)minValue - 8292)+ fpatemp - 273.16;
   value_max = 0.0217 *((float)maxValue - 8292)+ fpatemp - 273.16;
#else   
   float diff = (maxValue - minValue);
   if(diff >= 255) diff = 255;
   //float scale = diff/300.0;0x04df
   float scale = (float)(0xf813 - 0xe0ff)/diff ;
   //Serial.printf("maxValue %d\n",maxValue);Serial.printf("minValue %d\n",minValue);Serial.printf(" %f\n",diff);Serial.printf(" %f\n",scale);
   //Serial.printf("maxValue %d\n",maxValue);Serial.printf("minValue %d\n",minValue);Serial.printf(" %f\n",diff);Serial.printf(" %f\n",scale);
   float *buf = &scale;
 #endif
   int colorIndex = 0;
   for(int num = 0; num < 120 * 160; num++){
     colorIndex = (smallBuffer[num] - minValue)/diff;
     if(colorIndex <= 0) colorIndex = 0;  
     if(colorIndex > 255) colorIndex = 255;
     IndexData[num] = colorIndex;
     smallBuffer[num] = camColors[colorIndex];
   }
#if expd
   M5.Lcd.drawBitmap((int)(M5.Lcd.width()-160)/2 , (int)(M5.Lcd.height()-120)/2, (int)160, (int)120,smallBuffer);
   CursorLocation(max_num,temp,value_max,value_min);
#else
  for(int z = 0; z < 20; z++){
    for(int rows = 0; rows < 9; rows++){
      for(int columns = 0; columns < 320;columns++){
        expframeData[rows * 320 + columns] = smallBuffer[960 * z + rows * 2 / 3 * 160 + columns/2];
      }
    }
    M5.Lcd.drawBitmap(0, 27 + 9 * z, 320, 10,expframeData);
  }
  CursorLocation(max_num,temp,value_max,value_min);
#endif 
#if 0
   uint8_t data[10];
   data[0] = (uint8_t)(max_num>>8);
   data[1] = (uint8_t)(max_num&0xff);
   data[2] = (uint8_t)((uint16_t)(temp * 10)>>8);
   data[3] = (uint8_t)((uint16_t)(temp * 10)&0xff);
   data[4] = (uint8_t)((uint16_t)(value_max * 10)>>8);
   data[5] = (uint8_t)((uint16_t)(value_max * 10)&0xff);
   data[6] = (uint8_t)((uint16_t)(value_min * 10)>>8);
   data[7] = (uint8_t)((uint16_t)(value_min * 10)&0xff);
   esp_now_send(espnow.peer_addr, data,10);
   esp_err_t result;
  #if 0
    for(byte frame = 0; frame < 10 * 8 * 2; frame++){
      frameData[0] = frame;
      for(int i = 1; i < 241; i++){
        if(i%2)
        frameData[i] =  smallBuffer[frame * 120 + (i - 1)/2] & 0x00ff;
        else
        frameData[i] =  (smallBuffer[frame * 120 + (i - 1)/2] >> 8);
      }
      esp_now_send(espnow.peer_addr, frameData,241);
  #else
      for(byte frame = 0; frame < 10 * 8; frame++){
      frameData[0] = frame;
      for(int i = 1; i < 241; i++)
      {
        frameData[i] =  IndexData[frame * 240 + i - 1];
      }
      while(esp_now_send(espnow.peer_addr, frameData,241) !=ESP_OK) ;
  #endif  
      }  
   #endif   
#endif

 }
