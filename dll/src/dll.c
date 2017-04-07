#include "dll\includes\dll.h"
#include "hal\includes\hal_gpio.h"
#include "hal\includes\hal_uart.h"


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

//*****************************************************************************
//
// Internal/local functions
// \just defining functionality/
//
//*****************************************************************************
static uint8_t getSignalID(const uint16_t devicesAddress);

static uint8_t proccessFrame(uint8_t *frame, uint8_t len);
static void checksum(void);
static void checkDevAddr(void);



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

uint8_t rxBufferPC[MAX_BUFFER_LENGHT];
uint8_t txBufferPC[MAX_BUFFER_LENGHT];
uint8_t rxBufferCC2530[MAX_BUFFER_LENGHT];
uint8_t txBufferCC2530[MAX_BUFFER_LENGHT];

uint8_t rxBufferPCIndex;
uint8_t txBufferPCIndex;
uint8_t rxBufferCC2530Index;
uint8_t txBufferCC2530Index;

//*****************************************************************************
//
// Function definition
//
//*****************************************************************************
void dllInit(void)
{
	statePC = IDLE;
	stateCC2530 = IDLE;
	
	rxBufferPCIndex = 0;
	txBufferPCIndex = 0;
	rxBufferCC2530Index = 0;
	txBufferCC2530Index = 0;




}


void communicationTread(void const *arg)
{
	uint8_t port  = *(uint8_t *)(arg);
	char tmp;
	
	uint8_t *rxBuffer;
	uint8_t *txBuffer;
	uint8_t rxIndex;
	uint8_t txIndex;
	eDLLState_t state;
	CircularBuffer_t *cRxBuffer;
	CircularBuffer_t *cTxBuffer;
	
	if (PC == port)
	{
		state 			= statePC;
		cRxBuffer 	= &rxCBufferPC;
		rxBuffer 		= rxBufferPC;
		rxIndex 		= rxBufferPCIndex;
		cTxBuffer		= &txBufferPC;
		txBuffer		= txBufferPC;
	}
	else if (CC2530 == port)
	{
		state 			= stateCC2530;
		cRxBuffer 	= &rxCBufferCC2530;
		rxBuffer 		= rxBufferCC2530;
		rxIndex 		= rxBufferCC2530Index;
		cTxBuffer		= &txBufferCC2530;
		txBuffer		= txBufferCC2530;
	}
	
	while (1)
	{
		switch (state)
		{
			case IDLE:
				if (circularGet(cRxBuffer, &tmp))
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
				if(EMISSION_DEMAND)
				{
					state = EMISSION_START;
				}
				else
				{
					state = IDLE;
				}
				break;
			case RECEPTION:
				do
				{
					if (circularGet(cRxBuffer, &tmp))
					{
						if (START_DELIMITER == tmp)
						{
							rxIndex = 0;
						}
						else 
						{
							rxBuffer[rxIndex++] = tmp;
						}
					}
				} while(STOP_DELIMITER == tmp);
				state = IDLE;
				//
				// process frame on the proper way!
				//
				p
				break;
			case EMISSION_START:
				// kako konvertovati u karaktere kojim punimo cirkularni bufer
				do
				{
					if(START_DELIMITER == tmp)
					{
						txIndex = 0;
					}
					else
					{
						tmp = txBuffer[txIndex];
						circularPut(cTxBuffer, &tmp);
						txIndex++;
					}
				}
				while(STOP_DELIMITER != tmp)
				if(STOP_DELIMITER == tmp)
				{
					state = EMISSION;
				}
				break;
			case EMISSION:
				
				break;
				
			default:
				break;
		}
	}
}

static uint8_t getSignalID(const uint16_t deviceAddress);
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