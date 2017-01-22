/*
*********************************************************************************************************
* @file    	App_Menu.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-12-1 12:36:43
* @IDE	 	Keil V5.22.0.0
* @Chip    	STM32F407VE
* @brief   	Ӧ�ò�˵�Դ�ļ�
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

#define ICONVIEW_TBorder   					10   /* �ؼ�ICONVIEW���ϱ߾� */
#define ICONVIEW_LBorder   					10   /* �ؼ�ICONVIEW����߾� */
#define ICONVIEW_RBorder   					10   /* �ؼ�ICONVIEW���ұ߾� */
#define ICONVIEW_BBorder   					10   /* �ؼ�ICONVIEW���±߾� */   
#define ICONVIEW_Width     					128  /* �ؼ�ICONVIEW�Ŀ� */  
#define ICONVIEW_Height    					128  /* �ؼ�ICONVIEW�ĸߣ�����ͼ���Y������ */  
#define ICONVIEW_YSpace    					10   /* �ؼ�ICONVIEW��Y������ */  
#define ICONVIEW_ImagNum   					10   /* �ؼ�ICONVIEW�е�ͼ����� */

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
		"ϵͳ�˵�",
		"System Menu",
	},	//1
};

extern GUI_CONST_STORAGE GUI_BITMAP bmPIC_SystemSet;
extern GUI_CONST_STORAGE GUI_BITMAP bmPIC_TimeDate;
extern GUI_CONST_STORAGE GUI_BITMAP bmPIC_LanguageSelect;
extern GUI_CONST_STORAGE GUI_BITMAP bmPIC_DeviceManager;

static const BITMAP_ITEM _aLangBitmap[][SUPPORT_LANGUAGE_NUMS] = {
	{
		{&bmPIC_SystemSet, 			"��������"},
		{&bmPIC_SystemSet, 			"Parameter Set"},
	},	//1
	{
		{&bmPIC_TimeDate, 			"ʱ����������"},
		{&bmPIC_TimeDate, 			"Time Data Set"},
	},	//2
	{
		{&bmPIC_LanguageSelect, 	"����ѡ��"},
		{&bmPIC_LanguageSelect, 	"Language Select"},
	},	//3
	{
		{&bmPIC_DeviceManager,		"�豸������"},
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
* Description	: ��ȡ�ı���Դ
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
* Description	: ��ȡBitmap��Դ
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
* Description	: ���캯��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void Constructor(WM_MESSAGE *pMsg) 
{
	WM_HWIN hWin = pMsg->hWin;
	
	/* ���ñ����� */
	FRAMEWIN_SetFont(hWin, FRAME_FONT);
	FRAMEWIN_SetTitleHeight(hWin, DIALOG_TITLE_HEIGHT);
	FRAMEWIN_SetTextAlign(hWin, GUI_TA_HCENTER | GUI_TA_VCENTER);
	FRAMEWIN_SetText(hWin, _GetLang(1));
			
	/* ��ICONVIEW С���������ͼ�� */
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
	
	ECHO(DEBUG_APP_WINDOWS, "[APP] ���� <�˵�> ����");
}

/*
*********************************************************************************************************
* Function Name : Destructor
* Description	: ��������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void Destructor(WM_MESSAGE *pMsg) 
{		
	ECHO(DEBUG_APP_WINDOWS, "[APP] ���� <�˵�> ����");
}

/*
*********************************************************************************************************
* Function Name : Delete
* Description	: ɾ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void Delete(WM_MESSAGE *pMsg) 
{		
	g_CursorIndex = ICONVIEW_GetSel(pMsg->hWinSrc);	
	
	GUI_EndDialog(pMsg->hWin, 1);
	
	ECHO(DEBUG_APP_WINDOWS, "[APP] ɾ�� <�˵�> ����");
}

/*
*********************************************************************************************************
* Function Name : Paint
* Description	: ����
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
* Description	: ���ڻص�����
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
* Description	: �����˵�����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void App_MenuTaskCreate(void)
{
	ECHO(DEBUG_APP_WINDOWS, "[emWin] ʣ���ڴ� %ld Byte", GUI_ALLOC_GetNumFreeBytes());
	
	GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, hSuperWindows, 0, 0);
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
