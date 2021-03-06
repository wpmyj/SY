/*
*********************************************************************************************************
* @file    	file.h
* @author  	SY
* @version 	V1.0.0
* @date    	2016-11-1 08:43:55
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	文件相关操作头文件
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
#ifndef __FILE_H
#define __FILE_H

/*
*********************************************************************************************************
*                              				Exported Includes
*********************************************************************************************************
*/
#include "ff.h"
#include "diskio.h"

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

/*
*********************************************************************************************************
*                              				Exported functions
*********************************************************************************************************
*/
FRESULT CreatFolder( const char * const pPath );

ErrorStatus CopyFileFromDevices( const char * const pSourcePath, const char * const pTargetPath );
ErrorStatus CopyFolderFromDevices( const char * const pSourceFolderPath, const char * const pTargetFolderPath, \
				void (*GUI_CallBack)( const char *FolderNamePtr, const char *FileNamePtr ) );
ErrorStatus DeleteFileFromDevices( const char * const pTargetPath );
ErrorStatus DeleteFolderFromDevices( const char * const pTargetFolderPath, \
				void (*GUI_CallBack)( const char *FolderNamePtr, const char *FileNamePtr ) );

FRESULT FileStreamRead( const char *path, uint8_t *rxBodyPtr, uint32_t size );
FRESULT FileStreamWrite( const char *path, uint8_t *txBodyPtr, uint32_t size );
FRESULT FileStreamWriteAppend( const char *path, uint8_t *txBodyPtr, uint32_t size );


#endif
/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
