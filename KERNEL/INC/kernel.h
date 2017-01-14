/*
*********************************************************************************************************
* @file    	kernel.h
* @author  	SY
* @version 	V1.0.0
* @date    	2016-10-14 16:10:26
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	�ں�ͷ�ļ�
*********************************************************************************************************
* @attention
*
* 
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Define to prevent recursive inclusion
*********************************************************************************************************
*/
#ifndef __KERNEL_H
#define __KERNEL_H

/*
*********************************************************************************************************
*                              				Exported Includes
*********************************************************************************************************
*/
#include "bsp.h"
#include "key.h"
#include "io.h"
#include "date.h"
#include "GUI.h"
#include "DIALOG.h"
#include "mouse.h"
#include "shell.h"

/*
*********************************************************************************************************
*                              				Exported define
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                              				Exported types
*********************************************************************************************************
*/
typedef bool (*UART_SEND_PACKAGE_T)( uint8_t _ucPort, const uint8_t *_ucaBuf, uint16_t _usLen );
typedef bool (*UART_RECEIVE_PACKAGE_T)( uint8_t _ucPort, const uint8_t *_ucaBuf );


typedef enum {
	HID_USR_IDLE = 0,
	HID_USR_WAIT,  
	HID_USR_START,
	HID_USR_MOUSE,  
	HID_USR_KEYBOARD,    
	HID_USR_REENUMERATE,
}HID_Usr_State;

/*
*********************************************************************************************************
*                              				Exported constants
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Exported macro
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Exported variables
*********************************************************************************************************
*/
extern HID_Usr_State HID_HostState;

/*
*********************************************************************************************************
*                              				Exported functions
*********************************************************************************************************
*/
void KERNEL_Init( void );
void HID_HostDetectTask( HID_Usr_State *hidHostPtr );
bool GetFreeMemory(const char *(*getPath)(void), uint32_t *totalSize, uint32_t *freeSize);
void SystemTick_Inc(uint32_t inc);
uint64_t GetSystemTick(void);
void PrintProcessSnapshot(void);

/*
*********************************************************************************************************
*                              				Exported Includes
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                              				Exported functions
*********************************************************************************************************
*/



#endif
/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
