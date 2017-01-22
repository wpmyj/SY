/*
*********************************************************************************************************
* @file    	App_Language.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-12-1 15:04:54
* @IDE	 	Keil V5.22.0.0
* @Chip    	STM32F407VE
* @brief   	����ѡ��Դ�ļ�
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
#define GUI_DIALOG_WIDTH					270
#define GUI_DIALOG_HEIGHT					150
#define GUI_DIALOG_START_X					(30)
#define GUI_DIALOG_START_Y					(30)


#define GUI_ID_DIALOG0						(GUI_ID_USER + 0x00)



/*
*********************************************************************************************************
*                              				Private typedef
*********************************************************************************************************
*/
enum LANGUAGE_INDEX {
	LANGUAGE_CH_INDEX = 0,
	LANGUAGE_EN_INDEX,
};

/*
*********************************************************************************************************
*                              				Private constants
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
	{ FRAMEWIN_CreateIndirect, "Framewin", GUI_ID_DIALOG0,
		GUI_DIALOG_START_X, GUI_DIALOG_START_Y, GUI_DIALOG_WIDTH, GUI_DIALOG_HEIGHT, 
		0, 0, 0 },
	
	{ RADIO_CreateIndirect, "Radio", GUI_ID_RADIO0,
		10, 20, 0, 0, 
		0, 0x2402, 0 },
	
	{ TEXT_CreateIndirect, "Text", GUI_ID_TEXT0,
		30, 15, 150, 24, 
		0x0, 0, 0 },
	
	{ TEXT_CreateIndirect, "Text", GUI_ID_TEXT1,  
		30, (15+35), 150, 24,
		0x0, 0, 0 },
};

static const char * _aLang[][SUPPORT_LANGUAGE_NUMS] = {
	{
		"����ѡ��",
		"Language Select",
	},	//1
	{
		"��������",
		"��������",
	},	//2
	{
		"English",
		"English",
	},	//3
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
* Description	: ��ȡ�ı�
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
* Description	: ���캯��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void Constructor(WM_MESSAGE *pMsg) 
{
	WM_HWIN hWin = pMsg->hWin;
	
	FRAMEWIN_SetFont(hWin, FRAME_FONT);
	FRAMEWIN_SetTitleHeight(hWin, DIALOG_TITLE_HEIGHT);
	FRAMEWIN_SetTextAlign(hWin, GUI_TA_HCENTER | GUI_TA_VCENTER);
	FRAMEWIN_SetText(hWin, _GetLang(1));
	
	WM_HWIN hChild = WM_GetDialogItem(hWin, GUI_ID_TEXT0);	
	TEXT_SetTextAlign(hChild, TEXT_CF_LEFT);
	TEXT_SetFont(hChild, FRAME_TEXT_FONT);
	TEXT_SetText(hChild, _GetLang(2));
	
	hChild = WM_GetDialogItem(hWin, GUI_ID_TEXT1);	
	TEXT_SetTextAlign(hChild, TEXT_CF_LEFT);
	TEXT_SetFont(hChild, FRAME_TEXT_FONT);	
	TEXT_SetText(hChild, _GetLang(3));
	
	ECHO(DEBUG_APP_WINDOWS, "[APP] ���� <����ѡ��> ����");
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
static void Destructor( WM_MESSAGE *pMsg )
{	
	ECHO(DEBUG_APP_WINDOWS, "[APP] ���� <����ѡ��> ����");
}

/*
*********************************************************************************************************
* Function Name : Delete
* Description	: ɾ������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void Delete( WM_MESSAGE *pMsg )
{
	GUI_EndDialog(pMsg->hWin, 1);
	
	ECHO(DEBUG_APP_WINDOWS, "[APP] ɾ�� <����ѡ��> ����");
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
* Function Name : _cbDialog
* Description	: �Ի��򴰿ڻص�����
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
		case WM_TIMER:
		{
			if (GetLanguageType() == LANG_ENGLISH)
			{
				GUI_SendKeyMsg(GUI_KEY_DOWN, true);
			}
			break;
		}
		case WM_KEY:
		{
			int key = ((WM_KEY_INFO*)(pMsg->Data.p))->Key;
			
			switch (key)
			{
				case GUI_KEY_UP:
				{
					RADIO_Dec(hWin);
					break;				
				}
				case GUI_KEY_DOWN:	
				{	
					RADIO_Inc(hWin);
					break;				
				}
				case GUI_KEY_ENTER:
				{
					int value = RADIO_GetValue(hWin);
					switch (value)
					{
						case LANGUAGE_CH_INDEX:
							SetLanguageType(LANG_CHINESE);
							break;
						case LANGUAGE_EN_INDEX:
							SetLanguageType(LANG_ENGLISH);							
							break;
						default:
							return;
					}					
					WM_SendMessageNoPara(WM_GetParent(hWin), WM_USER_ESC);					
					break;
				}
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
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}

/*
*********************************************************************************************************
* Function Name : App_LanguageTaskCreate
* Description	: ��������Ӧ�ó�������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void App_LanguageTaskCreate(void)
{
	WM_HWIN hDialog = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbDialog, hSuperWindows, 0, 0);
	
	WM_CreateTimer(WM_GetClientWindow(hDialog),  /* ������Ϣ�Ĵ��ڵľ�� */
                   0, 	                        /* �û������Id���������ͬһ����ʹ�ö����ʱ������ֵ��������Ϊ�㡣 */
			       100,                         /* ���ڣ������ڹ���ָ������Ӧ�յ���Ϣ*/
			       0);	                        /* ��������ʹ�ã�ӦΪ0 */ 
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/

