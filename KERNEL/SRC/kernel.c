/*
*********************************************************************************************************
* @file    	kernel.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-10-14 16:10:26
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	�ں�Դ�ļ�
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
#include "kernel.h"
#include "ff_gen_drv.h"
#include "emWinUtils.h"
#include "SeqQueue.h"
#include "SeqList.h"
#include "usbh_usr.h"
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
HID_Usr_State HID_HostState;

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
* Function Name : KERNEL_Init
* Description	: �ں˳�ʼ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void KERNEL_Init( void )
{
	ECHO(DEBUG_KERNEL_INIT, "==================== KERNEL ====================");
	
	Key_Init();
	IO_Init();

	ECHO(DEBUG_KERNEL_INIT, "==================== END ====================\r\n");
}

/*
*********************************************************************************************************
* Function Name : HID_HostDetectTask
* Description	: ������̽����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void HID_HostDetectTask( HID_Usr_State *hidHostPtr )
{
	USB_HOST_STATUS_TypeDef USB_HostStatus = USBH_GetStatus();
	
	switch (*hidHostPtr)
	{
		case HID_USR_IDLE:
			*hidHostPtr = HID_USR_WAIT;
			break;
		case HID_USR_WAIT:
			*hidHostPtr = HID_USR_START;
			break;
		case HID_USR_START:
		{
			if (USB_HostStatus == USB_HOST_READY)
			{
				HID_TypeTypeDef HID_Type = USBH_HID_GetDeviceType(&hUsbHostHS);
				if (HID_Type == HID_KEYBOARD)
				{
					*hidHostPtr = HID_USR_KEYBOARD;
					ECHO(DEBUG_KERNEL_HID, "��̽������...");
				}
				else if (HID_Type == HID_MOUSE)
				{
					*hidHostPtr = HID_USR_MOUSE;
					HID_MouseInit(g_mousePtr);
					ECHO(DEBUG_KERNEL_HID, "��̽�����...");
				}
				else
				{
					ECHO(DEBUG_KERNEL_HID, "δ֪�豸��");
				}
			}
			break;
		}
		case HID_USR_MOUSE:
			HID_MouseProcess(g_mousePtr);
			break;
		case HID_USR_KEYBOARD:
			
			break;
		case HID_USR_REENUMERATE:
			USBH_ReEnumerate(&hUsbHostHS);		
			*hidHostPtr = HID_USR_START;
			ECHO(DEBUG_KERNEL_HID, "����ö�٣�");
			break;
		default:
			*hidHostPtr = HID_USR_IDLE;
			break;
	}
	
	if (USB_HostStatus == USB_HOST_DISCONNECT)
	{
		*hidHostPtr = HID_USR_IDLE;
	}
}

/*
*********************************************************************************************************
* Function Name : GetFreeMemory
* Description	: ��ȡ��������[��λ��MB]
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
float GetFreeMemorySize(const char *(*getPath)(void))
{
	float freeSize = 0;
	char rootPath[20] = {0};
	
	FATFS *fs;
	unsigned long size = 0;
	if (getPath)
	{
		strcpy(rootPath, getPath());
	}
	FRESULT fresult = f_getfree(rootPath, &size, &fs);
	if (fresult != FR_OK)
	{
		return 0;
	}
	
	uint32_t totalWord = size * fs->csize;
	uint32_t totalByte = totalWord >> 1;
	freeSize = totalByte / 1024.0f;
	
	return freeSize;
}

/*
*********************************************************************************************************
* Function Name : GetTotalMemorySize
* Description	: ��ȡSD��ȫ������[��λ��MB]
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
float GetTotalMemorySize(const char *(*getPath)(void))
{
	float totalSize = 0;
	char rootPath[20] = {0};
	
	FATFS *fs;
	unsigned long size = 0;
	if (getPath)
	{
		strcpy(rootPath, getPath());
	}
	FRESULT fresult = f_getfree(rootPath, &size, &fs);
	if (fresult != FR_OK)
	{
		return 0;
	}
	
	if (fs->n_fatent > 2)
	{
		uint32_t totalWord = (fs->n_fatent - 2) * fs->csize;
		uint32_t totalByte = totalWord >> 1;
		totalSize = totalByte / 1024.0f;
	}
	
	return totalSize;
}


/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
