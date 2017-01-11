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
#define ID_ICONVIEW0						(GUI_ID_USER + 0)
#define ID_ICONVIEW1						(GUI_ID_USER + 1)
#define ID_ICONVIEW2						(GUI_ID_USER + 2)
#define ID_ICONVIEW3						(GUI_ID_USER + 3)

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
extern GUI_CONST_STORAGE GUI_BITMAP bmPIC_SystemSet;
extern GUI_CONST_STORAGE GUI_BITMAP bmPIC_TimeDate;
extern GUI_CONST_STORAGE GUI_BITMAP bmPIC_LanguageSelect;
extern GUI_CONST_STORAGE GUI_BITMAP bmPIC_DeviceManager;

static const BITMAP_ITEM _aLang[][SUPPORT_LANGUAGE_NUMS] = {
	{
		{&bmPIC_SystemSet, 			"系统参数设置"},
		{&bmPIC_SystemSet, 			"System Parameter Set"},
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
* Description	: 获取资源
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static const BITMAP_ITEM *_GetLang(uint32_t Index) 
{
	const BITMAP_ITEM *p = NULL;
	uint8_t languageType = GetLanguageType();
	
	Index--;
	if ((Index < GUI_COUNTOF(_aLang)) && (languageType < GUI_COUNTOF(_aLang[0]))) 
	{
		p = &_aLang[Index][languageType];
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

	/* 设置ICONVIEW的显示位置 ********************************************************************/
	uint16_t ICONVIEW_VNum = (CONST_FRAME_WIDTH - ICONVIEW_TBorder - ICONVIEW_BBorder) / ICONVIEW_Width;
	uint16_t ICONVIEW_HNum = (CONST_FRAME_HEIGHT - ICONVIEW_LBorder - ICONVIEW_RBorder) / ICONVIEW_Height;
	
	/*在指定位置创建指定尺寸的ICONVIEW 小工具*/
	ICONVIEW_Handle hIcon;
	hIcon = ICONVIEW_CreateEx(ICONVIEW_TBorder, 				/* 小工具的最左像素（在父坐标中）*/
							 ICONVIEW_LBorder, 					/* 小工具的最上像素（在父坐标中）*/
							 ICONVIEW_VNum * ICONVIEW_Width + 30,  /* 小工具的水平尺寸（单位：像素）*/
							 ICONVIEW_HNum * ICONVIEW_Height + 30, /* 小工具的垂直尺寸（单位：像素）*/
							 hWin, 				        		/* 父窗口的句柄。如果为0 ，则新小工具将成为桌面（顶级窗口）的子窗口 */
							 WM_CF_SHOW | WM_CF_HASTRANS,       /* 窗口创建标记。为使小工具立即可见，通常使用 WM_CF_SHOW */ 
							 0,								 	/* 默认是0，如果不够可设置增减垂直滚动条 */
							 GUI_ID_ICONVIEW0, 			        /* 小工具的窗口ID */
							 ICONVIEW_Width, 				    /* 图标的水平尺寸 */
							 ICONVIEW_Height);					/* 图标的垂直尺寸 */

	/* 向ICONVIEW 小工具添加新图标 */
	for (uint8_t i = 1; i <= GUI_COUNTOF(_aLang); i++) 
	{		
		const BITMAP_ITEM *hBitmap = _GetLang(i);
		ICONVIEW_AddBitmapItem(hIcon, hBitmap->pBitmap, hBitmap->pText);
	}
	ICONVIEW_SetBkColor(hIcon, ICONVIEW_CI_SEL, GUI_LIGHTRED);
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
* Function Name : DeleteWindow
* Description	: 删除窗口
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void DeleteWindow(WM_MESSAGE *pMsg) 
{		
	g_CursorIndex = ICONVIEW_GetSel(pMsg->hWinSrc);	
	WM_DeleteWindow(pMsg->hWin);
	
	ECHO(DEBUG_APP_WINDOWS, "[APP] 删除 <菜单> 窗口");
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
		case WM_CREATE:	
			Constructor(pMsg);
			break;
		case WM_PAINT:		
			_PaintFrame();			
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
							DeleteWindow(pMsg);					
							App_SystemParameterTaskCreate();
							break;
						case 1:
							DeleteWindow(pMsg);
							App_TimeDateTaskCreate();		
							break;
						case 2:
							DeleteWindow(pMsg);
							App_LanguageTaskCreate();
							break;
						case 3:
							DeleteWindow(pMsg);
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
	
	_CreateFrame(_cbCallback);
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
