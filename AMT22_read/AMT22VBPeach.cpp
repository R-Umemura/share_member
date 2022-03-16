#include "AMT22VBPeach.h"

AMT22V::AMT22V(SPIClass *xSPI, byte xCSBpin)
{
  CSBpin = xCSBpin;
  pSPI = xSPI;

  pinMode(CSBpin, OUTPUT);
  digitalWrite(CSBpin, HIGH);
  init_done = false;
  // pSPI->begin();
}

// SPI送信部分
int AMT22V::spi_write(int msg)
{
  int msg_temp = 0;
  // digitalWrite(CSBpin, LOW);
  delayMicroseconds(3);
  msg_temp = SPI.transfer(msg); // chack
  digitalWrite(CSBpin, HIGH);
  delayMicroseconds(3);
  return (msg_temp);
}

int AMT22V::init()
{
  // pinMode(CSBpin, OUTPUT);
  // digitalWrite(CSBpin, HIGH);

  delay(100);

  // spi_write(0x10); // chack

  int ret = getRawEncount();
  preABSposition = ABSposition;
  init_done = true;
  /*
  if (ret == -1)
    return -1;
  else
    init_done = true;
  return 1;
  */
}

int AMT22V::getRawEncount()
{
  int recieved;
  int recieve_count = 0;
  int error_count = 0;
  bool recieve_done = false;

  unsigned int rawValue = 0;
  bool correct_answer = false;

  digitalWrite(CSBpin, LOW);
  delayMicroseconds(3);
  rawValue |= SPI.transfer(0x00) << 8; // chack
  delayMicroseconds(3);
  rawValue |= SPI.transfer(0x00); // chack
  delayMicroseconds(3);
  digitalWrite(CSBpin, HIGH);
  delayMicroseconds(3);

  bool odd, even;
  for (int i = 0; i < 14; i++)
  {
    if (i % 2)
    {
      odd ^= (rawValue >> i) & 0x01; // chack
    }
    else
    {
      even ^= (rawValue >> i) & 0x01; // chack
    }
  }
  odd = !odd;
  even = !even;

  correct_answer = (((rawValue >> 15) & 0x01) == odd) && (((rawValue >> 14) & 0x01) == even);
  ABSposition = rawValue & 0x3FFF;
  /*
    if (correct_answer)
    {
      ABSposition = rawValue & 0x3FFF;
      // printf("ret : %d %x\n", ABSposition, rawValue);
    }
    else
    {
      // printf("xxx : %d %x\n", rawValue, rawValue);
      return -1;
    }
    */
  return 1;
}

int AMT22V::getEncount()
{
  // if (init_done)
  //{
  getRawEncount();
  updateCount();

  encount = rotation * res + ABSposition;
  preABSposition = ABSposition;

  // printf("  encount %d abs position %d       ", encount, ABSposition);
  /*}
  else
  {
    return -1;
  }*/
  return encount;
}

// 2,1,0の次に4095ではなくマイナスの値になるように，4093,4094,4095の次に0にならず大きな値になるようにしてる関数
void AMT22V::updateCount()
{
  if (abs(preABSposition - ABSposition) >= 3000)
  {
    if (preABSposition > ABSposition)
    {
      rotation++;
    }
    else if (preABSposition < ABSposition)
    {
      rotation--;
    }
  }
}

int AMT22V::setZeroPos()
{
  int response;
  int count = 0;
  digitalWrite(CSBpin, LOW);
  delayMicroseconds(3);
  // SPI.transfer(0x70);//chack
  response |= SPI.transfer(0x00);
  delayMicroseconds(3);
  response |= SPI.transfer(0x70);
  delayMicroseconds(3);
  digitalWrite(CSBpin, HIGH);
  //printf("response %x\n", response);

  getRawEncount();
  preABSposition = ABSposition;

  return 1;
}