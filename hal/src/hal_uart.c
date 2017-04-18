#include "cmsis_os.h"

#include "hal\includes\hal_gpio.h"
#include "hal\includes\hal_uart.h"



static uint8_t isrCounter;
//static uint8_t isr1Counter;

//#if (defined HAL_TIMER) && (HAL_TIMER == TRUE)
//*****************************************************************************
//
//! Local functions
//
//*****************************************************************************
//uint16_t getCounter(void);

//*****************************************************************************
//
// Interrupt service routines
//
//*****************************************************************************
void halUART0RxISR(void);
void halUART1RxISR(void);
void halUART2RxISR(void);

//*****************************************************************************
//
//! Initialization of the UART interface, UART0 on portA - \b 0 and \b 1
//! 8 bits, parity-none, 1 stop-bit, data-rate 115200
//!
//! \return None.
//
//*****************************************************************************
void halUARTInit(void)
{
#if defined(HAL_UART0) 
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	
	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	
	#if defined(HAL_UART0) && (true == HAL_UART0)
	
		UARTFIFOEnable(UART0_BASE);
		UARTFIFOLevelSet(UART0_BASE, UART_FIFO_TX1_8, UART_FIFO_RX1_8);
	
		UARTIntRegister(UART0_BASE, halUART0RxISR);
		UARTIntClear(UART0_BASE, UART_INT_RX );
		UARTIntEnable(UART0_BASE, UART_INT_RX );

		IntEnable(INT_UART0);
		
	#ifdef __CMSIS_RTOS__
			circularInit(&rxBufferPC);
	#else
			circularInit(&rxBuffer0);
	#endif
	
	#endif
#endif

#ifdef HAL_UART1
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	
	GPIOPinConfigure(GPIO_PB4_U1RX);
	GPIOPinConfigure(GPIO_PB5_U1TX);
	
	GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5);
	
	#if (true == HAL_UART1)
	
		UARTFIFOEnable(UART1_BASE);
		UARTFIFOLevelSet(UART1_BASE, UART_FIFO_TX1_8, UART_FIFO_RX1_8);
	
		UARTIntRegister(UART1_BASE, halUART1RxISR);
		UARTIntClear(UART1_BASE, UART_INT_RX );
		UARTIntEnable(UART1_BASE, UART_INT_RX );

		IntEnable(INT_UART1);
	
	#ifdef __CMSIS_RTOS__
			circularInit(&rxBufferCC2530);
	#else
		  circularInit(&rxBuffer1);
	#endif
	
	#endif
#endif

#ifdef HAL_UART2
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
	
	GPIOPinConfigure(GPIO_PG0_U2RX);
	GPIOPinConfigure(GPIO_PG1_U2TX);
	
	GPIOPinTypeUART(GPIO_PORTG_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	
	#if (true == HAL_UART2)
	
		UARTFIFOEnable(UART2_BASE);
		UARTFIFOLevelSet(UART2_BASE, UART_FIFO_TX1_8, UART_FIFO_RX1_8);
	
		UARTIntRegister(UART2_BASE, halUART2RxISR);
		UARTIntClear(UART2_BASE, UART_INT_RX );
		UARTIntEnable(UART2_BASE, UART_INT_RX );

		IntEnable(INT_UART2);
	
		circularInit(&rxBuffer2);

	#endif
#endif

}

//*****************************************************************************
//
//! Initialization of the UART interface, UART0 on portA - \b 0 and \b 1
//! 8 bits, parity-none, 1 stop-bit, data-rate 115200 interrupt RX mode
//! with 1/8 level trigger set (2 bytes in FIFO reg.)
//!
//! \return None.
//
//*****************************************************************************


//*****************************************************************************
//
//! UART RX ISRs (bez RTOS-a)
//!
//! \return None.
//
//*****************************************************************************
 void halUART0RxISR(void)
 {
	 #ifdef __CMSIS_RTOS__
			uint32_t ulStatus;
			uint8_t tmp;
			ulStatus = (uint32_t)UARTIntStatus(UART0_BASE, true);
			UARTIntClear(UART0_BASE, ulStatus);
			
			if(ulStatus & (UART_INT_RX))
			{
				tmp = (uint8_t)UARTCharGet(UART0_BASE);
				circularPut(&rxBufferPC, tmp);
			}
		
	 #else
			uint32_t ulStatus;
			uint8_t tmp;
		
			ulStatus = (uint32_t)UARTIntStatus(UART0_BASE, true);
			UARTIntClear(UART0_BASE, ulStatus);
		
			// UART_INT_RX | UART_INT_RT
			if(ulStatus & (UART_INT_RX))
			{
					while(UARTCharsAvail(UART0_BASE))
					{
						tmp = (uint8_t)UARTCharGet(UART0_BASE);
						circularPut(&rxBuffer0, tmp);
					}	
			}
		#endif
 }
 
 void halUART1RxISR(void)
 {
	 
	 #ifdef __CMSIS_RTOS__
		
			uint32_t ulStatus;
			uint8_t tmp;
			ulStatus = (uint32_t)UARTIntStatus(UART1_BASE, true);
			UARTIntClear(UART1_BASE, ulStatus);
			
			if(ulStatus & (UART_INT_RX))
			{
				 tmp = (uint8_t)UARTCharGet(UART1_BASE);
				 circularPut(&rxBufferCC2530, tmp);
			}
		
	
	 #else
			uint32_t ulStatus;
			uint8_t tmp;
		
			ulStatus = (uint32_t)UARTIntStatus(UART1_BASE, true);
			UARTIntClear(UART1_BASE, ulStatus);
		
			// UART_INT_RX | UART_INT_RT
			if(ulStatus & (UART_INT_RX))
			{
				while(UARTCharsAvail(UART1_BASE))
				{
						tmp = (uint8_t)UARTCharGet(UART1_BASE);
			//			circularPut(&rxBuffer1, tmp);
				}	
			}
	 #endif
 }

 void halUART2RxISR(void)
 {
	 	#ifdef __CMSIS_RTOS__
		
		#else
			uint32_t ulStatus;
			uint8_t tmp;
		
			ulStatus = (uint32_t)UARTIntStatus(UART2_BASE, true);
			UARTIntClear(UART2_BASE, ulStatus);
		
			// UART_INT_RX | UART_INT_RT
			if(ulStatus & (UART_INT_RX))
			{
					while(UARTCharsAvail(UART2_BASE))
					{
						tmp = (uint8_t)UARTCharGet(UART2_BASE);
//						circularPut(&rxBuffer2, tmp);
					}	
			}
		#endif
}
 
 
 
//*****************************************************************************
//
//! Checking is there any data in the recevie FIFO buffer
//!
//! \return Returns \b true if there is data in the receive FIFO or \b false
//! if there is no data in the receive FIFO.
//
//*****************************************************************************
uint8_t halUARTPoll(uint8_t port)
{
	switch (port)
	{
		case 0:
			return (uint8_t)UARTCharsAvail(UART0_BASE);
		case 1:
			return (uint8_t)UARTCharsAvail(UART1_BASE);
		case 2:
			return (uint8_t)UARTCharsAvail(UART2_BASE);
		default:
			// error port!
			return 255;
	}
	
}

//*****************************************************************************
//
//! Checking is there any space in the transmit FIFO buffer
//!
//! \return Returns \b true if there is space available in the transmit FIFO
//! or \b false if there is no space available in the transmit FIFO.
//
//*****************************************************************************
uint8_t halUARTAvail(uint8_t port)
{
	switch (port)
	{
		case 0:
			return (uint8_t)UARTSpaceAvail(UART0_BASE);
		case 1:
			return (uint8_t)UARTSpaceAvail(UART1_BASE);
		case 2:
			return (uint8_t)UARTSpaceAvail(UART2_BASE);
		default:
			// error!
			return 255;
	}
}

//*****************************************************************************
//
//! Receives data via UART
//!
//! \param rxBuffer is a pointer to storage for the received data
//!
//! \return Returns number of received data
//
//*****************************************************************************
uint8_t halUARTRead(uint8_t port,
										CircularBuffer_t *rxBuffer)
{
	uint8_t cnt = 0;
	uint8_t data;
	uint32_t portBase;
	
	switch (port)
	{
		case 0:
			portBase = UART0_BASE;
			break;
		case 1:
			portBase = UART1_BASE;
			break;
		case 2:
			portBase = UART2_BASE;
			break;
		default:
			//error port!
			return 255;
	}

	
	while(UARTCharsAvail(portBase))
	{	
		data = (uint8_t)UARTCharGet(portBase);
		circularPut(rxBuffer, data);
		cnt++;
	}
	return cnt;
}

//*****************************************************************************
//
//! Transmits data via UART
//!
//! \param txBuffer is a pointer to storage for the transmit data
//!
//! \return Returns number of transmited data
//
//*****************************************************************************
uint8_t halUARTWrite(uint8_t port,
										CircularBuffer_t *txBuffer, 
										uint8_t len)
{
	uint8_t cnt = 0;
	short i;
	uint8_t data;
	uint32_t portBase;
	
	switch (port)
	{
		case 0:
			portBase = UART0_BASE;
			break;
		case 1:
			portBase = UART1_BASE;
			break;
		case 2:
			portBase = UART2_BASE;
			break;
		default:
			//error port!
			return 255;
	}
	
	if (0 == len)
	{
		while(!circularIsEmpty(txBuffer))
		{
			circularGet(txBuffer, &data);
			UARTCharPut(portBase, data);
			cnt++;
			
		}// end while
	}// end if
	
	else if (len < circularSize(txBuffer))
	{
		for (i = 0; i < len; i++)
		{
			circularGet(txBuffer, &data);
			UARTCharPut(portBase, data);
		}// end for
		cnt = i;
		
	}// end else if
	
	else 
	{
		// error_uart();
		
	}// end else
		
	return cnt;
}

uint8_t halUARTOpenPort(uint8_t pNum, 
												uint32_t dRate,
												uint8_t sBit,
												uint8_t pBit
												)
{
	uint8_t retVal = 0;
	uint32_t portNum;
	uint32_t dataRate;
	uint32_t config = UART_CONFIG_WLEN_8;
	switch (pNum)
	{
		case 0:
			portNum = UART0_BASE;
			retVal++;
			break;
		case 1:
			portNum = UART1_BASE;
			retVal++;
			break;
		case 2:
			portNum = UART2_BASE;
			retVal++;
			break;
		default:
			// error port!
			return retVal;
	}
	
	switch (dRate)
	{
		case 115200:
			dataRate = dRate;
			retVal++;
			break;
		case 9600:
			dataRate = dRate;
			retVal++;
			break;
		default:
			// error baud rate!
			return retVal;
	}
	switch (sBit)
	{
		case 1:
			config |= UART_CONFIG_STOP_ONE;
			retVal++;
			break;
		case 2:
			config |= UART_CONFIG_STOP_TWO;
			retVal++;
			break;
		default:
			// error stop bit!
			return retVal;
	}
		
	switch (pBit)
	{
		case 0:
			config |= UART_CONFIG_PAR_NONE;
			retVal++;
			break;
		case 1:
			config |= UART_CONFIG_PAR_EVEN;
			retVal++;
			break;
		case 2:
			config |= UART_CONFIG_PAR_ODD;
			retVal++;
			break;
		case 3:
			config |= UART_CONFIG_PAR_ONE;
			retVal++;
			break;
		case 4:
			config |= UART_CONFIG_PAR_ZERO;
			retVal++;
			break;
		default:
			//error Parity bit!
			return retVal;
	}
	
	UARTConfigSetExpClk(portNum,
											SysCtlClockGet(),
											dataRate,
											config);
	
	return retVal;
}




//
// tmp 
//
uint8_t getCounter(void)
{
	return isrCounter;
}
void initIsrCounter(void)
{
	isrCounter = 0;
}
