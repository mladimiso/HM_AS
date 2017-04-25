#include "hal\includes\hal_gpio.h"

enum defaultPortValue
{
	DEFAULT_VAL_PORT_A = 0x3f,
	DEFAULT_VAL_PORT_B = 0x0f,
	DEFAULT_VAL_PORT_C = 0x0f,
	DEFAULT_VAL_PORT_D = 0x00,
	DEFAULT_VAL_PORT_E = 0x00,
	DEFAULT_VAL_PORT_F = 0xc0,
	DEFAULT_VAL_PORT_G = 0x7c,
	DEFAULT_VAL_PORT_H = 0x18,
	DEFAULT_VAL_PORT_J = 0xf8
};

static uint8_t	portStatus[] = {
												DEFAULT_VAL_PORT_A,
												DEFAULT_VAL_PORT_B,
												DEFAULT_VAL_PORT_C,	
												DEFAULT_VAL_PORT_D,
												DEFAULT_VAL_PORT_E,
												DEFAULT_VAL_PORT_F,
												DEFAULT_VAL_PORT_G,
												DEFAULT_VAL_PORT_H,
												DEFAULT_VAL_PORT_J
};


//*****************************************************************************
//
//! Local functions
//
//*****************************************************************************
static uint8_t checkPins(Port_t pPort, 
												 uint8_t uPins);
static void setPins(Port_t pPort,
										uint8_t uPins);
	

//*****************************************************************************
//
//! Initialization ports, enabling clk to the desire GPIOs, depending on 
//! defines that has been defined in <tt>defines.h</tt>
//
//*****************************************************************************
void halGPIOInit(void)
{
	#ifdef PORT_A
			SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
			SysCtlDelay(5);
	#endif
	
	#ifdef PORT_B
			SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
			SysCtlDelay(5);
	#endif	
	
	#ifdef PORT_C

			SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
			SysCtlDelay(5);
	#endif
	
	#ifdef PORT_D
	
			SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
//			SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD;
			SysCtlDelay(5);
		
	#endif
	
	#ifdef PORT_E
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
		SysCtlDelay(5);
	#endif	

	#ifdef PORT_F
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
		SysCtlDelay(5);
	#endif
	
	#ifdef PORT_G
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
		SysCtlDelay(5);
	#endif
	
	#ifdef PORT_H
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
		SysCtlDelay(5);
	#endif
	
	#ifdef PORT_J
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
		SysCtlDelay(5);
	#endif
	
}


Status_t halGPIOInput(Port_t port, 
											uint8_t pins,
											uint8_t type
											)
{
	uint32_t uiPort;
	uint32_t currentStrenght;
	uint32_t uiType;
	
	if ( 0 != checkPins(port, pins))
	{
		return GPIO_ERROR;
	}
	else
	{
		switch (port)
		{
			case HW_PORT_A:
				uiPort = GPIO_PORTA_BASE;
				break;
			case HW_PORT_B:
				uiPort = GPIO_PORTB_BASE;
				break;
			case HW_PORT_C:
				uiPort = GPIO_PORTC_BASE;
				break;
			case HW_PORT_D:
				uiPort = GPIO_PORTD_BASE;
				break;
			case HW_PORT_E:
				uiPort = GPIO_PORTE_BASE;
				break;
			case HW_PORT_F:
				uiPort = GPIO_PORTF_BASE;
				break;
			case HW_PORT_G:
				uiPort = GPIO_PORTG_BASE;
				break;
			case HW_PORT_H:
				uiPort = GPIO_PORTH_BASE;
				break;
			case HW_PORT_J:
				uiPort = GPIO_PORTJ_BASE;
				break;
			default:
				return GPIO_ERROR;
		}
		
		//
		//	current strenght
		//
		switch ((type >> 4) & 0x0f)
		{
			case 0:
				currentStrenght = GPIO_STRENGTH_2MA;
				break;
			case 4:
				currentStrenght = GPIO_STRENGTH_4MA;
				break;
			case 8:
				currentStrenght = GPIO_STRENGTH_8MA;
				break;
			default:
				return GPIO_ERROR;
				
		}
		
		switch (type & 0x0f)
		{
			case 0:
				uiType = GPIO_PIN_TYPE_STD;
				break;
			case 1:
				uiType = GPIO_PIN_TYPE_STD;
				break;
			default:
				return GPIO_ERROR;
		}
		
		
		GPIOPadConfigSet(uiPort, pins, currentStrenght, uiType);
		GPIODirModeSet(uiPort, pins, GPIO_DIR_MODE_IN);
		setPins(port, pins);
		return GPIO_SUCCES;
	}
}

Status_t halGPIOOutput(Port_t port, 
											 uint8_t pins,
											 uint8_t type
											)
{
	uint32_t uiPort;
	uint32_t currentStrenght;
	uint32_t uiType;
	
	if ( 0 != checkPins(port, pins))
	{
		return GPIO_ERROR;
	}
	else
	{
		switch (port)
		{
			case HW_PORT_A:
				uiPort = GPIO_PORTA_BASE;
				break;
			case HW_PORT_B:
				uiPort = GPIO_PORTB_BASE;
				break;
			case HW_PORT_C:
				uiPort = GPIO_PORTC_BASE;
				break;
			case HW_PORT_D:
				uiPort = GPIO_PORTD_BASE;
				break;
			case HW_PORT_E:
				uiPort = GPIO_PORTE_BASE;
				break;
			case HW_PORT_F:
				uiPort = GPIO_PORTF_BASE;
				break;
			case HW_PORT_G:
				uiPort = GPIO_PORTG_BASE;
				break;
			case HW_PORT_H:
				uiPort = GPIO_PORTH_BASE;
				break;
			case HW_PORT_J:
				uiPort = GPIO_PORTJ_BASE;
				break;
			default:
				return GPIO_ERROR;
		}
		
		//
		//	current strenght
		//
		switch ((type >> 4) & 0x0f)
		{
			case 0:
				currentStrenght = GPIO_STRENGTH_2MA;
				break;
			case 4:
				currentStrenght = GPIO_STRENGTH_4MA;
				break;
			case 8:
				currentStrenght = GPIO_STRENGTH_8MA;
				break;
			default:
				return GPIO_ERROR;
				
		}
		
		switch (type & 0x0f)
		{
			case 0:
				uiType = GPIO_PIN_TYPE_STD;
				break;
			case 1:
				uiType = GPIO_PIN_TYPE_OD;
				break;
			default:
				return GPIO_ERROR;
		}
		
		
		GPIOPadConfigSet(uiPort, pins, currentStrenght, uiType);
		GPIODirModeSet(uiPort, pins, GPIO_DIR_MODE_OUT);
		GPIO_PORTD_DATA_R = 0x00;
		setPins(port, pins);
		return GPIO_SUCCES;
}
}

extern void halGPIOPinWrite(Port_t pPort, 
														uint8_t uPin,
														uint8_t uVal
														)
{
//	uint32_t uiPort;
		
	switch (pPort)
	{
		case HW_PORT_A:
			if (LOW == uVal)
			{
				GPIO_PORTA_DATA_R &= ~uPin;
			}
			else 
			{
				GPIO_PORTA_DATA_R |= uPin;
			}
//			uiPort = GPIO_PORTA_BASE;
			break;
			
		case HW_PORT_B:
			if (LOW == uVal)
			{
				GPIO_PORTB_DATA_R &= ~uPin;
			}
			else 
			{
				GPIO_PORTB_DATA_R |= uPin;
			}
//			uiPort = GPIO_PORTB_BASE;
			break;
			
		case HW_PORT_C:
			if (LOW == uVal)
			{
				GPIO_PORTC_DATA_R &= ~uPin;
			}
			else 
			{
				GPIO_PORTC_DATA_R |= uPin;
			}
//			uiPort = GPIO_PORTC_BASE;
			break;
		case HW_PORT_D:
			if (LOW == uVal)
			{
				GPIO_PORTD_DATA_R &= ~uPin;
			}
			else 
			{
				GPIO_PORTD_DATA_R |= uPin;
			}
//			uiPort = GPIO_PORTD_BASE;
			break;
		case HW_PORT_E:
			if (LOW == uVal)
			{
				GPIO_PORTE_DATA_R &= ~uPin;
			}
			else 
			{
				GPIO_PORTE_DATA_R |= uPin;
			}
//			uiPort = GPIO_PORTE_BASE;
			break;
		case HW_PORT_F:
			if (LOW == uVal)
			{
				GPIO_PORTF_DATA_R &= ~uPin;
			}
			else 
			{
				GPIO_PORTF_DATA_R |= uPin;
			}
//			uiPort = GPIO_PORTF_BASE;
			break;
		case HW_PORT_G:
			if (LOW == uVal)
			{
				GPIO_PORTG_DATA_R &= ~uPin;
			}
			else 
			{
				GPIO_PORTG_DATA_R |= uPin;
			}
//			uiPort = GPIO_PORTG_BASE;
			break;
		case HW_PORT_H:
			if (LOW == uVal)
			{
				GPIO_PORTH_DATA_R &= ~uPin;
			}
			else 
			{
				GPIO_PORTH_DATA_R |= uPin;
			}
//			uiPort = GPIO_PORTH_BASE;
			break;
		case HW_PORT_J:
			if (LOW == uVal)
			{
				GPIO_PORTJ_DATA_R &= ~uPin;
			}
			else 
			{
				GPIO_PORTJ_DATA_R |= uPin;
			}
//			uiPort = GPIO_PORTJ_BASE;
			break;
		default:
			break;
				//return GPIO_ERROR;
		}// end switch()
		
//		GPIOPinWrite(uiPort, uPin, uVal);
		
}
	
extern uint8_t halGPIOPinRead(Port_t pPort,
															uint8_t uPins)
{
	uint32_t uiPort;
	
	switch (pPort)
	{
		case HW_PORT_A:
			uiPort = GPIO_PORTA_BASE;
			break;
		case HW_PORT_B:			
			uiPort = GPIO_PORTB_BASE;
			break;
		case HW_PORT_C:
			uiPort = GPIO_PORTC_BASE;
			break;
		case HW_PORT_D:
			uiPort = GPIO_PORTD_BASE;
   		break;
		case HW_PORT_E:
			uiPort = GPIO_PORTE_BASE;
			break;
		case HW_PORT_F:
			uiPort = GPIO_PORTF_BASE;
			break;
		case HW_PORT_G:
			uiPort = GPIO_PORTG_BASE;
			break;
		case HW_PORT_H:
			uiPort = GPIO_PORTH_BASE;
			break;
		case HW_PORT_J:
			uiPort = GPIO_PORTJ_BASE;
			break;
		default:
			break;
				//return GPIO_ERROR;
		}// end switch()
		
		return GPIOPinRead(uiPort, uPins);
}

extern uint8_t getPortStatus(Port_t pPort)
{
	return portStatus[pPort];
}

//*****************************************************************************
//
//! Returns 0 if pin(s) on that port are available
//
//*****************************************************************************
static uint8_t checkPins(Port_t pPort, 
												uint8_t uPins)
{
		return portStatus[pPort] & uPins;
}

static void setPins(Port_t pPort,
	  								uint8_t uPins)
{
	portStatus[pPort] |= uPins;
}
