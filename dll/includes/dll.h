#ifndef __DLL_H__
#define __DLL_H__

#include <stdint.h>


//*****************************************************************************
//
// Defining devices address
//
//*****************************************************************************
#define TEMP_SENSOR_INSIDE														0x1111
#define TEMP_SENSOR_OUTSIDE														0x1112
#define PRESSURE_SENSOR_INSIDE												0x1113
#define PRESSURE_SENSOR_OUTSIDE												0x1114
#define HUMIDITY_SENSOR_INSIDE												0x1115
#define HUMIDITY_SENSOR_OUTSIDE												0x1116
#define DOOR																					0x1117
#define WINDOW																				0x1118
#define MOTION_SENSOR																	0x1119


#pragma pack(push)
#pragma pack(1)

typedef struct sData
{
	uint16_t devID;
	uint8_t packNum;
	uint32_t data;
} Data_t;

#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)

typedef struct sDLLPacket_t
{
	Data_t appData;
//	uint16_t devID;
	
//	uint32_t data;
	uint8_t timeStamp;
	uint8_t checkSum;
} DLLPacket_t;

#pragma pack(pop)


typedef void (*CallBack_t)(Data_t *, uint8_t);

void dllInit(void);
void CallBackRegister(CallBack_t cb);
void dllDataRequest(Data_t *aData, uint8_t port);

#endif //__DLL_H__
