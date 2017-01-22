/*
*********************************************************************************************************
* @file    	App_Menu.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-12-1 12:36:43
* @IDE	 	Keil V5.22.0.0
* @Chip    	STM32F407VE
* @brief   	应用层菜单源文件
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
#include "App_SystemParameter.h"
#include "App_Language.h"
#include "App_TimeDate.h"
#include "App_DeviceManager.h"

/*
*********************************************************************************************************
*                              				Private define
*********************************************************************************************************
*/
#define GUI_ID_DIALOG0     					(GUI_ID_USER + 0)
#define ID_ICONVIEW0						(GUI_ID_USER + 10)
#define ID_ICONVIEW1						(GUI_ID_USER + 11)
#define ID_ICONVIEW2						(GUI_ID_USER + 12)
#define ID_ICONVIEW3						(GUI_ID_USER + 13)

#define ICONVIEW_TBorder   					10   /* 控件ICONVIEW的上边距 */
#define ICONVIEW_LBorder   					10   /* 控件ICONVIEW的左边距 */
#define ICONVIEW_RBorder   					10   /* 控件ICONVIEW的右边距 */
#define ICONVIEW_BBorder   					10   /* 控件ICONVIEW的下边距 */   
#define ICONVIEW_Width     					128  /* 控件ICONVIEW的宽 */  
#define ICONVIEW_Height    					128  /* 控件ICONVIEW的高，包括图标的Y方向间距 */  
#define ICONVIEW_YSpace    					10   /* 控件ICONVIEW的Y方向间距 */  
#define ICONVIEW_ImagNum   					10   /* 控件ICONVIEW中的图标个数 */

/*
*********************************************************************************************************
*                              				Private typedef
*********************************************************************************************************
*/
typedef struct {
	const GUI_BITMAP * pBitmap;
	const char       * pText;
}BITMAP_ITEM;

/*
*********************************************************************************************************
*                              				Private constants
*********************************************************************************************************
*/
static const char * _aLang[][SUPPORT_LANGUAGE_NUMS] = {
	{
		"系统菜单",
		"System Menu",
	},	//1
};

extern GUI_CONST_STORAGE GUI_BITMAP bmPIC_SystemSet;
extern GUI_CONST_STORAGE GUI_BITMAP bmPIC_TimeDate;
extern GUI_CONST_STORAGE GUI_BITMAP bmPIC_LanguageSelect;
extern GUI_CONST_STORAGE GUI_BITMAP bmPIC_DeviceManager;

static const BITMAP_ITEM _aLangBitmap[][SUPPORT_LANGUAGE_NUMS] = {
	{
		{&bmPIC_SystemSet, 			"参数设置"},
		{&bmPIC_SystemSet, 			"Parameter Set"},
	},	//1
	{
		{&bmPIC_TimeDate, 			"时间日期设置"},
		{&bmPIC_TimeDate, 			"Time Data Set"},
	},	//2
	{
		{&bmPIC_LanguageSelect, 	"语言选择"},
		{&bmPIC_LanguageSelect, 	"Language Select"},
	},	//3
	{
		{&bmPIC_DeviceManager,		"设备管理器"},
		{&bmPIC_DeviceManager,		"Device Manager"},
	},	//4
};

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
	{ FRAMEWIN_CreateIndirect, "Framewin", GUI_ID_DIALOG0,
		25, 25, CONST_FRAME_WIDTH-50, CONST_FRAME_HEIGHT-50, 
		0, 0x0, 0 },
	{ ICONVIEW_CreateIndirect, "Iconview", GUI_ID_ICONVIEW0,
		ICONVIEW_TBorder, ICONVIEW_LBorder, CONST_FRAME_WIDTH-75, CONST_FRAME_HEIGHT-75,
		WM_CF_SHOW | WM_CF_HASTRANS, ((ICONVIEW_Width << 16) | ICONVIEW_Height), 0 },
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
static int g_CursorIndex;

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
* Description	: 获取文本资源
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
* Function Name : _GetLangBitmap
* Description	: 获取Bitmap资源
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static const BITMAP_ITEM *_GetLangBitmap(uint32_t Index) 
{
	const BITMAP_ITEM *p = NULL;
	uint8_t languageType = GetLanguageType();
	
	Index--;
	if ((Index < GUI_COUNTOF(_aLangBitmap)) && (languageType < GUI_COUNTOF(_aLangBitmap[0]))) 
	{
		p = &_aLangBitmap[Index][languageType];
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
	
	/* 设置标题栏 */
	FRAMEWIN_SetFont(hWin, FRAME_FONT);
	FRAMEWIN_SetTitleHeight(hWin, DIALOG_TITLE_HEIGHT);
	FRAMEWIN_SetTextAlign(hWin, GUI_TA_HCENTER | GUI_TA_VCENTER);
	FRAMEWIN_SetText(hWin, _GetLang(1));
			
	/* 向ICONVIEW 小工具添加新图标 */
	ICONVIEW_Handle hIcon = WM_GetDialogItem(hWin, GUI_ID_ICONVIEW0);		
	for (uint8_t i = 1; i <= GUI_COUNTOF(_aLangBitmap); i++) 
	{		
		const BITMAP_ITEM *hBitmap = _GetLangBitmap(i);
		ICONVIEW_AddBitmapItem(hIcon, hBitmap->pBitmap, hBitmap->pText);
	}
	ICONVIEW_SetBkColor(hIcon, ICONVIEW_CI_SEL, GUI_BLUE | 0xC0000000);
	ICONVIEW_SetFont(hIcon, FRAME_ICON_FONT);
	ICONVIEW_SetTextColor(hIcon, ICONVIEW_CI_UNSEL, GUI_BLACK);
	ICONVIEW_SetTextColor(hIcon, ICONVIEW_CI_SEL, GUI_WHITE);
	ICONVIEW_SetIconAlign(hIcon, ICONVIEW_IA_HCENTER|ICONVIEW_IA_TOP);
	
	WM_SetFocus(WM_GetFirstChild(hWin));
	if (g_CursorIndex)
	{		
		ICONVIEW_SetSel(hIcon, g_CursorIndex);
	}
	
	ECHO(DEBUG_APP_WINDOWS, "[APP] 构造 <菜单> 窗口");
}

/*
*********************************************************************************************************
* Function Name : Destructor
* Description	: 析构函数
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void Destructor(WM_MESSAGE *pMsg) 
{		
	ECHO(DEBUG_APP_WINDOWS, "[APP] 析构 <菜单> 窗口");
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
static void Delete(WM_MESSAGE *pMsg) 
{		
	g_CursorIndex = ICONVIEW_GetSel(pMsg->hWinSrc);	
	
	GUI_EndDialog(pMsg->hWin, 1);
	
	ECHO(DEBUG_APP_WINDOWS, "[APP] 删除 <菜单> 窗口");
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
* Function Name : _cbCallback
* Description	: 窗口回调函数
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void _cbCallback(WM_MESSAGE* pMsg) 
{
	WM_HWIN hWin = pMsg->hWin;
	(void)hWin;
	
	switch (pMsg->MsgId) 
	{
		case WM_INIT_DIALOG:
			Constructor(pMsg);
			break;
		case WM_CREATE:			
			break;
		case WM_PAINT:		
			Paint();			
			break;
		case WM_DELETE:
			Destructor(pMsg);
			break;
		case WM_KEY:
		{
			int key = ((WM_KEY_INFO*)(pMsg->Data.p))->Key;			
			switch (key)
			{
				case GUI_KEY_ENTER:
				{					
					switch (ICONVIEW_GetSel(pMsg->hWinSrc))
					{
						case 0:
							Delete(pMsg);					
							App_SystemParameterTaskCreate();
							break;
						case 1:
							Delete(pMsg);
							App_TimeDateTaskCreate();									
							break;
						case 2:
							Delete(pMsg);
							App_LanguageTaskCreate();
							break;
						case 3:
							Delete(pMsg);
							App_DeviceManagerTaskCreate();
							break;
					}
					break;				
				}
				case GUI_KEY_ESCAPE:	
				{
										
					break;				
				}
				default:				
				{					
					break;
				}
			}
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
* Function Name : App_MenuTaskCreate
* Description	: 创建菜单任务
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void App_MenuTaskCreate(void)
{
	ECHO(DEBUG_APP_WINDOWS, "[emWin] 剩余内存 %ld Byte", GUI_ALLOC_GetNumFreeBytes());
	
	GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hSuperWindows, 0, 0);
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
