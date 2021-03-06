#ifndef __DEFINES_H__
#define __DEFINES_H__

//#define HAL_UART0			true			// for interrupts enable - \b true, else \b false
//#define HAL_UART1			true			// for interrupts enable - \b true, else \b false
//#define	HAL_UART2			true			// for interrupts enable - \b true, else \b false


//*****************************************************************************
// defines for config
//*****************************************************************************
//#define DEBUG_MODE
#define PART_LM3S9B90
#define __CMSIS_RTOS__

#define PORT_D
#define DEBUG_MODE_ISR																	false
#define STELLARIS																				true
#define STELLARIS_STELARIS															false//true
#define HAL_UART0																				true
#define HAL_UART1																				true
	
#define PC																							0x00
#define CC2530																					0x01
#define BAUDRATE_PC																		115200

#if defined(STELLARIS_STELARIS) && (true == STELLARIS_STELARIS)
	#define BAUDRATE_CC2530															115200
#else
 #define BAUDRATE_CC2530																9600
#endif


#endif // __DEFINES_H__
