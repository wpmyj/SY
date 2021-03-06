/*
*********************************************************************************************************
* @file    	pcf8563.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-10-18 15:21:26
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	PCF8563源文件
*********************************************************************************************************
* @attention
*
* 
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Private Includes
*********************************************************************************************************
*/
#include "bsp.h"


/*
*********************************************************************************************************
*                              				Private define
*********************************************************************************************************
*/
	

/*
*********************************************************************************************************
*                              				Private typedef
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Private constants
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Private macro
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Private variables
*********************************************************************************************************
*/
extern I2C_HandleTypeDef *I2C_PCF8563_Handle;

/*
*********************************************************************************************************
*                              				Private function prototypes
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Private functions
*********************************************************************************************************
*/
/*
*********************************************************************************************************
* Function Name : bsp_InitPCF8563
* Description	: 初始化PCF8563
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_InitPCF8563( void )
{
	uint8_t temp[10] = {0};
	
	bsp_WritePCF8563(0,temp,2);
	
	temp[0] = 0x80;
	temp[1] = 0x80;
	temp[2] = 0x80;
	temp[3] = 0x80;
	bsp_WritePCF8563(0x09,temp,4);
	
	ECHO(DEBUG_BSP_INIT, "PCF8563初始化 .......... OK");
}

/*
*********************************************************************************************************
* Function Name : bsp_WritePCF8563
* Description	: 向PCF8563写数据
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_WritePCF8563( uint16_t regAddress, uint8_t *pData, uint16_t size )
{
	bsp_I2C_WriteReg(I2C_PCF8563_Handle, regAddress, pData, size);
}

/*
*********************************************************************************************************
* Function Name : bsp_ReadPCF8563
* Description	: 从PCF8563读数据
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_ReadPCF8563( uint16_t regAddress, uint8_t *pData, uint16_t size )
{
	bsp_I2C_ReadReg(I2C_PCF8563_Handle, regAddress, pData, size);
}



/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
