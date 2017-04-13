#include "apl/includes/apl.h"
#include "dll/includes/dll.h"
#include "hal/includes/hal_gpio.h"
#include "hal/includes/hal_uart.h"
#include "common/includes/includes.h"
#include "common/includes/defines.h"
//*****************************************************************************
//
// Extern variables
//
// treba promijeniti naziv ova dva bafera u circularBuffer.h
//
//*****************************************************************************



//*****************************************************************************
//
// Local variables
//
//*****************************************************************************
Data_t *app_packet;


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
	CallBackRegister(updateData);
}

void aplSendData(uint32_t data, uint16_t devID, uint8_t port)
{
  //DLLPacket_t *app_packet;
  app_packet->data = data;
  app_packet->devID = devID;
  dllDataRequest(app_packet, port);
}



void updateData(Data_t *pData)
{
  int i = 0;
  while(DataID[i].devID != 0 && DataID[i].devID != pData->devID && i<20)
  {
    i++;
  }
  if(DataID[i].devID != pData->devID)
  {
      DataID[i].devID = pData->devID;
  }
  DataID[i].data = pData->data;
}


uint32_t getData(uint16_t devID)
{
  int i = 0;
  uint32_t data = -1;
  while(DataID[i].devID != 0 && DataID[i].devID != devID)
  {
    i++;
  }
  if(DataID[i].devID == devID)
  {
    data = DataID[i].data;
  }
  return data;
}

/*void aplProcessCommand(void)
{
  uint32_t data = 0;
  uint16_t devID = 0;
  aplReadData(&data, &devID, PC);
  aplSendData(data, devID, CC2530);

}*/
