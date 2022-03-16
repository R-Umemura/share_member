#ifndef _AMT22V_H_INCLUDED
#define _AMT22V_H_INCLUDED

#include <SPI.h>

class AMT22V
{
public:
  AMT22V(SPIClass *, byte);

  int spi_write(int msg);
  int init();
  int getEncount();
  void updateCount();
  int setZeroPos();
  int getRawEncount();

private:
  byte CSBpin;
  bool init_done;
  bool recieve_done;
  int res = 0x3FFF;//4096
  int ABSposition; // rawdata
  int preABSposition;
  int rotation = 0;
  int encount; // 渡す値
  int temp[2];
  SPIClass *pSPI;
  //DigitalOut *CSBpin;
};

#endif
