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
	{ MULTIPAGE_CreateIndirect, "Multipage", GUI_ID_MULTIPAGE0,
		10, 10, GUI_DIALOG_WIDTH - 30, GUI_DIALOG_HEIGHT - 60,
		0, 0x0, 0 },
};

static const GUI_WIDGET_CREATE_INFO _aDialogCreate1[] = {
	{ WINDOW_CreateIndirect, "Framewin", GUI_ID_DIALOG1, 
		10, 10, GUI_DIALOG_WIDTH-30, GUI_DIALOG_HEIGHT-40, 
		0, 0, 0 },
	{ TEXT_CreateIndirect, "Text", GUI_ID_TEXT0,
		20, 20, 150, 32,
		0, 0x0, 0 },
	{ DROPDOWN_CreateIndirect, "Dropdown", GUI_ID_DROPDOWN0,
		180, 20, 200, 32,
		0, 0x0, 0 },
	{ TEXT_CreateIndirect, "Text", GUI_ID_TEXT1,
		20, 60, 150, 32,
		0, 0x0, 0 },
	{ DROPDOWN_CreateIndirect, "Dropdown", GUI_ID_DROPDOWN1,
		180, 60, 200, 32,
		0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "Button", GUI_ID_BUTTON0,
		70, 210, 150, 32, 
		0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "Button", GUI_ID_BUTTON1,
		310, 210, 150, 32, 
		0, 0x0, 0 },
};

static const GUI_WIDGET_CREATE_INFO _aDialogCreate2[] = {
	{ WINDOW_CreateIndirect, "Framewin", GUI_ID_DIALOG2, 
		10, 10, GUI_DIALOG_WIDTH-30, GUI_DIALOG_HEIGHT-40, 
		0, 0, 0 },	
	{ CHECKBOX_CreateIndirect, "Checkbox", GUI_ID_CHECK0,
		20, 50, 200, 32, 
		0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "Button", GUI_ID_BUTTON0,
		70, 210, 150, 32, 
		0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "Button", GUI_ID_BUTTON1,
		310, 210, 150, 32, 
		0, 0x0, 0 },
};

static const char * _aLang[][SUPPORT_LANGUAGE_NUMS] = {
	{
		"参数设置",
		"Parameter Set",
	},	//1
	{
		"试验参数",
		"Test Parameter",
	},	//2
	{
		"控制参数",
		"Control Parameter",
	},	//3
	{
		"系统最大力：",
		"Max Force Of System:",
	},	//4
	{
		"P·I",
		"P·I",
	},	//5
	{
		"P·II",
		"P·II",
	},	//6
	{
		"试样品种：",
		"Sample Variety:",
	},	//7
	{
		"上一页",
		"Before Page",
	},	//8
	{
		"下一页",
		"Next Page",
	},	//9
	{
		"P·O",
		"P·O",
	},	//10
	{
		"P·S·A",
		"P·S·A",
	},	//11
	{
		"P·S·B",
		"P·S·B",
	},	//12
	{
		"P·P",
		"P·P",
	},	//13
	{
		"P·F",
		"P·F",
	},	//14
	{
		"P·C",
		"P·C",
	},	//15
	{
		"矿粉S",
		"矿粉S",
	},	//16
	{
		"强度等级：",
		"Strength Grade:",
	},	//17
	{
		"32.5",
		"32.5",
	},	//18
	{
		"32.5R",
		"32.5R",
	},	//19
	{
		"42.5",
		"42.5",
	},	//20
	{
		"42.5R",
		"42.5R",
	},	//21
	{
		"52.5",
		"52.5",
	},	//22
	{
		"52.5R",
		"52.5R",
	},	//23
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
static void _cbDialog1(WM_MESSAGE* pMsg);
static void _cbDialog2(WM_MESSAGE* pMsg);

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
		case WM_USER_ESC:
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
	
	FRAMEWIN_SetActive(hWin, 1);
	
	FRAMEWIN_SetFont(hWin, FRAME_FONT);
	FRAMEWIN_SetTitleHeight(hWin, DIALOG_TITLE_HEIGHT);
	FRAMEWIN_SetTextAlign(hWin, GUI_TA_HCENTER | GUI_TA_VCENTER);
	FRAMEWIN_SetText(hWin, _GetLang(1));
	
	WM_HWIN hMultiPage = WM_GetDialogItem(hWin, GUI_ID_MULTIPAGE0);
	MULTIPAGE_SetFont(hMultiPage, FRAME_MULTIPAGE_FONT);
	MULTIPAGE_SetAlign(hMultiPage, MULTIPAGE_ALIGN_TOP);
	
	WM_HWIN hDialog = GUI_CreateDialogBox(_aDialogCreate1, GUI_COUNTOF(_aDialogCreate1), _cbDialog1, hMultiPage, 0, 0);
	MULTIPAGE_AddEmptyPage(hMultiPage, hDialog, _GetLang(2));
	
	hDialog = GUI_CreateDialogBox(_aDialogCreate2, GUI_COUNTOF(_aDialogCreate2), _cbDialog2, hMultiPage, 0, 0);
	MULTIPAGE_AddEmptyPage(hMultiPage, hDialog, _GetLang(3));
	
	MULTIPAGE_SelectPage(hMultiPage, 0);
}

/*
*********************************************************************************************************
* Function Name : DialogDestructor
* Description	: 析构函数
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void DialogDestructor(WM_MESSAGE* pMsg) 
{
	DeleteAllKeyMsgRemap(GetKeyMsgRemapHandle());
	
	ECHO(DEBUG_APP_WINDOWS, "[APP] 析构 <参数设置> 对话框");
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
		case WM_INIT_DIALOG:	
			DialogConstructor(pMsg);
			break;
		case WM_CREATE:				
			break;
		case WM_DELETE:
			DialogDestructor(pMsg);
			break;
		case WM_PAINT:	
			break;
		case WM_KEY:
		{			
			break;
		}
		case WM_NOTIFY_PARENT:
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
* Function Name : DialogConstructor1
* Description	: 构造函数
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void DialogConstructor1(WM_MESSAGE* pMsg) 
{
	WM_HWIN hWin = pMsg->hWin;
	
	WM_HWIN hText = WM_GetDialogItem(hWin, GUI_ID_TEXT0);
	TEXT_SetFont(hText, FRAME_TEXT_FONT);
	TEXT_SetTextAlign(hText, TEXT_CF_RIGHT | TEXT_CF_VCENTER);
	TEXT_SetText(hText, _GetLang(7));
	
	WM_HWIN hDropDown = WM_GetDialogItem(hWin, GUI_ID_DROPDOWN0);
	DROPDOWN_SetFont(hDropDown, FRAME_DROPDOWN_FONT);
	DROPDOWN_AddString(hDropDown, _GetLang(5));
	DROPDOWN_AddString(hDropDown, _GetLang(6));
	DROPDOWN_AddString(hDropDown, _GetLang(10));
	DROPDOWN_AddString(hDropDown, _GetLang(11));
	DROPDOWN_AddString(hDropDown, _GetLang(12));
	DROPDOWN_AddString(hDropDown, _GetLang(13));
	DROPDOWN_AddString(hDropDown, _GetLang(14));
	DROPDOWN_AddString(hDropDown, _GetLang(15));
	DROPDOWN_AddString(hDropDown, _GetLang(16));
	DROPDOWN_SetListHeight(hDropDown, 80);
//	DROPDOWN_SetAutoScroll(hDropDown, 1);	//添加会进入硬件中断
	DROPDOWN_SetScrollbarWidth(hDropDown, 20);
	
	hText = WM_GetDialogItem(hWin, GUI_ID_TEXT1);
	TEXT_SetFont(hText, FRAME_TEXT_FONT);
	TEXT_SetTextAlign(hText, TEXT_CF_RIGHT | TEXT_CF_VCENTER);
	TEXT_SetText(hText, _GetLang(17));
	
	hDropDown = WM_GetDialogItem(hWin, GUI_ID_DROPDOWN1);
	DROPDOWN_SetFont(hDropDown, FRAME_DROPDOWN_FONT);
	DROPDOWN_AddString(hDropDown, _GetLang(18));
	DROPDOWN_AddString(hDropDown, _GetLang(19));
	DROPDOWN_AddString(hDropDown, _GetLang(20));
	DROPDOWN_AddString(hDropDown, _GetLang(21));
	DROPDOWN_AddString(hDropDown, _GetLang(22));
	DROPDOWN_AddString(hDropDown, _GetLang(23));
	DROPDOWN_SetListHeight(hDropDown, 80);
//	DROPDOWN_SetAutoScroll(hDropDown, 1);	//添加会进入硬件中断
	DROPDOWN_SetScrollbarWidth(hDropDown, 20);
	
	WM_HWIN hButton = WM_GetDialogItem(hWin, GUI_ID_BUTTON0);
	BUTTON_SetFont(hButton, FRAME_BUTTON_FONT);
	BUTTON_SetText(hButton, _GetLang(8));
	
	hButton = WM_GetDialogItem(hWin, GUI_ID_BUTTON1);
	BUTTON_SetFont(hButton, FRAME_BUTTON_FONT);
	BUTTON_SetText(hButton, _GetLang(9));
	
	//添加按键重映射
	AddKeyMsgRemap(GetKeyMsgRemapHandle(), GUI_KEY_UP, NULL);
	AddKeyMsgRemap(GetKeyMsgRemapHandle(), GUI_KEY_DOWN, NULL);
	AddKeyMsgRemap(GetKeyMsgRemapHandle(), GUI_KEY_LEFT, NULL);
	AddKeyMsgRemap(GetKeyMsgRemapHandle(), GUI_KEY_RIGHT, NULL);
	AddKeyMsgRemap(GetKeyMsgRemapHandle(), GUI_KEY_ENTER, NULL);
}

/*
*********************************************************************************************************
* Function Name : Dialog1_RegisterDefaultKeyRemap
* Description	: 注册默认键值映射回调函数
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void Dialog1_RegisterDefaultKeyRemap(void)
{	
	Register_KeyMsgRemap(GetKeyMsgRemapHandle(), GUI_KEY_UP,\
		SendKeyUpMsg_CallBack);
	Register_KeyMsgRemap(GetKeyMsgRemapHandle(), GUI_KEY_DOWN,\
		SendKeyDownMsg_CallBack);
	Register_KeyMsgRemap(GetKeyMsgRemapHandle(), GUI_KEY_LEFT,\
		SendKeyLeftMsg_CallBack);
	Register_KeyMsgRemap(GetKeyMsgRemapHandle(), GUI_KEY_RIGHT,\
		SendKeyRightMsg_CallBack);
	Register_KeyMsgRemap(GetKeyMsgRemapHandle(), GUI_KEY_ENTER,\
		SendKeyEnterMsg_CallBack);
}

/*
*********************************************************************************************************
* Function Name : Dialog1_RegisterDropDownWidgetKeyRemap
* Description	: 注册DropDown控件键值映射回调函数
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void Dialog1_RegisterDropDownWidgetKeyRemap(void)
{	
	Register_KeyMsgRemap(GetKeyMsgRemapHandle(), GUI_KEY_UP,\
		SendKeyBackTabMsg_CallBack);
	Register_KeyMsgRemap(GetKeyMsgRemapHandle(), GUI_KEY_DOWN,\
		SendKeyTabMsg_CallBack);
	Register_KeyMsgRemap(GetKeyMsgRemapHandle(), GUI_KEY_LEFT,\
		SendKeyBackTabMsg_CallBack);
	Register_KeyMsgRemap(GetKeyMsgRemapHandle(), GUI_KEY_RIGHT,\
		SendKeyTabMsg_CallBack);
	Register_KeyMsgRemap(GetKeyMsgRemapHandle(), GUI_KEY_ENTER,\
		SendKeySpaceMsg_CallBack);
}

/*
*********************************************************************************************************
* Function Name : Dialog1_RegisterButtonWidgetKeyRemap
* Description	: 注册Button控件键值映射回调函数
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void Dialog1_RegisterButtonWidgetKeyRemap(void)
{	
	Register_KeyMsgRemap(GetKeyMsgRemapHandle(), GUI_KEY_UP,\
		SendKeyBackTabMsg_CallBack);
	Register_KeyMsgRemap(GetKeyMsgRemapHandle(), GUI_KEY_DOWN,\
		SendKeyTabMsg_CallBack);
	Register_KeyMsgRemap(GetKeyMsgRemapHandle(), GUI_KEY_LEFT,\
		SendKeyBackTabMsg_CallBack);
	Register_KeyMsgRemap(GetKeyMsgRemapHandle(), GUI_KEY_RIGHT,\
		SendKeyTabMsg_CallBack);
	Register_KeyMsgRemap(GetKeyMsgRemapHandle(), GUI_KEY_ENTER,\
		SendKeyEnterMsg_CallBack);
}

/*
*********************************************************************************************************
* Function Name : _cbDialog1
* Description	: 窗口回调函数
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void _cbDialog1(WM_MESSAGE* pMsg) 
{
	WM_HWIN hWin = pMsg->hWin;
	
	switch (pMsg->MsgId) 
	{
		case WM_INIT_DIALOG:	
			DialogConstructor1(pMsg);
			break;
		case WM_CREATE:				
			break;
		case WM_DELETE:	
			break;
		case WM_PAINT:				
			break;
		case WM_KEY:
		{
			int key = ((WM_KEY_INFO*)(pMsg->Data.p))->Key;
			
			switch (key)
			{
				case GUI_KEY_ESCAPE:	
					WM_SendMessageNoPara(WM_GetParent(hWin), WM_USER_ESC);
					break;			
			}
			break;
		}
		case WM_NOTIFY_PARENT:
		{	
			int NCode = pMsg->Data.v;
			int Id = WM_GetId(pMsg->hWinSrc);
			
			switch (NCode) 
			{
				case WM_NOTIFICATION_GOT_FOCUS:
					switch (Id)
					{
						case GUI_ID_DROPDOWN0:
						case GUI_ID_DROPDOWN1:
							Dialog1_RegisterDropDownWidgetKeyRemap();
							break;
						case GUI_ID_BUTTON0:
						case GUI_ID_BUTTON1:	
							Dialog1_RegisterButtonWidgetKeyRemap();
							break;
						default:
							Dialog1_RegisterDefaultKeyRemap();
							break;
					}
					break;
				case WM_NOTIFICATION_CLICKED:
				{
					switch (Id)
					{
						case GUI_ID_DROPDOWN0:
							
							break;
						case GUI_ID_DROPDOWN1:
							
							break;
						case GUI_ID_BUTTON0:
							
							break;
						case GUI_ID_BUTTON1:	
							
							break;
						default:
							
							break;
					}
					break;
				}
			}
			break;
		}	
		default:
			WM_DefaultProc(pMsg);
			break;
	}
	WM_DefaultProc(pMsg);
}

/*
*********************************************************************************************************
* Function Name : _cbDialog2
* Description	: 窗口回调函数
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void _cbDialog2(WM_MESSAGE* pMsg) 
{
	WM_DefaultProc(pMsg);
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
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
