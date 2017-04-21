#ifndef __APL_H__
#define __APL_H__

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "dll/includes/dll.h"
//*****************************************************************************
//
// Global variables
//
//*****************************************************************************
#pragma pack(push)
#pragma pack(1)
typedef struct sAPLData_t
{
	uint16_t devID;
	uint8_t packNum;
	uint32_t data;
} APLData_t;
#pragma pack(pop)


//uint32_t aplData;
//uint8_t devID;
//extern APLData_t DataID[20];
//APLData_t DataID[20];
//*****************************************************************************
//
// Defining devices address
//
//*****************************************************************************

void aplInit(void);
void aplSendData(uint32_t data, uint16_t devID, uint8_t port);
void aplProcessCommand(void);
void updateData(Data_t *pData, uint8_t port);

uint32_t getData(uint16_t devID);




#endif //__APL_H__
