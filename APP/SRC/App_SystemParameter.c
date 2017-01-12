/*
*********************************************************************************************************
* @file    	App_SystemParameter.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-12-2 15:45:07
* @IDE	 	Keil V5.22.0.0
* @Chip    	STM32F407VE
* @brief   	系统参数页面源文件
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
#include "App_SystemParameter.h"
#include "App_Language.h"
#include "App_Menu.h"
#include "App_TypeWrite.h"

/*
*********************************************************************************************************
*                              				Private define
*********************************************************************************************************
*/
#define GUI_ID_DIALOG0     					(GUI_ID_USER + 0x00)
#define GUI_ID_DIALOG1     					(GUI_ID_USER + 0x01)
#define GUI_ID_DIALOG2     					(GUI_ID_USER + 0x02)

#define GUI_DIALOG_WIDTH					550
#define GUI_DIALOG_HEIGHT					350
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
};

static const GUI_WIDGET_CREATE_INFO _aDialogCreate1[] = {
	{ WINDOW_CreateIndirect, "Framewin", GUI_ID_DIALOG1, 
		10, 50, GUI_DIALOG_WIDTH-30, GUI_DIALOG_HEIGHT-50, 
		0, 0, 0 },
	{ DROPDOWN_CreateIndirect, "Dropdown", GUI_ID_DROPDOWN0,
		20, 50, 200, 32,
		0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "Button", GUI_ID_BUTTON0,
		30, 200, 100, 32, 
		0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "Button", GUI_ID_BUTTON1,
		180, 200, 100, 32, 
		0, 0x0, 0 },
};

static const GUI_WIDGET_CREATE_INFO _aDialogCreate2[] = {
	{ WINDOW_CreateIndirect, "Framewin", GUI_ID_DIALOG2, 
		10, 50, GUI_DIALOG_WIDTH-30, GUI_DIALOG_HEIGHT-50, 
		0, 0, 0 },	
	{ CHECKBOX_CreateIndirect, "Checkbox", GUI_ID_CHECK0,
		20, 50, 200, 32, 
		0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "Button", GUI_ID_BUTTON0,
		30, 200, 100, 32, 
		0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "Button", GUI_ID_BUTTON1,
		180, 200, 100, 32, 
		0, 0x0, 0 },
};

static const char * _aLang[][SUPPORT_LANGUAGE_NUMS] = {
	{
		"参数设置",
		"Parameter Set",
	},	//1
	{
		"控制参数",
		"Control Parameter",
	},	//2
	{
		"试验参数",
		"Test Parameter",
	},	//3
	{
		"系统最大力：",
		"Max Force Of System:",
	},	//4
	{
		"自动加载：",
		"Auto Load:",
	},	//5
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
	
	ECHO(DEBUG_APP_WINDOWS, "[APP] 构造 <参数设置> 窗口");
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
	ECHO(DEBUG_APP_WINDOWS, "[APP] 析构 <参数设置> 窗口");
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
	
	ECHO(DEBUG_APP_WINDOWS, "[APP] 删除 <参数设置> 窗口");
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
* Description	: 构造函数
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void DialogConstructor(WM_MESSAGE* pMsg) 
{
	WM_HWIN hWin = pMsg->hWin;
	
	FRAMEWIN_SetFont(hWin, FRAME_FONT);
	FRAMEWIN_SetTitleHeight(hWin, DIALOG_TITLE_HEIGHT);
	FRAMEWIN_SetTextAlign(hWin, GUI_TA_HCENTER | GUI_TA_VCENTER);
	FRAMEWIN_SetText(hWin, _GetLang(1));
	
}

/*
*********************************************************************************************************
* Function Name : _cbCallback
* Description	: 窗口回调函数
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void _cbDialog(WM_MESSAGE* pMsg) 
{
	WM_HWIN hWin = pMsg->hWin;
	
	switch (pMsg->MsgId) 
	{
		case WM_CREATE:	
			DialogConstructor(pMsg);
			break;
		case WM_PAINT:				
			break;
		case WM_KEY:
		{
			int key = ((WM_KEY_INFO*)(pMsg->Data.p))->Key;			
			switch (key)
			{
				case GUI_KEY_UP:
				{
					
					break;				
				}
				case GUI_KEY_DOWN:	
				{
									
					break;				
				}
				case GUI_KEY_ESCAPE:				
					WM_SendMessageNoPara(WM_GetParent(hWin), MSG_USER_ESC);
					break;
				default:
					
					break;
			}
			break;
		}
		case WM_NOTIFY_PARENT:
		{
						
			break;
		}
		case MSG_USER_PAD_CHANGED:
		case MSG_USER_PAD_DELETE:
		case MSG_USER_PAD_CLEAR:
		{					
			break;
		}		
		case MSG_USER_PAD_CANCEL:	
		{					
			break;
		}
		case MSG_USER_PAD_OK:	
		{			
			break;
		}
		case WM_SET_FOCUS:
		{
			break;
		}
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}

/*
*********************************************************************************************************
* Function Name : App_SystemParameterTaskCreate
* Description	: 创建系统参数任务
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void App_SystemParameterTaskCreate( void )
{
	WM_HWIN hWin = _CreateFrame(_cbDesktop);
	WM_HWIN hFrameWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbDialog, hWin, 0, 0);
	
	WM_HWIN hMultiPage = MULTIPAGE_CreateEx(10, 40, GUI_DIALOG_WIDTH - 30, GUI_DIALOG_HEIGHT - 80, WM_GetClientWindow(hFrameWin), WM_CF_SHOW, 0, 0);
	MULTIPAGE_SetFont(hMultiPage, FRAME_FONT);
	MULTIPAGE_SetAlign(hMultiPage, MULTIPAGE_ALIGN_TOP);
	
	WM_HWIN hDialog = GUI_CreateDialogBox(_aDialogCreate1, GUI_COUNTOF(_aDialogCreate1), _cbDialog, hMultiPage, 0, 0);
	MULTIPAGE_AddPage(hMultiPage, hDialog, _GetLang(2));
	
	hDialog = GUI_CreateDialogBox(_aDialogCreate2, GUI_COUNTOF(_aDialogCreate2), _cbDialog, hMultiPage, 0, 0);
	MULTIPAGE_AddPage(hMultiPage, hDialog, _GetLang(3));
	
	MULTIPAGE_SelectPage(hMultiPage, 0);
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
