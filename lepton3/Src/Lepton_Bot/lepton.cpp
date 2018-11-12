#include "Arduino.h"
#include "Lepton.h"
#include "Wire.h"
#include "SPI.h"
#define rotationHorizont 0
#define rotationVert 0
static byte leptonFrame[164];
unsigned short  smallBuffer[160 * 120];
//unsigned char temp[240][4] = {0};
//byte  segm = 0;
uint16_t aux_temp = 0, fpa_temp = 0;
Lepton::Lepton(int sdaPin, int sclPin, int ssPin) : _sdaPin(sdaPin), _sclPin(sclPin), _ssPin(ssPin) {
}
void Lepton::begin() {
  
  Wire.begin(_sdaPin, _sclPin);

  pinMode(_ssPin, OUTPUT);
  digitalWrite(_ssPin, HIGH);

  SPI.begin(); 
}

uint16_t Lepton::readRegister(uint16_t reg) {
  setRegister(reg);
  Wire.requestFrom(DEVICE_ID, (uint8_t)2);
  return readWord();
}

void Lepton::writeRegister(uint16_t reg, uint16_t value) {
  startTransmission(reg);
  transmitWord(value);
  endTransmission();
}

/* Get one line package from the Lepton */
int Lepton::getPackage(byte line, byte seg)
{

  SPI.transferBytes(0x0000,leptonFrame,164); 
   
  if((leptonFrame[0] & 0x0F) == 0x0F)
    return 1;
    
  //Check if the line number matches the expected line
  if (leptonFrame[1] != line)
   return 2;

  //For the Lepton3.x, check if the segment number matches
  if (line == 20)
  {
    byte segment = (leptonFrame[0] >> 4);
    if (segment == 0)
      return 3;
    if (segment != seg)
      return 4;
  }

/*
  temp[(seg  - 1)* 60 + line][0] = seg;
  temp[(seg - 1) * 60 + line][1] = line;

  temp[(seg  - 1)* 60 + line][2] = (leptonFrame[0] >> 4);
  temp[(seg - 1) * 60 + line][3] = leptonFrame[1];
  */
  
  //Everything worked
  return 0;
}

/* Store one package of 80 columns into RAM */
bool Lepton::savePackage(byte line, byte segment)
{
  //Go through the video pixels for one video line
  for (int column = 0; column < 80; column++)
  {
    //Apply horizontal mirroring
    if (rotationHorizont)
      column = 79 - column;

    //Make a 16-bit rawvalue from the lepton frame
    uint16_t result = (uint16_t) (leptonFrame[2 * column + 4] << 8
        | leptonFrame[2 * column + 5]);

    //Discard horizontal mirroring
    if (rotationHorizont)
      column = 79 - column;

    //Invalid value, return
    if (result == 0)
    {
      return 0;
    }
/*
    //Lepton2.x
    if ((leptonVersion != leptonVersion_3_0_shutter)
        && (leptonVersion != leptonVersion_3_5_shutter))
    {
      //Rotated or old hardware version
      if (((mlx90614Version == mlx90614Version_old) && (!rotationVert))
          || ((mlx90614Version == mlx90614Version_new)
              && (rotationVert)))
      {
        smallBuffer[(line * 2 * 160) + (column * 2)] = result;
        smallBuffer[(line * 2 * 160) + (column * 2) + 1] = result;
        smallBuffer[(line * 2 * 160) + 160 + (column * 2)] = result;
        smallBuffer[(line * 2 * 160) + 160 + (column * 2) + 1] = result;
      }
      //Non-rotated
      else
      {
        smallBuffer[19199 - ((line * 2 * 160) + (column * 2))] = result;
        smallBuffer[19199 - ((line * 2 * 160) + (column * 2) + 1)] =
            result;
        smallBuffer[19199 - ((line * 2 * 160) + 160 + (column * 2))] =
            result;
        smallBuffer[19199 - ((line * 2 * 160) + 160 + (column * 2) + 1)] =
            result;
      }
    }
*/
    //Lepton3
   // else
    {
      //Non-rotated
      if (!rotationVert)
      {
        switch (segment)
        {
        case 1:
          if (rotationHorizont)
            smallBuffer[19199
                - (((line / 2) * 160) + ((1 - (line % 2)) * 80)
                    + (column))] = result;
          else
            smallBuffer[19199
                - (((line / 2) * 160) + ((line % 2) * 80)
                    + (column))] = result;
          break;
        case 2:
          if (rotationHorizont)
            smallBuffer[14399
                - (((line / 2) * 160) + ((1 - (line % 2)) * 80)
                    + (column))] = result;
          else
            smallBuffer[14399
                - (((line / 2) * 160) + ((line % 2) * 80)
                    + (column))] = result;
          break;
        case 3:
          if (rotationHorizont)
            smallBuffer[9599
                - (((line / 2) * 160) + ((1 - (line % 2)) * 80)
                    + (column))] = result;
          else
            smallBuffer[9599
                - (((line / 2) * 160) + ((line % 2) * 80)
                    + (column))] = result;
          break;
        case 4:
          if (rotationHorizont)
            smallBuffer[4799
                - (((line / 2) * 160) + ((1 - (line % 2)) * 80)
                    + (column))] = result;
          else
            smallBuffer[4799
                - (((line / 2) * 160) + ((line % 2) * 80)
                    + (column))] = result;
          break;
          default:break;
        }
      }
      //Rotated
      else
      {
        switch (segment)
        {
        case 1:
          smallBuffer[((line / 2) * 160) + ((line % 2) * 80)
              + (column)] = result;
          break;
        case 2:
          smallBuffer[4800
              + (((line / 2) * 160) + ((line % 2) * 80) + (column))] =
              result;
          break;
        case 3:
          smallBuffer[9600
              + (((line / 2) * 160) + ((line % 2) * 80) + (column))] =
              result;
          break;
        case 4:
          smallBuffer[14400
              + (((line / 2) * 160) + ((line % 2) * 80) + (column))] =
              result;
          break;
          default:break;
        }
      }
    }

  }
  //Everything worked
  return 1;
}
uint16_t Lepton::doGetCommand(uint16_t commandIdBase, uint16_t* data) {
  writeRegister(REG_COMMAND_ID, commandIdBase | TYPE_GET);
  waitIdle();
  return readData(data);
}

void Lepton::doSetCommand(uint16_t commandIdBase, uint16_t* data, uint16_t dataLen) {
  writeData(data, dataLen);
  writeRegister(REG_COMMAND_ID, commandIdBase | TYPE_SET);
  waitIdle();
}

uint16_t Lepton::doRunCommand(uint16_t commandIdBase, uint16_t* data, uint16_t dataLen) {
  writeData(data, dataLen);
  writeRegister(REG_COMMAND_ID, commandIdBase | TYPE_RUN);
  waitIdle();
  return readData(data);
}

/* Get one frame of raw values from the lepton */
void Lepton::getRawValues()
{
  byte line, error, segmentNumbers = 4;
  
  //Begin SPI Transmission
  //lepton_begin();
  syncFrame();
  
  //Go through the segments
  for (byte segment = 1; segment <= segmentNumbers; segment++)
  {
    //Sync
    uint16_t delay_time = 0;
    while(digitalRead(13) == HIGH)
    {
      delay_time++;
      if(delay_time>=10)
      return;    
    }
 
    //Reset error counter for each segment
    error = 0;
    //Go through one segment, equals 60 lines of 80 values
    do
    {
      //int retVal = 0;
      for (line = 0; line < 60; line++)
      {

        //Maximum error count
        if (error == 255)
        {
          //Reset segment
          segment = 1;
          //Reset error
          error = 0;
          //Reset Lepton SPI
          reset();
          //Restart at line 0
          break;
        }
        //Get a package from the lepton
        int retVal = getPackage(line, segment);
        //If everythin worked, continue
        if (retVal == 0){
          if (savePackage(line, segment))
          //if (savePackage(line,  segm))
            continue;
        }
    
        //Raise lepton error
        error++;

        //Stabilize framerate
        uint32_t time = micros();
        while ((micros() - time) < 800)
          __asm__ volatile ("nop");
          
        //Restart at line 0
        break;
      }
    } while (line != 60);
   
  }


  doGetCommand(CMD_SYS_AUX_TEMPERATURE_KELVIN, &aux_temp);
  doGetCommand(CMD_SYS_FPA_TEMPERATURE_KELVIN, &fpa_temp);
  //End SPI Transmission
  end();
}

void Lepton::reset()
{
  //End SPI Transmission
  end();
  delay(186);
  syncFrame();
}
uint16_t Lepton::syncFrame() {
  
  static int count;
  #if 1
    SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE3));
  //SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE3));
    //if(count < 5)
      //SPI.beginTransaction(SPISettings(10200000, MSBFIRST, SPI_MODE3));
      //SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE3));
   //SPI.beginTransaction(SPISettings(10200000, MSBFIRST, SPI_MODE3));
   // else
     // SPI.beginTransaction(SPISettings(10130000, MSBFIRST, SPI_MODE3));
  //SPI.beginTransaction(SPISettings(10110000, MSBFIRST, SPI_MODE3));
    count++;
    if(count > 5)
     count = 5;
   #else
     static int l,b;
     l = 4000000 + b/5 * 100000;
     Serial.printf("b =  %d\n", l);
     SPI.beginTransaction(SPISettings(l, MSBFIRST, SPI_MODE3));
     b++;
   
  //SPI.beginTransaction(SPISettings(36000, MSBFIRST, SPI_MODE0));
   #endif
  // SPI.beginTransaction(SPISettings(11000000, MSBFIRST, SPI_MODE3));
  //SPI.beginTransaction(SPISettings(36000, MSBFIRST, SPI_MODE0));
   //SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE3));
  //SPI.beginTransaction(SPISettings(30000000, MSBFIRST, SPI_MODE3));
  //SPISettings(SPISettings(20000000, MSBFIRST, SPI_MODE3)); //なくても動くが気休め
  //SPI.setClockDivider(SPI_CLOCK_DIV2);
   digitalWrite(_ssPin, LOW);//delay(0.02);
  //delay(186);

/*
     static int l,b;
     l = 360000 + b/5 * 100000;
     Serial.printf("b =  %d\n", l);
     //SPI.beginTransaction(SPISettings(l, MSBFIRST, SPI_MODE3));
     b++;
  SPISettings(SPISettings(l, MSBFIRST, SPI_MODE3)); //なくても動くが気休め
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  digitalWrite(_ssPin, LOW);//delay(0.02);
  */
}

void Lepton::end() {
  digitalWrite(_ssPin, HIGH);
  SPI.endTransaction();
}

int Lepton::readFrame(uint16_t* data) {
  for (byte segment = 1; segment <= 4; segment++){
  uint16_t row = 0;
  uint16_t id = waitNextFrame();
  while ((id & 0xfff) == row) {
    uint16_t crc = readFrameWord();
    for (int col = 0; col < 80; col++) {
      data[(segment - 1) * 4800 + row * 80 + col] = readFrameWord();
    }

    if ((row == 20)){
    //byte seg = (id >> 12);
    //if (seg == 0)
     // return 1;
    ///if (segment != seg)
     // return 2;
    }
    //Serial.printf("row = %d, segment = %d,  id = %d,id_row = %d\n", row, segment, id >> 12, id & 0xfff);
    row++;  
    if (row < 60) {
      id = readFrameWord();
    } else {
      //return 1;
      break;
    }
    //Serial.printf("................readFrame ended with row %4x != id %4x\n", row, id);
  }

  
   //for(;row<60;row++)
  //for (int col = 0; col < 80; col++) {
    //  data[(segment - 1) * 4800 + row * 80 + col] = 0x1f40;
    //}
  //Serial.printf("readFrame ended with row %4x != id %4x\n", row, id);
  }
  return 0;
}

/*
uint16_t Lepton::wait_160x120_Seg(void)
{
  //uint16_t Lepton::wait_160X120_NextFrame() {
  uint16_t id = readFrameWord();
  Serial.printf("id =  %d\n", id);
  while ((id & 0x0f00) == 0x0f00) {
    for (int i = 0; i < 161; i++) {
      readFrameWord();
    }
    id = readFrameWord();
    //Serial.printf("id =  %d\n", id);
  }
  return id;    
}
}
*/
int Lepton::read_160x120_Frame(uint16_t* data) {
  uint16_t row = 0;
  uint16_t id = waitNextFrame();
  while ((id & 0xfff) == row) {
    uint16_t crc = readFrameWord();
    for (int col = 0; col < 80; col++) {
      data[row * 80 + col] = readFrameWord();
    }

   // Serial.printf(" %d %d %d\n",  (id & 0x7000)>>12 ,(id & 0xfff) , row);
    row++;
    if (row < 60) {
      id = readFrameWord();
    } else {
      return 1;
    }
    //Serial.printf("................readFrame ended with row %4x != id %4x\n", row, id);
  }
  Serial.printf("readFrame ended with row %4x != id %4x\n", row, id);
  return 0;
}



void Lepton::readFrameRaw(uint16_t* data) {
  data[0] = waitNextFrame();
  for (int i = 1; i < 82 * 60; i++) {
    data[i] = readFrameWord();
  }
}

void Lepton::startTransmission(uint16_t reg) {
  Wire.beginTransmission(DEVICE_ID);
  transmitWord(reg);
}

void Lepton::transmitWord(uint16_t value) {
  Wire.write(value >> 8 & 0xff);
  Wire.write(value & 0xff);    
}

void Lepton::endTransmission() {
  uint8_t error = Wire.endTransmission();
  if (error != 0) {
    Serial.print("error=");
    Serial.println(error);
  }    
}

uint16_t Lepton::readWord() {
  uint16_t value = Wire.read() << 8;
  value |= Wire.read();
  return value;
}

void Lepton::setRegister(uint16_t reg) {
  startTransmission(reg);
  endTransmission();
}

void Lepton::waitIdle() {
  while (readRegister(REG_STATUS) & STATUS_BIT_BUSY) {
  }
}

uint16_t Lepton::readData(uint16_t* data) {
  uint16_t dataLen = readRegister(REG_DATA_LEN) / 2; // The data sheet says the data length register is in 16-bit words, but it actually seems to be in bytes
  setRegister(REG_DATA_BASE);
  Wire.requestFrom(DEVICE_ID, (uint8_t)(dataLen * 2));
  for (int i = 0; i < dataLen; i++) {
    data[i] = readWord();
  }
  // TODO Check CRC
  return dataLen;
}

void Lepton::writeData(uint16_t* data, uint16_t dataLen) {
  startTransmission(REG_DATA_LEN);
  transmitWord(dataLen);
  for (int i = 0; i < dataLen; i++) {
    transmitWord(data[i]);
  }
  endTransmission();
}

uint16_t Lepton::readFrameWord() {
  uint16_t data = SPI.transfer(0x00) << 8;
  data |= SPI.transfer(0x00);
  return data;
}

uint16_t Lepton::waitNextFrame() {
  uint16_t id = readFrameWord();
  //Serial.printf("id =  %d\n", id);
  while ((id & 0x0f00) == 0x0f00) {
    for (int i = 0; i < 81; i++) {
      readFrameWord();
    }
    id = readFrameWord();
    //Serial.printf(" %d %d %d\n",  id,(id & 0xfff) , (id & 0x0f00));
     //Serial.printf("ID =  %X \n",  id);
  }
  //Serial.printf("idwhile =  %d\n", id);
  return id;    
}
uint16_t Lepton::wait_160X120_NextFrame() {
  /*
  uint16_t seg = readFrameWord();
  Serial.printf("seg =  %d\n", seg);
  //while ((id & 0x0e00) == 0x0e00) {
    
    for (int i = 0; i < 81; i++) {
    id =  readFrameWord();
    }
    id = readFrameWord();
    //Serial.printf("id =  %d\n", id);
  */
  uint16_t seg = 0;
  for(int i = 0; i < 82 * 60; i++)
  {
       seg = readFrameWord();
       Serial.printf("seg =  %d ", (seg & 0x7000)>>12);Serial.printf("id =  %d\n", seg & 0x0fff);
  }
  //return id;    
}


void Lepton::dumpHex(uint16_t *data, int dataLen) {
  for (int i = 0; i < dataLen; i++) {
    Serial.printf("%4x ", data[i]);
  }
  Serial.println();
}
