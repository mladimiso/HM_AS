#include "apl/includes/apl.h"
#include "dll/includes/dll.h"
#include "hal/includes/hal_gpio.h"
#include "hal/includes/hal_uart.h"
#include "common/includes/includes.h"
#include "common/includes/defines.h"

//#define osObjectsPublic                     // define objects in main module
//#include "osObjects.h"                      // RTOS object definitions
#include "cmsis_os.h"



#define DEV_CAPACITY												20
#define REGULATION											0x3666
#define REGULATION_OFFSET										15																	

//
//testing with mutex
//
osMutexId mID_sendDataLock;
osMutexId mID_receivedDataLock;
osMutexDef(mID_sendDataLock);
osMutexDef(mID_receivedDataLock);



//*****************************************************************************
//
// Extern variables
//
//*****************************************************************************
extern uint8_t dataFromPC;
extern uint8_t dataFromCC2530;

//*****************************************************************************
//
// Local variables
//
//*****************************************************************************
static APLData_t DataID[DEV_CAPACITY];
//static uint8_t dev_number;

//static APLData_t userDataID[DEV_CAPACITY];


//*****************************************************************************
//
// Function definition
//
//*****************************************************************************

void aplInit(void)
{
	#if defined(STELLARIS) && true == STELLARIS
		halGPIOOutput(HW_PORT_D, 0xff, 0);
		//halGPIOPinWrite(HW_PORT_D, 0x02, LOW);
	
	#else
	#endif
	CallBackRegister(updateData);
	
	mID_sendDataLock = osMutexCreate( osMutex(mID_sendDataLock));
	mID_receivedDataLock = osMutexCreate( osMutex(mID_receivedDataLock));
}

void aplSendData(uint32_t data, uint16_t devID, uint8_t port)
{
	Data_t app_packet;
  //DLLPacket_t *app_packet;
	osMutexWait(mID_sendDataLock, osWaitForever);
   app_packet.data = data;
   app_packet.devID = devID;
	 app_packet.packNum = 0;
   dllDataRequest(&app_packet, port);
	osMutexRelease(mID_sendDataLock);
}

/*
	for (int i = 0; i < dev_number; i++)
	{
		
	}
*/


void updateData(Data_t *pData, uint8_t port)
{

  int i = 0;
  while(DataID[i].devID != 0 && DataID[i].devID != pData->devID && i<20)
  {
    i++;
  }
	osMutexWait(mID_receivedDataLock, osWaitForever);
  if(DataID[i].devID == 0)//!= pData->devID)
	//if(DataID[i].devID == pData->devID)
  {
		//osMutexWait(mID_receivedDataLock, osWaitForever);
      DataID[i].devID = pData->devID;
			//dev_number++;
		//osMutexRelease(mID_receivedDataLock);
  }
	
	if(i < 20)
	{
		//osMutexWait(mID_receivedDataLock, osWaitForever);
//			switch (DataID[i].devID)
//			{
//				case TEMP_SENSOR_INSIDE:
//					
//					break;
//				case TEMP_SENSOR_OUTSIDE:
//					break;
//				case PRESSURE_SENSOR_INSIDE:
//					break;
//				case PRESSURE_SENSOR_OUTSIDE:
//					break;
//				case HUMIDITY_SENSOR_INSIDE:
//					break;
//			  case HUMIDITY_SENSOR_OUTSIDE:
//					break;
//				case DOOR:
//					break;
//				case WINDOW:
//					break;
//				case MOTION_SENSOR:
//					break;
//				
//				default:
//					break;
			
	//		}
			if (PC == port)
			{
				if (REGULATION == (pData->data & 0x7fff))
				{
					DataID[i].regulation = (pData->data >> REGULATION_OFFSET) & 0x01;
					//dataFromPC = 2;
					DataID[i].status |= PC_REGULATION;
				}
				else
				{
					DataID[i].userData = pData->data;
					//dataFromPC = 1;
					DataID[i].status |= PC_VALUE;
				}
			}
			else if (CC2530 == port)
			{
				DataID[i].data = pData->data;
				//dataFromCC2530 = 1;
				DataID[i].status |= CC_MONITOR;
			}
		//osMutexRelease(mID_receivedDataLock);	
	}
	osMutexRelease(mID_receivedDataLock);
	
}

void resetStatus(uint16_t devID, 
									uint8_t value)
{
	int i = 0;
  while(DataID[i].devID != devID && i < 20)
  {
    i++;
  }
	osMutexWait(mID_receivedDataLock, osWaitForever);
  if(DataID[i].devID == devID)
  {
		DataID[i].status &= ~(value);
	}
	
	osMutexRelease(mID_receivedDataLock);
  
}



uint32_t getData(uint16_t devID, 
								 uint8_t flag)
{
  int i = 0;
  uint32_t data = 555555;
  while(DataID[i].devID != devID && i < 20) //
	//while(DataID[i].devID != 0 && DataID[i].devID != devID && i < 20)
  {
    i++;
  }
	osMutexWait(mID_receivedDataLock, osWaitForever);
  if(DataID[i].devID == devID)
  {
		switch(flag)
		{
			case SENSOR:
				data = DataID[i].data;
				break;
			case USER:
				data = DataID[i].userData;
				break;
			case REGULATE:
				data = DataID[i].regulation;
				break;
			case STATUS:
				data = DataID[i].status;
				//DataID[i].status = 0;
				break;
			default:
				break;
		}
	}
	osMutexRelease(mID_receivedDataLock);
  return data;
}


void regulateTemperature(uint8_t state,
												 int16_t diference)
{
	#if defined(STELLARIS) && true == STELLARIS 
		if (0 == diference)
		{
			//
			//turn of aircond and heater
			//
			if ((HIGH << 0) == halGPIOPinRead(HW_PORT_D, GPIO_PIN_0))
			{
				halGPIOPinWrite(HW_PORT_D, GPIO_PIN_0, LOW);
			}
			if ((HIGH << 1) == halGPIOPinRead(HW_PORT_D, GPIO_PIN_1))
			{
				halGPIOPinWrite(HW_PORT_D, GPIO_PIN_1, LOW);
			}
		}
		if (0 > diference)
		{
			if ((LOW << 0) == halGPIOPinRead(HW_PORT_D, GPIO_PIN_0))
			{
				halGPIOPinWrite(HW_PORT_D, GPIO_PIN_0, HIGH);
			}
			if ((HIGH << 1) == halGPIOPinRead(HW_PORT_D, GPIO_PIN_1))
			{
				halGPIOPinWrite(HW_PORT_D, GPIO_PIN_1, LOW);
			}
		}
		else if (0 < diference)
		{
			if ((HIGH << 0) == halGPIOPinRead(HW_PORT_D, GPIO_PIN_0))
			{
				halGPIOPinWrite(HW_PORT_D, GPIO_PIN_0, LOW);
			}
			if ((LOW << 1) == halGPIOPinRead(HW_PORT_D, GPIO_PIN_1))
			{
				halGPIOPinWrite(HW_PORT_D, GPIO_PIN_1, HIGH);
			}
		}
	#else

	#endif
}


/*void aplProcessCommand(void)
{
  uint32_t data = 0;
  uint16_t devID = 0;
  aplReadData(&data, &devID, PC);
  aplSendData(data, devID, CC2530);

}*/
