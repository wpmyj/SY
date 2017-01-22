/*
*********************************************************************************************************
* @file    	App_TimeDate.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-12-26 16:20:16
* @IDE	 	Keil V5.22.0.0
* @Chip    	STM32F407VE
* @brief   	时间日期设置源文件
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
#define GUI_DIALOG_HEIGHT					150
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
		10, 20, 84, 32, 
		0, 0x0, 0 },
	{ SPINBOX_CreateIndirect, "Spinbox", GUI_ID_SPINBOX0,
		94, 20, 84, 32, 
		0, 0x0, 0 },
	
	{ TEXT_CreateIndirect, "Text", GUI_ID_TEXT1, 
		178, 20, 84, 32, 
		0, 0x0, 0 },
	{ SPINBOX_CreateIndirect, "Spinbox", GUI_ID_SPINBOX1, 
		262, 20, 84, 32,  
		0, 0x0, 0 },
	
	{ TEXT_CreateIndirect, "Text", GUI_ID_TEXT2, 
		346, 20, 84, 32, 
		0, 0x0, 0 },
	{ SPINBOX_CreateIndirect, "Spinbox", GUI_ID_SPINBOX2,
		430, 20, 84, 32, 
		0, 0x0, 0 },
	
	{ TEXT_CreateIndirect, "Text", GUI_ID_TEXT3, 
		10, 60, 84, 32, 
		0, 0x0, 0 },
	{ SPINBOX_CreateIndirect, "Spinbox", GUI_ID_SPINBOX3,
		94, 60, 84, 32, 
		0, 0x0, 0 },
	
	{ TEXT_CreateIndirect, "Text", GUI_ID_TEXT4, 
		178, 60, 84, 32, 
		0, 0x0, 0 },
	{ SPINBOX_CreateIndirect, "Spinbox", GUI_ID_SPINBOX4, 
		262, 60, 84, 32,  
		0, 0x0, 0 },
	
	{ TEXT_CreateIndirect, "Text", GUI_ID_TEXT5, 
		346, 60, 84, 32, 
		0, 0x0, 0 },
	{ SPINBOX_CreateIndirect, "Spinbox", GUI_ID_SPINBOX5,
		430, 60, 84, 32, 
		0, 0x0, 0 },
};

static const char * _aLang[][SUPPORT_LANGUAGE_NUMS] = {
	{
		"时间日期设置",
		"Time Date Set",
	},	//1
	{
		"年",
		"Year",
	},	//2
	{
		"月",
		"Month",
	},	//3
	{
		"日",
		"Day",
	},	//4
	{
		"时",
		"Hour",
	},	//5
	{
		"分",
		"Minute",
	},	//6
	{
		"秒",
		"Second",
	},	//7
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
* Function Name : Constructor
* Description	: 构造函数
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void Constructor(WM_MESSAGE *pMsg) 
{
	WM_HWIN hWin = pMsg->hWin;
	
	tTime date = {0};
	ReadSystemDate(&date);
	tTime time = {0};
	ReadSystemTime(&time);
	
	FRAMEWIN_SetFont(hWin, FRAME_FONT);
	FRAMEWIN_SetTitleHeight(hWin, DIALOG_TITLE_HEIGHT);
	FRAMEWIN_SetTextAlign(hWin, GUI_TA_HCENTER | GUI_TA_VCENTER);
	FRAMEWIN_SetText(hWin, _GetLang(1));
	
	WM_HWIN hChild;
	hChild = WM_GetDialogItem(hWin, GUI_ID_TEXT0);	
	TEXT_SetTextAlign(hChild, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
	TEXT_SetFont(hChild, FRAME_TEXT_FONT);
	TEXT_SetText(hChild, _GetLang(2));
	
	hChild = WM_GetDialogItem(hWin, GUI_ID_TEXT1);	
	TEXT_SetTextAlign(hChild, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
	TEXT_SetFont(hChild, FRAME_TEXT_FONT);
	TEXT_SetText(hChild, _GetLang(3));

	hChild = WM_GetDialogItem(hWin, GUI_ID_TEXT2);
	TEXT_SetTextAlign(hChild, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
	TEXT_SetFont(hChild, FRAME_TEXT_FONT);	
	TEXT_SetText(hChild, _GetLang(4));
	
	hChild = WM_GetDialogItem(hWin, GUI_ID_TEXT3);
	TEXT_SetTextAlign(hChild, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
	TEXT_SetFont(hChild, FRAME_TEXT_FONT);	
	TEXT_SetText(hChild, _GetLang(5));
	
	hChild = WM_GetDialogItem(hWin, GUI_ID_TEXT4);
	TEXT_SetTextAlign(hChild, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
	TEXT_SetFont(hChild, FRAME_TEXT_FONT);	
	TEXT_SetText(hChild, _GetLang(6));
	
	hChild = WM_GetDialogItem(hWin, GUI_ID_TEXT5);
	TEXT_SetTextAlign(hChild, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
	TEXT_SetFont(hChild, FRAME_TEXT_FONT);	
	TEXT_SetText(hChild, _GetLang(7));
	
	hChild = WM_GetDialogItem(hWin, GUI_ID_SPINBOX0);	
	SPINBOX_SetFont(hChild, FRAME_SPINBOX_FONT);
	SPINBOX_SetStep(hChild, 1);
	SPINBOX_SetRange(hChild, 2000, 2050);
	SPINBOX_SetValue(hChild, date.usYear);
	
	hChild = WM_GetDialogItem(hWin, GUI_ID_SPINBOX1);	
	SPINBOX_SetFont(hChild, FRAME_SPINBOX_FONT);
	SPINBOX_SetStep(hChild, 1);
	SPINBOX_SetRange(hChild, 1, 12);
	SPINBOX_SetValue(hChild, date.ucMon);
	
	hChild = WM_GetDialogItem(hWin, GUI_ID_SPINBOX2);	
	SPINBOX_SetFont(hChild, FRAME_SPINBOX_FONT);
	SPINBOX_SetStep(hChild, 1);
	{
		uint8_t days = GetMonthDays(date.usYear, date.ucMon);
		
		SPINBOX_SetRange(hChild, 1, days);
	}
	SPINBOX_SetValue(hChild, date.ucMday);
	
	hChild = WM_GetDialogItem(hWin, GUI_ID_SPINBOX3);	
	SPINBOX_SetFont(hChild, FRAME_SPINBOX_FONT);
	SPINBOX_SetStep(hChild, 1);
	SPINBOX_SetRange(hChild, 0, 23);
	SPINBOX_SetValue(hChild, time.ucHour);
	
	hChild = WM_GetDialogItem(hWin, GUI_ID_SPINBOX4);	
	SPINBOX_SetFont(hChild, FRAME_SPINBOX_FONT);
	SPINBOX_SetStep(hChild, 1);
	SPINBOX_SetRange(hChild, 0, 59);
	SPINBOX_SetValue(hChild, time.ucMin);
	
	hChild = WM_GetDialogItem(hWin, GUI_ID_SPINBOX5);	
	SPINBOX_SetFont(hChild, FRAME_SPINBOX_FONT);
	SPINBOX_SetStep(hChild, 1);
	SPINBOX_SetRange(hChild, 0, 59);
	SPINBOX_SetValue(hChild, time.ucSec);	
	
	ECHO(DEBUG_APP_WINDOWS, "[APP] 构造 <时间日期> 窗口");
}

/*
*********************************************************************************************************
* Function Name : Destructor
* Description	: 窗口析构函数
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void Destructor( WM_MESSAGE *pMsg )
{	
	ECHO(DEBUG_APP_WINDOWS, "[APP] 析构 <时间日期> 窗口");
}

/*
*********************************************************************************************************
* Function Name : Delete
* Description	: 删除
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void Delete( WM_MESSAGE *pMsg )
{
	GUI_EndDialog(pMsg->hWin, 1);	
	
	ECHO(DEBUG_APP_WINDOWS, "[APP] 删除 <时间日期> 窗口");
}

/*
*********************************************************************************************************
* Function Name : Paint
* Description	: 绘制
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void Paint(void)
{
	GUI_SetBkColor(DIALOG_BKCOLOR);
	GUI_Clear();
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
			Constructor(pMsg);
			break;
		case WM_CREATE:			
			break;
		case WM_DELETE:
			Destructor(pMsg);
			break;
		case WM_PAINT:		
			Paint();			
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
					Delete(pMsg);
					App_MenuTaskCreate();
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
							WM_HWIN hChild = WM_GetDialogItem(hWin, GUI_ID_SPINBOX0);							
							int year = SPINBOX_GetValue(hChild);
							hChild = WM_GetDialogItem(hWin, GUI_ID_SPINBOX1);							
							int month = SPINBOX_GetValue(hChild);
							{
								uint8_t days = GetMonthDays(year, month);
								
								hChild = WM_GetDialogItem(hWin, GUI_ID_SPINBOX2);
								SPINBOX_SetRange(hChild, 1, days);
								int curDays = SPINBOX_GetValue(hChild);
								if (curDays > days)
								{
									SPINBOX_SetValue(hChild, days);
								}
							}							
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
* Function Name : App_TimeDateTaskCreate
* Description	: 创建时间日期应用程序任务
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void App_TimeDateTaskCreate(void)
{
	GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbDialog, hSuperWindows, 0, 0);
}


/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
