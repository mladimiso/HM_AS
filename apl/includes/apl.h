#ifndef __APL_H__
#define __APL_H__

#include <stdint.h>

//*****************************************************************************
//
// Global variables
//
//*****************************************************************************
typedef struct sAPLData_t
{
	uint16_t devID = 0;
  //uint16_t devAddress;
	//uint8_t exist = 0;
	uint32_t data = 0;
//	uint32_t cmd = 0;
} APLData_t;



//uint32_t aplData;
//uint8_t devID;
APLData_t appData[20];
//*****************************************************************************
//
// Defining devices address
//
//*****************************************************************************

void aplInit(void);
void aplSendData(uint32_t data, uint16_t devID, uin8_t port);
void aplProcessCommand(void);
void updateData(Data_t *pData);
uint32_t getData(uint16_t devID);












#end //__APL_H__
