#include "dll/includes/dll.h"
#include "hal/includes/hal_gpio.h"
#include "hal/includes/hal_uart.h"
#include "common/includes/circular_buffer.h"

#include "cmsis_os.h"

//*****************************************************************************
//
// Values that determines start and stop of each frame as well as start and
// stop of each field of received data
//
//*****************************************************************************



#define START_DELIMITER													0x3e // '>'
#define STOP_DELIMITER													0x3e			//0x0a // '\n'
#define START_FIELD															0x23 // '#'
#define STOP_FIELD															0x2c // ','
#define EMISSION_START												1

#define MAX_BUFFER_LENGHT												256
#define CHECK_SUM_OFFSET												2
//*****************************************************************************
//
// A mapping of devices address to signal_id number.
//
//*****************************************************************************
static const uint16_t signalID[] = {
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
	EMISSION
} DLLState_t;

//*****************************************************************************
//
// Internal/local functions
// \just defining functionality/
//
//*****************************************************************************
static uint8_t getSignalID(const uint16_t devicesAddress);
static uint8_t checksum(DLLPacket_t *frame);
static uint8_t proccessFrameRx(uint8_t *frame,
															 uint8_t len,
															 uint8_t port
															 );
static uint8_t devAddressSupport(uint16_t devAddress);




//*****************************************************************************
//
// CMSIS-RTOS tread definition and tread  function declaration
//
//*****************************************************************************
void communicationThread(void const *arg);
osThreadId tID_PC_Communication;
osThreadId tID_CC2530_Communication;


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
DLLState_t statePC;
DLLState_t stateCC2530;
uint8_t emFlagPC;
uint8_t emFlagCC2530;

char uiTxBufferPC[MAX_BUFFER_LENGHT];
uint8_t uiRxBufferPC[MAX_BUFFER_LENGHT];

char uiTxBufferCC2530[MAX_BUFFER_LENGHT];
uint8_t uiRxBufferCC2530[MAX_BUFFER_LENGHT];

uint8_t rxBufferPCIndex;
uint8_t txBufferPCIndex;

uint8_t rxBufferCC2530Index;
uint8_t txBufferCC2530Index;

uint8_t tmpPC;
uint8_t tmpCC2530;

CircularBuffer_t cTxBufferPC;
CircularBuffer_t cTxBufferCC2530;

DLLPacket_t recPack;

static CallBack_t callBack;

//*****************************************************************************
//
// Function definition
//
//*****************************************************************************
void dllInit(void)
{
	osThreadDef(communicationThread,
							osPriorityHigh,
							2,
							0
							);
	
	uint8_t portPC = PC;
	uint8_t portCC2530 = CC2530;
	
	statePC = IDLE;
	stateCC2530 = IDLE;
	rxBufferPCIndex = 0;
	txBufferPCIndex = 0;
	rxBufferCC2530Index = 0;
	txBufferCC2530Index = 0;
	
	circularInit(&cTxBufferPC);
	circularInit(&cTxBufferCC2530);
	
	halUARTOpenPort(PC, BAUDRATE_PC , 1, 0);
	halUARTOpenPort(CC2530, BAUDRATE_CC2530, 1, 0);
	
	tID_PC_Communication = osThreadCreate( osThread(communicationThread),
																					(void *)(&portPC)
																					);
	tID_CC2530_Communication = osThreadCreate( osThread(communicationThread),
																					(void *)(&portCC2530)
																					);
}

void CallBackRegister(CallBack_t cb)
{
	callBack = cb;
}

void communicationThread(void const *arg)
{
	uint8_t port  = *(uint8_t *)(arg);
	uint8_t *tmp;
	uint8_t *rxBuffer;
  uint8_t *txBuffer;
	uint8_t *txIndex;
	uint8_t *rxIndex;
	uint8_t *emFlag;
	DLLState_t *state;
	CircularBuffer_t *cRxBuffer;
	CircularBuffer_t *cTxBuffer;
	
	if (PC == port)
	{
		tmp = &tmpPC;
		state = &statePC;
		txBuffer = (uint8_t *)uiTxBufferPC;
		rxBuffer = uiRxBufferPC;
		txIndex = &txBufferPCIndex;
		rxIndex = &rxBufferPCIndex;
		emFlag = &emFlagPC;
		cRxBuffer = &rxBufferPC;
		cTxBuffer = &cTxBufferPC;
	}
	else if (CC2530 == port)
	{
  	tmp = &tmpCC2530; 
		state = &stateCC2530;
		txBuffer = (uint8_t *)uiTxBufferCC2530;
		rxBuffer = uiRxBufferCC2530;
		txIndex = &txBufferCC2530Index;
		rxIndex = &rxBufferCC2530Index;
		emFlag = &emFlagCC2530;
		cRxBuffer = &rxBufferCC2530;
		cTxBuffer = &cTxBufferCC2530;
	}

	while (1)
	{
		osDelay(2);
		switch ((*state))
		{
			case IDLE:
				if (circularGet(cRxBuffer, tmp))	//zastita!!!
				{
					if (START_DELIMITER == (*tmp))
					{
						*rxIndex = 0;
						*state = RECEPTION;
					}
					else
					{
						*state = IDLE;
						
					}
				}
				else if (EMISSION_START == (*emFlag))
				{
					*state = EMISSION;
				}
				
				else
				{
					*state = IDLE;
					
				}
				break;
			case RECEPTION:
				do
				{
					if (circularGet(cRxBuffer, tmp)) // zastita!!!
					{
						if (START_DELIMITER == (*tmp))
						{
							*rxIndex = 0;
						}
						else
						{
							rxBuffer[(*rxIndex)++] = *tmp;
						}
					}
				} while(STOP_DELIMITER != *tmp);
				*state = IDLE;				//
				proccessFrameRx(rxBuffer, *rxIndex, port);
		

				break;
			 
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
			
			case EMISSION:
				circularEmptyBuffer(cTxBuffer);
				array2circular(cTxBuffer, txBuffer, *txIndex);
				halUARTWrite(port, cTxBuffer, 0);
				*txIndex = 0;
				*state = IDLE;
			break;
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

uint8_t proccessFrameRx(uint8_t *frame,
												uint8_t len,
												uint8_t port
												)
{
	Data_t *cbData;
	//DLLPacket_t *tmpRrcPack = recPack;
	int iSignalID;
	int iData;
	int iPackNum;
	int iTimeStamp;
	int iCheckSum;
	sscanf((char *)frame,
					"%*[^0123456789]%d\
					 %*[^0123456789]%d\
					 %*[^0123456789]%d\
					 %*[^0123456789]%d\
					 %*[^0123456789]%d",
					 &iSignalID,
					 &iPackNum,
					 &iData,
					 &iTimeStamp,
					 &iCheckSum
					 );
		recPack.appData.devID = signalID[iSignalID];
		recPack.appData.packNum = (uint32_t)iPackNum & 0xff;
		recPack.appData.data = (uint32_t)iData & 0xffffffff;
		recPack.timeStamp = (uint32_t)iTimeStamp & 0xff;
		recPack.checkSum = (uint32_t)iCheckSum & 0xff;
		
		cbData = (struct sData *)(&recPack);
		

//		if (checksum(recPack) == recPack->checkSum)
//		{
//			if (devAddressSupport(recPack->appData.devID))
//			{

								callBack(cbData, port);

//			}
			return 0;
//		}
//	//	else
//		{
//			//error_checkSum();
//			recPack = NULL;
//			return 1;
//		}

}

uint8_t checksum(DLLPacket_t *frame)
{
	uint8_t sum  = 0x00;
	uint8_t *ptr = (uint8_t *)(frame);
	uint8_t *end = &frame->checkSum;
	while(ptr != end)
	{
		sum ^= *ptr;
		ptr++;
	}

	return sum;

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


void dllDataRequest(Data_t *aData, uint8_t port)
{
	DLLPacket_t emPacket;
	emPacket.appData = *aData;
	
	// ako bude bilo potrebe za slanjem vremenskog trenutka
	// radi iscrtavanja grafa obezbjedicemo mehanizam, inace 
	// timeStamp = 0
	emPacket.timeStamp = 0;
	emPacket.checkSum = checksum(&emPacket);
	
	if (PC == port)
	{
		txBufferPCIndex = sprintf(uiTxBufferPC,
															">#%d,#%d,#%d,#%d,#%d",
															getSignalID(emPacket.appData.devID),
															emPacket.appData.packNum,
															emPacket.appData.data,
															emPacket.timeStamp,
															emPacket.checkSum
															);
		emFlagPC = EMISSION_START;
	}
	else if (CC2530 == port)
	{
		txBufferCC2530Index = sprintf(uiTxBufferCC2530,
																	">#%d,#%d,#%d,#%d,#%d",
																	getSignalID(emPacket.appData.devID),
																	emPacket.appData.packNum,
																	emPacket.appData.data,
																	emPacket.timeStamp,
																	emPacket.checkSum
																	);
	  emFlagCC2530 = EMISSION_START;
	}
	else 
	{
		//error!!!
	}
	
}
