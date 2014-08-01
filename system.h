#ifndef __SYSTEM_H__
#define __SYSTEM_H__

/** CMSIS SystemInit */
void SystemInit();

/** User can redefine this function to handle SystemInit errors */
void SystemInitError(uint8_t error_source);
	#define SYSTEM_INIT_ERROR_FLASH 0x01
	#define SYSTEM_INIT_ERROR_PLL 0x02
	#define SYSTEM_INIT_ERROR_CLKSRC 0x04
	#define SYSTEM_INIT_ERROR_HSI 0x08

#endif
