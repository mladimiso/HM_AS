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
uint8_t dataFromPC;
uint8_t dataFromCC2530;



//osThreadId tID_app;
osThreadId tID_testAppTemp;

//applications for test
void testAppTemp(void const *arg);
//void app(void const *arg);

osThreadDef(testAppTemp,
						osPriorityNormal,
						1,
						0
						);


//osThreadDef(app,
//						osPriorityNormal,
//						1,
//						0
//						);

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
	
//	tID_app = osThreadCreate(osThread(app),
//													 NULL
//													 );
	
#if defined(DEBUG_MODE_ISR) && (false == DEBUG_MODE_ISR)	
	tID_testAppTemp = osThreadCreate(osThread(testAppTemp),
																		NULL
																		);

	
  osKernelStart ();                         // start thread execution 
#else 
	while(1)
	{}
#endif
}


//void app(void const *arg)
//{
//	Data_t aData;
//	uint32_t data;
//	aData.devID = 0x1112;
//	aData.packNum = 0;
//	srand(8);
//	for(;;)
//	{
//	#ifdef STELLARIS_MAIN
//		if (dataFromPC)
//		{
//			//aData.data = 55;
//			//aData.devID = 0x1111;
//			//dllDataRequest(&aData, PC);
//			aplSendData(getData(0x1111, USER) + 1, 0x1111, PC);
//			dataFromPC = 0;
//			osDelay(2);
//		}
//		if (dataFromCC2530)
//		{
//			//aData.data = getData(aData.devID, SENSOR);
//			//aData.devID = 0x1112;
//			//dllDataRequest(&aData, PC);
//			aplSendData(getData(aData.devID, SENSOR), 0x1112, PC);
//			dataFromCC2530 = 0;
//			osDelay(2);
//		}
//	#else
//		data = 3000 - rand() % 1000;	
//		aplSendData(data, TEMP_SENSOR_INSIDE, PC);
//		osDelay(5000);
//	#endif
//	}
//		
//}


void testAppTemp(void const *arg)
{
	volatile int32_t userTemperature;
	volatile int32_t currentTemperature;
	volatile uint8_t t_regulation = 0;
	for(;;)
	{
		//
		// Display new value on monitor
		//
		if (dataFromCC2530)
		{
			currentTemperature = getData(TEMP_SENSOR_INSIDE, SENSOR);
			aplSendData(currentTemperature,
									TEMP_SENSOR_INSIDE,
									PC
									);
			dataFromCC2530 = 0;
		}
		if (2 == dataFromPC)
		{
			t_regulation = getData(TEMP_SENSOR_INSIDE, REGULATE);
			dataFromPC = 0;
		}
		else if (1 == dataFromPC)
		{
			uint32_t tmpT = getData(TEMP_SENSOR_INSIDE, USER);
			if (tmpT > AMBI_TEMP_MIN && tmpT < AMBI_TEMP_MAX)
			{
				userTemperature = tmpT;
				//t_regulation = 1; //treba obrisati kasnije i odkomentarisati 
													// "if (2 == dataFromPC){...}"
			}
			else 
			{
				userTemperature = DEFAULT_TEMPERATURE;
			}
			dataFromPC = 0;
		}
		if (1 == t_regulation)
		{
			if (abs(currentTemperature - userTemperature) < TEMP_OFFSET_25)
			{
				regulateTemperature(OFF, 0);
			}
			else
			{
				regulateTemperature(ON, currentTemperature - userTemperature);
			}
		}
		else if (0 == t_regulation)
		{
			regulateTemperature(OFF, 0);
		}
	}
		
}	
