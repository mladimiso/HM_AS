#include "hal\includes\hal_driver.h"

void halInitDriver(void)
{

	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | 
					SYSCTL_XTAL_16MHZ);
					
	halGPIOInit();
	halUARTInit();

}
