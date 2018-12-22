#include <M5Stack.h>
#include"espnow.h"
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
uint16_t frameData[160 * 120];
uint16_t expframeData[320 * 9];
uint8_t TempData[160 * 120];
uint16_t num; 
float fpa_temp;
float max_temp;
float min_temp;
extern const unsigned short camColors[];

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
// callback when data is sent from Master to Slave
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  
}

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len)
{
   if(bot)
    espnow.OnBotRecv(mac_addr,data,data_len);
    else
    espnow.OnRemotRecv(mac_addr,data,data_len);


    if(data_len == 10){
      num = (data[0] << 8 | data[1]);
      fpa_temp = (float)((data[2] << 8 | data[3])) / 10.0;
      max_temp = (float)((data[4] << 8 | data[5])) / 10.0;
      min_temp = (float)((data[6] << 8 | data[7])) / 10.0;
    }
    
    if(data_len == 241){ 
        for(int i = 1; i < 241; i++)
         TempData[data[0] * 240 + i - 1] = data[i];
    }    
}

void infodisplay(void) {

  M5.Lcd.setCursor(58, 0, 4);
  //M5.Lcd.print("THERMOGRAPHY");
  display_ch(120, 3,122, 24);

  M5.Lcd.fillRect(0, 206, 320, 4, TFT_WHITE);
  M5.Lcd.setTextColor(TFT_WHITE);
  M5.Lcd.fillRect(0, 22, 320, 4, TFT_WHITE);
  M5.Lcd.setTextColor(TFT_WHITE);
}
float old_fpa_temp,old_max_temp,old_min_temp;
void CursorLocation(int num, float fpa_temp,float max_temp,float min_temp){

  if(fpa_temp >= 120) fpa_temp = old_fpa_temp;
  if(max_temp >= 120) max_temp = old_max_temp;
  if(min_temp >= 120) min_temp = old_min_temp;
  old_fpa_temp = fpa_temp;
  old_max_temp = max_temp;
  old_min_temp = min_temp;
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
void setup() {
  // put your setup code here, to run once:
  delay(100);
  m5.begin();
  espnow.RemoteInit();
  esp_now_register_recv_cb(OnDataRecv);
  esp_now_register_send_cb(OnDataSent); 
  infodisplay();
}
void loop() {
    if(bot)
    espnow.BotConnectUpdate();
    else
    espnow.RemoteConnectUpdate();
    for(int num = 0; num < 120 * 160; num++)
     frameData[num] = camColors[TempData[num]];
#if expd
   M5.Lcd.drawBitmap((int)(M5.Lcd.width()-160)/2 , (int)(M5.Lcd.height()-120)/2, (int)160, (int)120,frameData);
   CursorLocation(num,fpa_temp,max_temp,min_temp);
#else
    for(int z = 0; z < 20; z++){
      for(int rows = 0; rows < 9; rows++){
        for(int columns = 0; columns < 320;columns++){
          expframeData[rows * 320 + columns] = frameData[960 * z + rows * 2 / 3 * 160 + columns/2];
        }
      }
      M5.Lcd.drawBitmap(0, 27 + 9 * z, 320, 10,expframeData);
    }
    CursorLocation(num,fpa_temp,max_temp,min_temp);
#endif 
    
}
