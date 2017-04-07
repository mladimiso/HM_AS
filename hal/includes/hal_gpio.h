#ifndef __HAL_GPIO_H__
#define __HAL_GPIO_H__

#include "common/includes/defines.h"
#include "common/includes/includes.h"
#include "common/includes/status.h"

#define HIGH				0x01
#define LOW					0x00

typedef enum port
{
	HW_PORT_A = 0,
	HW_PORT_B,
	HW_PORT_C,
	HW_PORT_D,
	HW_PORT_E,
	HW_PORT_F,
	HW_PORT_G,
	HW_PORT_H,
	HW_PORT_J
} Port_t;


extern void halGPIOInit(void);
extern uint8_t getPortStatus(Port_t pPort);
extern Status_t halGPIOInput(Port_t pPort, 
														 uint8_t uPins,			/* pin(s) are specified  
																										 using a bit-packed byte */
														 uint8_t uType				/* most 4 bits are for current strenght 
																										 0 - 2mA, 4 - 4mA and 8 - 8mA
																										 least 4 bits are for output type 
																										 0 - STD push-pull, 1 - OpenDrain */
														);
extern Status_t halGPIOOutput(Port_t pPort, 
															uint8_t uiPins,			/* pin(s) are specified  
																										 using a bit-packed byte */
															uint8_t uiType			/* most 4 bits are for current strenght 
																										 0 - 2mA, 4 - 4mA and 8 - 8mA
																										 least 4 bits are for output type 
																										 0 - STD push-pull, 1 - OpenDrain */
															);

extern void halGPIOPinWrite(Port_t pPort, 
														uint8_t uPin,
														uint8_t uVal
														);
extern uint8_t halGPIOPinRead(Port_t pPort,
															uint8_t uPin
															);


#endif //__HAL_GPIO_H__
