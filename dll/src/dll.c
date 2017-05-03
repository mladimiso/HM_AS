#include "dll/includes/dll.h"
#include "hal/includes/hal_gpio.h"
#include "hal/includes/hal_uart.h"
#include "common/includes/circular_buffer.h"

#include "cmsis_os.h"

//*****************************************************************************
//
// A mapping of devices IDs to signal_id number.
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

static const uint8_t portPC = PC;
static const uint8_t portCC2530 = CC2530;


//*****************************************************************************
//
// Internal/local functions
// \just defining functionality/
//
//*****************************************************************************
static uint8_t getSignalID(const uint16_t devicesAddress);
static uint8_t checksum(DLLPacket_t *frame);
static uint8_t processFrameRx(uint8_t *frame,
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

#ifdef DEBUG_MODE
	CircularBuffer_t debugCBuffer;
#endif

//*****************************************************************************
//
// Function definition
//
//*****************************************************************************

//*****************************************************************************
//
//! Data link layer initialization function
//!
//! This function initializes states and indexes,
//! configures communication ports, one of \b PC or \b CC2530 and their
//! baudrates, one of \b BAUDRATE_PC \b BAUDRATE_CC2530. Creates two threads
//! for communication wia UART interface.
//!
//! \return None.
//
//*****************************************************************************
void dllInit(void)
{
	osThreadDef(communicationThread,
							osPriorityHigh,
							2,
							0
							);
	
	
	
	statePC = IDLE;
	stateCC2530 = IDLE;
	rxBufferPCIndex = 0;
	txBufferPCIndex = 0;
	rxBufferCC2530Index = 0;
	txBufferCC2530Index = 0;
	
	circularInit(&cTxBufferPC);
	circularInit(&cTxBufferCC2530);
#ifdef DEBUG_MODE
		circularInit(&debugCBuffer);
#endif
	halUARTOpenPort(PC, BAUDRATE_PC , 1, 0);
	halUARTOpenPort(CC2530, 9600, 1, 0);
	
	tID_PC_Communication = osThreadCreate( osThread(communicationThread),
																					(void *)(&portPC)
																					);
	tID_CC2530_Communication = osThreadCreate( osThread(communicationThread),
																					(void *)(&portCC2530)
																					);
}

//*****************************************************************************
//
//! Register callback function.
//!
//! \param cb is pointer on the function.
//!
//! \return None.
//
//*****************************************************************************
void CallBackRegister(CallBack_t cb)
{
	callBack = cb;
}

//*****************************************************************************
//
//! Function handles comminucation. Handles RX or TX on active ports.
//!
//! \param arg is pointer on port.
//!
//! This function sends data from app layer forward and delivers data to app 
//! layer.
//!
//! \return None.
//
//*****************************************************************************
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
		osDelay(10);
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
					*emFlag = 0;
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

						#ifdef DEBUG_MODE
								circularPut(&debugCBuffer, *tmp);
								halUARTWrite(PC, &debugCBuffer, 0);
						#endif
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
				processFrameRx(rxBuffer, *rxIndex, port);
			break;				
			case EMISSION:
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

//*****************************************************************************
//
//! Get the signal ID.
//!
//! \param deviceAddress is device address.
//!
//! For given a device address, this function returns the corresponding 
//! signal ID.
//!
//! \return Returns signal ID, or -1 if \e deviceAddress is not supported.
//
//*****************************************************************************
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

//*****************************************************************************
//
//! Process received data.
//!
//! \param frame is pointer on the received frame.
//! \param len is lenght of received frame.
//! \param port is number of port.
//!
//! Function process received frame, converts, checks checksum and validation
//! of signal ID (supported devices)
//!
//! \return Returns signal ID, or -1 if \e deviceAddress is not supported.
//
//*****************************************************************************
static uint8_t processFrameRx(uint8_t *frame,
															uint8_t len,
															uint8_t port
															)
{
	Data_t *cbData;
	int iSignalID;
	int iData;
	int iPackNum;
	int iTimeStamp;
	int iCheckSum;
	sscanf((char *)frame,
					"%*[^0123456789]%d%*[^0123456789]%d%*[^0123456789]%d%*[^0123456789]%d%*[^0123456789]%d",
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
		
//	if (checksum(&recPack) == recPack.checkSum)
//	{
		
//		if (devAddressSupport(recPack.appData.devID))
//		{
			callBack(cbData, port);
//		}
		return 1;
//	}
//	else
//	{
//	error_checkSum();
	//recPack = NULL;
	//	return 0;
//	}

}


//*****************************************************************************
//
//! Calculating checksum.
//!
//! \param frame is pointer on frame on which one is needed to calculated.
//!
//! Function caluculates checksum executing XOR operation on evert member of
//! \b frame.
//!
//! \return Returns result of sumary operation.
//
//*****************************************************************************
static uint8_t checksum(DLLPacket_t *frame)
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

//*****************************************************************************
//
//! Checks a devices address.
//!
//! \param devAddress is device address.
//!
//! This function determines if a device address is supported.
//!
//! \return Returns \b true if address is supported, or \b false otherwise.
//
//*****************************************************************************
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

//*****************************************************************************
//
//! Function that handles data services.
//!
//! \param aData is pointer on data structure.
//! \param port indicates port number.
//!
//! This function prepare data to be ready to send and sets state flag to 
//! signals the thread to perform an action.
//!
//! \return None.
//
//*****************************************************************************
void dllDataRequest(Data_t *aData,
										uint8_t port)
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
															">%d %d %d %d %d\n",
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
																	">%d %d %d %d %d\n",
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
