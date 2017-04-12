#include "apl\includes\apl.h"
#include "dll\includes\dll.h"
#include "hal\includes\hal_gpio.h"
#include "hal\includes\hal_uart.h"
#include "common\includes\includes.h"
#include "common\includes\defines.h"
//*****************************************************************************
//
// Extern variables
//
// treba promijeniti naziv ova dva bafera u circularBuffer.h
//
//*****************************************************************************
DLLPacket_t* app_packet;


//*****************************************************************************
//
// Local variables
//
//*****************************************************************************



//*****************************************************************************
//
// Function definition
//
//*****************************************************************************

void aplInit(void)
{
  halGPIOInit();
  halUARTInit();
  dllInit();

}

void aplSendData(uint32_t data, uint16_t devID, uin8_t port)
{
  //DLLPacket_t *app_packet;
  app_packet->data = data;
  app_packet->devID = devID;
  dllDataRequest(app_packet, port);
}



void updateData(uint32_t data, uint16_t devID)
{
  int i = 0;
  while(appData[i].devID != 0 && appData[i].devID != devID && i<20)
  {
    i++;
  }
  if(appData[i].devID != devID)
  {
      appData[i].devID = devID;
  }
  appData[i].data = data;
}

void updateCmd(uint32_t data, uint16_t devID)
{
  int i = 0;
  while(appData[i].devID != 0 && appData[i].devID != devID && i<20)
  {
    i++;
  }
  if(appData[i].devID != devID)
  {
      appData[i].devID = devID;
  }
  appData[i].cmd = data;
}
uint32_t getData(uint16_t devID)
{
  int i = 0;
  uint32_t data = 4294967295;
  while(appData[i].devID != 0 && appData[i].devID != devID)
  {
    i++;
  }
  if(appData[i].devID == devID)
  {
    data = appData[i].data;
  }
  return data;
}

void aplProcessCommand(void)
{
  uint32_t data = 0;
  uint16_t devID = 0;
  aplReadData(&data, &devID, PC);
  aplSendData(data, devID, CC2530);

}
