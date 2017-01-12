/*
*********************************************************************************************************
* @file    	kernel.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-10-14 16:10:26
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	内核源文件
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
* Description	: 内核初始化
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
* Description	: 主机嗅探任务
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
					ECHO(DEBUG_KERNEL_HID, "嗅探到键盘...");
				}
				else if (HID_Type == HID_MOUSE)
				{
					*hidHostPtr = HID_USR_MOUSE;
					HID_MouseInit(g_mousePtr);
					ECHO(DEBUG_KERNEL_HID, "嗅探到鼠标...");
				}
				else
				{
					ECHO(DEBUG_KERNEL_HID, "未知设备！");
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
			ECHO(DEBUG_KERNEL_HID, "重新枚举！");
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
* Description	: 获取空闲容量[单位：KB]
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
bool GetFreeMemory(const char *(*getPath)(void), uint32_t *totalSize, uint32_t *freeSize)
{
	*freeSize = 0;
	char rootPath[20] = {0};
	
	FATFS *fs;
	unsigned long freeClust = 0;
	if (getPath)
	{
		strcpy(rootPath, getPath());
	}
	FRESULT fresult = f_getfree(rootPath, &freeClust, &fs);
	if (fresult != FR_OK)
	{
		return false;
	}
	
	uint32_t totalSector = (fs->n_fatent - 2) * fs->csize;
	uint32_t freeSector = fs->csize * freeClust;
#if _MAX_SS!=512				  				    //扇区大小不是512字节,则转换为512字节	
	totalSector *= FS->ssize / 512;
	freeSector *= FS->ssize / 512;
#endif
	
	*totalSize = totalSector >> 1;
	*freeSize = freeSector >> 1;
	
	return true;
}



/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
