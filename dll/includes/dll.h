#ifndef __DLL_H__
#define __DLL_H__

#include <stdint.h>


//*****************************************************************************
//
// Defining devices address
//
//*****************************************************************************
#define TEMP_SENSOR_INSIDE														0x0000
#define TEMP_SENSOR_OUTSIDE														0x0000
#define PRESSURE_SENSOR_INSIDE												0x0000
#define PRESSURE_SENSOR_OUTSIDE												0x0000
#define HUMIDITY_SENSOR_INSIDE												0x0000
#define HUMIDITY_SENSOR_OUTSIDE												0x0000
#define DOOR																					0x0000
#define WINDOW																				0x0000
#define MOTION_SENSOR																	0x0000


#pragma pack(push)
#pragma pack(1)

typedef struct sDLLPacket_t
{
	uint16_t devID;
	uint8_t packNum;
	uint32_t data;
	uint8_t timeStamp;
	uint8_t checkSum;
} DLLPacket_t;

#pragma pack(pop)

void dllInit(void);
void dllDataRequest(DLLPacket_t *appData);

#end //__DLL_H__
