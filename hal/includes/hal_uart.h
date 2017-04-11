#ifndef __HAL_UART_H__
#define __HAL_UART_H__

#include "common/includes/defines.h"
#include "common/includes/includes.h"	
#include "common/includes/circular_buffer.h"

#include "driverlib/uart.h"
#include "utils/uartstdio.h"



// ISR receive buffer
extern CircularBuffer_t rxBufferPC;
extern CircularBuffer_t rxBufferCC2530;
extern CircularBuffer_t rxBuffer2;



//*****************************************************************************
//
// Function declaration
//
//*****************************************************************************
void halUARTInit(void);
uint8_t halUARTOpenPort(uint8_t pNum, 
												uint32_t dRate,
												uint8_t sBit,
												uint8_t pBit
												);
uint8_t halUARTPoll(uint8_t port);
uint8_t halUARTAvail(uint8_t port);
uint8_t halUARTRead(uint8_t port,
										CircularBuffer_t *rxBuffer);
uint8_t halUARTWrite(uint8_t port,
											CircularBuffer_t *txBuffer, 
											uint8_t len);


//*****************************************************************************
//
// Function declaration TMP
//
//*****************************************************************************
extern uint8_t getCounter(void);
extern void initIsrCounter(void);

#endif //__HAL_UART_H__
