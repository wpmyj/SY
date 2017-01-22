/*
*********************************************************************************************************
* @file    	app.h
* @author  	SY
* @version 	V1.0.0
* @date    	2016-10-11 15:10:28
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	应用层头文件
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
#ifndef __APP_H
#define __APP_H

/*
*********************************************************************************************************
*                              				Exported Includes
*********************************************************************************************************
*/
#include "kernel.h"
#include "ff_gen_drv.h"
#include "app_cfg.h"
#include "App_Utils.h"
#include "App_GUI_Cfg.h"
#include "emWinUtils.h"

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
extern WM_HWIN hSuperWindows;

/*
*********************************************************************************************************
*                              				Exported functions
*********************************************************************************************************
*/
void APP_Init( void );
KEY_MSG_REMAP_TypeDef *GetKeyMsgRemapHandle(void);


#endif
/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
