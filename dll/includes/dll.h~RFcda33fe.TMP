#ifndef __DLL_H__
#define __DLL_H__

#include <stdint.h>


//*****************************************************************************
//
// Defining supported devices IDs
//
//*****************************************************************************
#define TEMP_SENSOR_INSIDE														0x1112
#define TEMP_SENSOR_OUTSIDE														0x1111
#define PRESSURE_SENSOR_INSIDE												0x1113
#define PRESSURE_SENSOR_OUTSIDE												0x1114
#define HUMIDITY_SENSOR_INSIDE												0x1115
#define HUMIDITY_SENSOR_OUTSIDE												0x1116
#define DOOR																					0x1117
#define WINDOW																				0x1118
#define MOTION_SENSOR																	0x1119

//*****************************************************************************
//
// Values that determines start and stop of each frame as well as start and
// stop of each field of received data
//
//*****************************************************************************
#define START_DELIMITER																0x3e // '>'
#define STOP_DELIMITER																0x3c // '<'			//0x0a // '\n'
#define START_FIELD																		0x23 // '#'
#define STOP_FIELD																		0x2c // ','
#define EMISSION_START																0x01

//
// Maximum DLL layer payload size
//
#define MAX_BUFFER_LENGHT												256

//*****************************************************************************
//
// Typedefs
//
//*****************************************************************************
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
	uint8_t timeStamp;
	uint8_t checkSum;
} DLLPacket_t;
#pragma pack(pop)


typedef enum eDLLState
{
	IDLE,
	RECEPTION,
	EMISSION
} DLLState_t;

typedef void (*CallBack_t)(Data_t *, uint8_t);

//*****************************************************************************
//
// API Function prototypes
//
//*****************************************************************************
void dllInit(void);
void CallBackRegister(CallBack_t cb);
void dllDataRequest(Data_t *aData, 
										uint8_t port);

#endif //__DLL_H__
