/*
*********************************************************************************************************
* @file    	App_DeviceManager.c
* @author  	SY
* @version 	V1.0.0
* @date    	2017-1-11 12:27:01
* @IDE	 	Keil V5.22.0.0
* @Chip    	STM32F407VE
* @brief   	设备管理器源文件
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
#include "app.h"
#include "App_Menu.h"

/*
*********************************************************************************************************
*                              				Private define
*********************************************************************************************************
*/
#define GUI_ID_DIALOG0     					(GUI_ID_USER + 0x00)

#define GUI_DIALOG_WIDTH					550
#define GUI_DIALOG_HEIGHT					250
#define GUI_DIALOG_START_X					(30)
#define GUI_DIALOG_START_Y					(30)



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
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
	{ FRAMEWIN_CreateIndirect, "Framewin", GUI_ID_DIALOG0, 
		GUI_DIALOG_START_X, GUI_DIALOG_START_Y, GUI_DIALOG_WIDTH, GUI_DIALOG_HEIGHT, 
		0, 0, 0 },
	{ TEXT_CreateIndirect, "Text", GUI_ID_TEXT0, 
		20, 30, 150, 32,
		0, 0x0, 0 },
	{ PROGBAR_CreateIndirect, "Progbar", GUI_ID_PROGBAR0, 
		180, 30, 200, 32,
		0, 0x0, 0 },
	
	{ TEXT_CreateIndirect, "Text", GUI_ID_TEXT1, 
		20, 80, 150, 32, 
		0, 0x0, 0 },
	{ PROGBAR_CreateIndirect, "Progbar", GUI_ID_PROGBAR1,
		180, 80, 200, 32,
		0, 0x0, 0 },
	
	{ TEXT_CreateIndirect, "Text", GUI_ID_TEXT2, 
		20, 130, 150, 32, 
		0, 0x0, 0 },
	{ SLIDER_CreateIndirect, "Slider", GUI_ID_SLIDER0,
		180, 130, 200, 32,
		0, 0x0, 0 },
  
};

static const char * _aLang[][SUPPORT_LANGUAGE_NUMS] = {
	{
		"设备管理器",
		"Device Manager",
	},	//1
	{
		"SD卡容量：",
		"SD Card Capacity:",
	},	//2
	{
		"USB容量：",
		"USB Capacity:",
	},	//3
	{
		"屏幕亮度：",
		"Screen Light:",
	},	//4
	
};

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
* Function Name : _GetLang
* Description	: 获取文本
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static const char *_GetLang(uint32_t Index) 
{
	const char *p = LANG_EXCEPTION;
	uint8_t languageType = GetLanguageType();
	
	Index--;
	if ((Index < GUI_COUNTOF(_aLang)) && (languageType < GUI_COUNTOF(_aLang[0]))) 
	{
		p = _aLang[Index][languageType];
	}
	
	return p;
}

/*
*********************************************************************************************************
* Function Name : WindowsConstructor
* Description	: 窗口构造函数
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void WindowsConstructor(WM_MESSAGE *pMsg) 
{
	WM_HWIN hWin = pMsg->hWin;
	WM_SetFocus(hWin);
	
	ECHO(DEBUG_APP_WINDOWS, "[APP] 构造 <设备管理器> 窗口");
}

/*
*********************************************************************************************************
* Function Name : WindowsDestructor
* Description	: 窗口析构函数
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void WindowsDestructor( WM_MESSAGE *pMsg )
{	
	ECHO(DEBUG_APP_WINDOWS, "[APP] 析构 <设备管理器> 窗口");
}

/*
*********************************************************************************************************
* Function Name : DeleteWindow
* Description	: 删除窗口
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void DeleteWindow( WM_MESSAGE *pMsg )
{
	WM_DeleteWindow(pMsg->hWin);	
	
	ECHO(DEBUG_APP_WINDOWS, "[APP] 删除 <设备管理器> 窗口");
}

/*
*********************************************************************************************************
* Function Name : _cbDesktop
* Description	: 桌面窗口回调函数
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void _cbDesktop(WM_MESSAGE *pMsg) 
{
	WM_HWIN hWin = pMsg->hWin;
	(void)hWin;
	
	switch (pMsg->MsgId) 
	{
		case WM_CREATE:	
			WindowsConstructor(pMsg);
			break;
		case WM_DELETE:
			WindowsDestructor(pMsg);
			break;
		case WM_PAINT:
			_PaintFrame();
			break;
		case MSG_USER_ESC:
		{
			DeleteWindow(pMsg);
			App_MenuTaskCreate();
			break;
		}		
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}

/*
*********************************************************************************************************
* Function Name : DialogConstructor
* Description	: 对话框构造函数
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void DialogConstructor(WM_MESSAGE *pMsg) 
{
	WM_HWIN hWin = pMsg->hWin;
	
	FRAMEWIN_SetFont(hWin, FRAME_FONT);
	FRAMEWIN_SetTitleHeight(hWin, DIALOG_TITLE_HEIGHT);
	FRAMEWIN_SetTextAlign(hWin, GUI_TA_HCENTER | GUI_TA_VCENTER);
	FRAMEWIN_SetText(hWin, _GetLang(1));
	
	WM_HWIN hChild;
	hChild = WM_GetDialogItem(hWin, GUI_ID_TEXT0);	
	TEXT_SetTextAlign(hChild, TEXT_CF_RIGHT | TEXT_CF_VCENTER);
	TEXT_SetFont(hChild, FRAME_TEXT_FONT);
	TEXT_SetText(hChild, _GetLang(2));
	
	hChild = WM_GetDialogItem(hWin, GUI_ID_TEXT1);	
	TEXT_SetTextAlign(hChild, TEXT_CF_RIGHT | TEXT_CF_VCENTER);
	TEXT_SetFont(hChild, FRAME_TEXT_FONT);
	TEXT_SetText(hChild, _GetLang(3));
	
	hChild = WM_GetDialogItem(hWin, GUI_ID_TEXT2);	
	TEXT_SetTextAlign(hChild, TEXT_CF_RIGHT | TEXT_CF_VCENTER);
	TEXT_SetFont(hChild, FRAME_TEXT_FONT);
	TEXT_SetText(hChild, _GetLang(4));
	
	
	hChild = WM_GetDialogItem(hWin, GUI_ID_PROGBAR0);	
	PROGBAR_SetFont(hChild, FRAME_PROGBAR_FONT);
	PROGBAR_SetMinMax(hChild, 0, 100);
	PROGBAR_SetTextColor(hChild, PROGBAR_SKINFLEX_L, GUI_WHITE);
	{
		float usedSize = 0;
		char usedBuff[50] = {0};
		
		float freeSize = GetFreeMemorySize(bsp_GetSDRootPath);	
		float totalSize = GetTotalMemorySize(bsp_GetSDRootPath);
		char totalBuff[20] = {0};
		sprintf(totalBuff, "%5.2f", totalSize);
		
		usedSize = totalSize - freeSize;
		sprintf(usedBuff, "%5.2f", usedSize);
			
		strcat(usedBuff, " / ");
		strcat(usedBuff, totalBuff);
		strcat(usedBuff, " MB");
		PROGBAR_SetText(hChild, usedBuff);
		uint32_t usedRate = 0;
		if (totalSize)
		{
			usedRate = usedSize / totalSize * 100;
		}			
		PROGBAR_SetValue(hChild, usedRate);
	}
	
	hChild = WM_GetDialogItem(hWin, GUI_ID_PROGBAR1);	
	PROGBAR_SetFont(hChild, FRAME_PROGBAR_FONT);
	PROGBAR_SetMinMax(hChild, 0, 100);
	PROGBAR_SetTextColor(hChild, PROGBAR_SKINFLEX_L, GUI_WHITE);
	
	{
		float usedSize = 0;
		char usedBuff[50] = {0};
		
		float freeSize = GetFreeMemorySize(bsp_GetUSBRootPath);	
		float totalSize = GetTotalMemorySize(bsp_GetUSBRootPath);		
		
		char totalBuff[20] = {0};
		sprintf(totalBuff, "%5.2f", totalSize);
		
		usedSize = totalSize - freeSize;
		sprintf(usedBuff, "%5.2f", usedSize);
			
		strcat(usedBuff, " / ");
		strcat(usedBuff, totalBuff);
		strcat(usedBuff, " MB");
		PROGBAR_SetText(hChild, usedBuff);
		uint32_t usedRate = 0;
		if (totalSize)
		{
			usedRate = usedSize / totalSize * 100;
		}			
		PROGBAR_SetValue(hChild, usedRate);
	}
}

/*
*********************************************************************************************************
* Function Name : _cbDialog
* Description	: 对话框窗口回调函数
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void _cbDialog(WM_MESSAGE *pMsg) 
{
	WM_HWIN hWin = pMsg->hWin;
	
	switch (pMsg->MsgId) 
	{
		case WM_INIT_DIALOG:	
			DialogConstructor(pMsg);
			break;
		case WM_KEY:
		{
			int key = ((WM_KEY_INFO*)(pMsg->Data.p))->Key;
			
			switch (key)
			{
				case GUI_KEY_LEFT:
				{
					GUI_SendKeyMsg(GUI_KEY_BACKTAB, true);
					break;
				}	
				case GUI_KEY_RIGHT:
				{
					GUI_SendKeyMsg(GUI_KEY_TAB, true);
					break;
				}			
				case GUI_KEY_ENTER:
				case GUI_KEY_ESCAPE:
				{									
					WM_SendMessageNoPara(WM_GetParent(hWin), MSG_USER_ESC);
					break;
				}
				default:				
					break;
			}
			break;
		}
		case WM_NOTIFY_PARENT:
		{
			int NCode = pMsg->Data.v;
			int Id;
			
			switch (NCode) 
			{
				case WM_NOTIFICATION_VALUE_CHANGED:
					Id = WM_GetId(pMsg->hWinSrc);					
					
					switch (Id)
					{
						case GUI_ID_SPINBOX0:
						case GUI_ID_SPINBOX1:
						{
														
							break;
						}
						default:
							break;
					}		
					break;
			}
			break;
		}
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}

/*
*********************************************************************************************************
* Function Name : App_DeviceManagerTaskCreate
* Description	: 创建设备管理器应用程序任务
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void App_DeviceManagerTaskCreate(void)
{
	WM_HWIN hWin = _CreateFrame(_cbDesktop);	
	GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbDialog, hWin, 0, 0);
}


/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
