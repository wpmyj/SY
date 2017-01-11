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
#define ID_ICONVIEW0						(GUI_ID_USER + 0)
#define ID_ICONVIEW1						(GUI_ID_USER + 1)
#define ID_ICONVIEW2						(GUI_ID_USER + 2)
#define ID_ICONVIEW3						(GUI_ID_USER + 3)

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
extern GUI_CONST_STORAGE GUI_BITMAP bmPIC_SystemSet;
extern GUI_CONST_STORAGE GUI_BITMAP bmPIC_TimeDate;
extern GUI_CONST_STORAGE GUI_BITMAP bmPIC_LanguageSelect;
extern GUI_CONST_STORAGE GUI_BITMAP bmPIC_DeviceManager;

static const BITMAP_ITEM _aLang[][SUPPORT_LANGUAGE_NUMS] = {
	{
		{&bmPIC_SystemSet, 			"ϵͳ��������"},
		{&bmPIC_SystemSet, 			"System Parameter Set"},
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
* Description	: ��ȡ��Դ
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
* Description	: ���캯��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void Constructor(WM_MESSAGE *pMsg) 
{
	WM_HWIN hWin = pMsg->hWin;

	/* ����ICONVIEW����ʾλ�� ********************************************************************/
	uint16_t ICONVIEW_VNum = (CONST_FRAME_WIDTH - ICONVIEW_TBorder - ICONVIEW_BBorder) / ICONVIEW_Width;
	uint16_t ICONVIEW_HNum = (CONST_FRAME_HEIGHT - ICONVIEW_LBorder - ICONVIEW_RBorder) / ICONVIEW_Height;
	
	/*��ָ��λ�ô���ָ���ߴ��ICONVIEW С����*/
	ICONVIEW_Handle hIcon;
	hIcon = ICONVIEW_CreateEx(ICONVIEW_TBorder, 				/* С���ߵ��������أ��ڸ������У�*/
							 ICONVIEW_LBorder, 					/* С���ߵ��������أ��ڸ������У�*/
							 ICONVIEW_VNum * ICONVIEW_Width + 30,  /* С���ߵ�ˮƽ�ߴ磨��λ�����أ�*/
							 ICONVIEW_HNum * ICONVIEW_Height + 30, /* С���ߵĴ�ֱ�ߴ磨��λ�����أ�*/
							 hWin, 				        		/* �����ڵľ�������Ϊ0 ������С���߽���Ϊ���棨�������ڣ����Ӵ��� */
							 WM_CF_SHOW | WM_CF_HASTRANS,       /* ���ڴ�����ǡ�ΪʹС���������ɼ���ͨ��ʹ�� WM_CF_SHOW */ 
							 0,								 	/* Ĭ����0���������������������ֱ������ */
							 GUI_ID_ICONVIEW0, 			        /* С���ߵĴ���ID */
							 ICONVIEW_Width, 				    /* ͼ���ˮƽ�ߴ� */
							 ICONVIEW_Height);					/* ͼ��Ĵ�ֱ�ߴ� */

	/* ��ICONVIEW С���������ͼ�� */
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
* Function Name : DeleteWindow
* Description	: ɾ������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void DeleteWindow(WM_MESSAGE *pMsg) 
{		
	g_CursorIndex = ICONVIEW_GetSel(pMsg->hWinSrc);	
	WM_DeleteWindow(pMsg->hWin);
	
	ECHO(DEBUG_APP_WINDOWS, "[APP] ɾ�� <�˵�> ����");
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
* Description	: �����˵�����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void App_MenuTaskCreate(void)
{
	ECHO(DEBUG_APP_WINDOWS, "[emWin] ʣ���ڴ� %ld Byte", GUI_ALLOC_GetNumFreeBytes());
	
	_CreateFrame(_cbCallback);
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
