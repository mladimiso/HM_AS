/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX

#include "hal/includes/hal_driver.h"
#include "dll/includes/dll.h"
#include "apl/includes/apl.h"

#include "common/includes/circular_buffer.h"


char poruka[] = "Miso";
CircularBuffer_t cMain;
uint8_t dataReadyPC;
uint8_t dataReadyCC2530;

osThreadId tID_app;

void app(void const *arg);

osThreadDef(app,
						osPriorityNormal,
						1,
						0
						);

/*
 * main: initialize and start the system
 */
int main (void) 
{
  osKernelInitialize ();                    // initialize CMSIS-RTOS
	
  // initialize peripherals here
	halInitDriver();
	dllInit();
	aplInit();
	circularInit(&cMain);
 
	circularPut(&cMain, 'T');
	circularPut(&cMain, 'E');
	circularPut(&cMain, 'S');
	circularPut(&cMain, 'T');
	
	halUARTWrite(PC, &cMain, 0);
  // create 'thread' functions that start executing,
  // example: tid_name = osThreadCreate (osThread(name), NULL);
	
	tID_app = osThreadCreate(osThread(app),
													 NULL
													 );
	
	//circularPut(&cMain, *k);
	//halUARTWrite(PC, &cMain, 0);
	
  osKernelStart ();                         // start thread execution 
}


void app(void const *arg)
{
	Data_t aData;
//	aData.devID = 0x1112;
	aData.packNum = 0;
	srand(8);
	for(;;)
	{
	#ifdef STELLARIS_MAIN
		if (dataReadyPC)
		{
			aData.data = 55;
			aData.devID = 0x1111;
			//dllDataRequest(&aData, PC);
			aplSendData(55, 0x1111, PC);
			dataReadyPC = 0;
			osDelay(2);
		}
		if (dataReadyCC2530)
		{
			aData.data = getData(aData.devID);
			aData.devID = 0x1112;
			dllDataRequest(&aData, PC);
			dataReadyCC2530 = 0;
			osDelay(2);
		}
	#else
		aData.data = 25 - rand() % 4;	
		dllDataRequest(&aData, PC);
		osDelay(3000);
	#endif
	}
		
}
