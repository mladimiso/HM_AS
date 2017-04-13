#include "dll/includes/dll.h"
#include "hal/includes/hal_gpio.h"
#include "hal/includes/hal_uart.h"
#include "common/includes/circular_buffer.h"

//*****************************************************************************
//
// Values that determines start and stop of each frame as well as start and
// stop of each field of received data
//
//*****************************************************************************



#define START_DELIMITER													0x3e // '>'
#define STOP_DELIMITER													0x0a // '\n'
#define START_FIELD															0x23 // '#'
#define STOP_FIELD															0x2c // ','
#define EMISSION_DEMAND													1

#define MAX_BUFFER_LENGHT												256
#define CHECK_SUM_OFFSET												2
//*****************************************************************************
//
// A mapping of devices address to signal_id number.
//
//*****************************************************************************
static const uint8_t signalID[] = {
									TEMP_SENSOR_INSIDE,
									TEMP_SENSOR_OUTSIDE,
									PRESSURE_SENSOR_INSIDE,
									PRESSURE_SENSOR_OUTSIDE,
									HUMIDITY_SENSOR_INSIDE,
									HUMIDITY_SENSOR_OUTSIDE,
									DOOR,
									WINDOW,
									MOTION_SENSOR
									};

typedef enum eDLLState
{
	IDLE,
	RECEPTION,
	EMISSION_START,
	EMISSION
} eDLLState_t;


uint8_t uiTxBufferPC[MAX_BUFFER_LENGHT];
uint8_t uiTxBufferCC2530[MAX_BUFFER_LENGHT];
//*****************************************************************************
//
// Internal/local functions
// \just defining functionality/
//
//*****************************************************************************
static uint8_t getSignalID(const uint16_t devicesAddress);
static uint8_t checksum(DLLPacket_t *frame,
												uint8_t cs
												);
/*static uint8_t proccessFrame(CircularBuffer_t *frame,
															uint8_t len,
																	//void (*update)(uint32_t data, uint16_t devID));
																	call_back back);*/
static uint8_t proccessFrameRx(CircularBuffer_t *frame,
															 uint8_t len
															 );
static uint8_t devAddressSupport(uint16_t devAddress);
static void checkdevID(void);



//*****************************************************************************
//
// CMSIS-RTOS tread definition and tread  function declaration
//
//*****************************************************************************
void communicationTread(void const *arg);



//*****************************************************************************
//
// Extern variables
//
// treba promijeniti naziv ova dva bafera u circularBuffer.h
//
//*****************************************************************************
CircularBuffer_t rxBufferPC;
CircularBuffer_t rxBufferCC2530;

//*****************************************************************************
//
// Local variables
//
//*****************************************************************************
eDLLState_t statePC;
eDLLState_t stateCC2530;
DLLPacket_t *recPack;
CallBack_t callBack;

//*****************************************************************************
//
// Function definition
//
//*****************************************************************************
void dllInit(void)
{
	statePC = IDLE;
	stateCC2530 = IDLE;





}


void communicationTread(void const *arg)
{
	uint8_t port  = *(uint8_t *)(arg);
	//char tmp;
	uint8_t tmp;
	uint8_t rxIndex;
	eDLLState_t state;
	CircularBuffer_t *cRxBuffer;
	if (PC == port)
	{

		uint8_t uiRxBufferPC[MAX_BUFFER_LENGHT];
		uint8_t uiTxBufferPC[MAX_BUFFER_LENGHT];
		CircularBuffer_t cTxBufferPC;

		uint8_t rxBufferPCIndex = 0;

		uint8_t txBufferPCIndex = 0;
		state = statePC;
		rxIndex = rxBufferPCIndex;
		cRxBuffer = &rxBufferPC;
	}
	else if (CC2530 == port)
	{

		uint8_t uiRxBufferCC2530[MAX_BUFFER_LENGHT];
		uint8_t uiTxBufferCC2530[MAX_BUFFER_LENGHT];
		CircularBuffer_t cTxBufferCC2530;

		uint8_t rxBufferCC2530Index = 0;

		uint8_t txBufferCC2530Index = 0;
		state = stateCC2530;
		rxIndex = rxBufferCC2530Index;
		cRxBuffer = &rxBufferCC2530;
	}

	while (1)
	{
		switch (state)
		{
			case IDLE:
				if (circularGet(cRxBuffer, &tmp))	//zastita!!!
				{
					if (START_DELIMITER == tmp)
					{
						rxIndex = 0;
						state = RECEPTION;
					}
					else
					{
						state = IDLE;
					}
				}
				else
				{
					state = IDLE;
				}
				break;
			case RECEPTION:
				do
				{
					if (circularGet(cRxBuffer, &tmp)) // zastita!!!
					{
						if (START_DELIMITER == tmp)
						{
							rxIndex = 0;
						}
						else
						{
							cRxBuffer->buffer[rxIndex++] = tmp;
						}
					}
				} while(STOP_DELIMITER == tmp);
				state = IDLE;				//
				proccessFrameRx(cRxBuffer, rxIndex);
				// process frame on the proper way!
				//
			/*	if(port == CC2530)
				{
					proccessFrame(cRxBuffer, rxIndex, updateData(recPack->data, recPack->devID));
				}
				else if(port == PC)
				{
					proccessFrame(cRxBuffer, rxIndex, updateCmd(recPack->data, recPack->devID));
				}*/

				break;
			// case EMISSION_START:
				// // kako konvertovati u karaktere kojim punimo cirkularni bufer
				// do
				// {
					// if(START_DELIMITER == tmp)
					// {
						// txIndex = 0;
					// }
					// else
					// {
						// tmp = txBuffer[txIndex];
						// circularPut(cTxBuffer, &tmp);
						// txIndex++;
					// }
				// }
				// while(STOP_DELIMITER != tmp)
				// if(STOP_DELIMITER == tmp)
				// {
					// state = EMISSION;
				// }
				// break;
			// case EMISSION:

				// break;

			default:
				// error state!!!
				break;
		}
	}
}

static uint8_t getSignalID(const uint16_t deviceAddress)
{
	uint8_t i;
	uint8_t retVal = 255;
	for (i = 0; i < sizeof(signalID); i++)
	{
		if (deviceAddress == signalID[i])
		{
			retVal = i;
		}
	}
	return retVal;
}

static uint8_t proccessFrameRx(	CircularBuffer_t *frame,
										uint8_t len
										//void (*update)(uint32_t data, uint16_t devID)

									)
{
	uint8_t tmpCS = atoi((char *)frame->buffer[len - CHECK_SUM_OFFSET]);
	/*if (checksum(frame, len, tmpCS))
	{*/
		int i1;
		int i2;
		int i3;
		int i4;
		int i5;
		sscanf((char *)frame,
					"%*[^0123456789]%d\
					 %*[^0123456789]%d\
					 %*[^0123456789]%d\
					 %*[^0123456789]%d\
					 %*[^0123456789]%d",
					 &i1,
					 &i2,
					 &i3,
					 &i4,
					 &i5);
		recPack->appData.devID = signalID[i1];
		recPack->packNum = i2 & 0xff;
		recPack->appData.data = i3 & 0xffffffff;
		recPack->timeStamp = i4 & 0xff;
		recPack->checkSum = i5 & 0xff;
		//back(recPack->data, recPack->devID);
		//}
		if (checksum(recPack, tmpCS))
		{
			if (devAddressSupport(recPack->appData.devID))
			{
				callBack((struct sData *)(recPack));
				return 0;
			}
		}
		else
		{
			//error_checkSum();
			recPack = NULL;
			return 1;
		}

}

static uint8_t checksum(DLLPacket_t *frame,
												uint8_t cs)
{
	uint8_t sum  = 0x00;
	uint8_t *ptr = (uint8_t *)(frame);
	uint8_t *end = &frame->checkSum;
	while(ptr != end)
	{
		sum ^= *ptr;
		ptr++;
	}

	return (sum == cs);

}

static uint8_t devAddressSupport(uint16_t devAddress)
{
	return( (devAddress == TEMP_SENSOR_INSIDE) ||
					(devAddress == TEMP_SENSOR_OUTSIDE) ||
					(devAddress == PRESSURE_SENSOR_INSIDE) ||
 					(devAddress == PRESSURE_SENSOR_OUTSIDE) ||
					(devAddress == HUMIDITY_SENSOR_INSIDE) ||
					(devAddress == HUMIDITY_SENSOR_OUTSIDE) ||
					(devAddress == DOOR) ||
					(devAddress == WINDOW) ||
					(devAddress == MOTION_SENSOR)
					);
}

void CallBackRegister(CallBack_t cb)
{
	callBack = cb;
}
void dllDataRequest(Data_t *appData, uint8_t port)
{

}
