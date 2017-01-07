/*
*********************************************************************************************************
* @file    	App_SystemParameter.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-12-2 15:45:07
* @IDE	 	Keil V5.22.0.0
* @Chip    	STM32F407VE
* @brief   	ϵͳ����ҳ��Դ�ļ�
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
#define START_WIDGET_X						10
#define START_WIDGET_Y						10
#define WIDGET_ROWS_DISTANCE				24
#define WIDGET_COLUMNS_DISTANCE				(0)

#define TEXT_LENTH							250	
#define TEXT_WIDTH							32
#define EDIT_LENTH							250	
#define EDIT_WIDTH							32
#define EDIT_BOTTOM_BUTTON_LENTH_DISTANCE	3
#define EDIT_BOTTOM_BUTTON_WIDTH_DISTANCE	3
#define EDIT_BOTTOM_BUTTON_LENTH			(EDIT_LENTH+2*EDIT_BOTTOM_BUTTON_LENTH_DISTANCE)
#define EDIT_BOTTOM_BUTTON_WIDTH			(EDIT_WIDTH+2*EDIT_BOTTOM_BUTTON_WIDTH_DISTANCE)


/*
*********************************************************************************************************
*                              				Private typedef
*********************************************************************************************************
*/
typedef struct {
	LIST_HANDLE_TypeDef curWidgetHandle;		
	char stringBuff[MAX_STRING_NUM+1];
	struct list_head handleHead;
}APP_SYSTEM_PARAMETER_TypeDef;


/*
*********************************************************************************************************
*                              				Private constants
*********************************************************************************************************
*/
static const char * _aLang[][SUPPORT_LANGUAGE_NUMS] = {
	{
		"ϵͳ�������",
		"MaxForceOfSystem:",
	},													//1
	{
		"����������",
		"MaxControlDegree:",
	},													//2
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
static APP_SYSTEM_PARAMETER_TypeDef *this;

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
static void Constructor(WM_MESSAGE* pMsg) 
{
	uint16_t x = START_WIDGET_X;
	uint16_t y = START_WIDGET_Y;
	WM_HWIN hWin = pMsg->hWin;
	
	this = new(sizeof(APP_SYSTEM_PARAMETER_TypeDef));

	INIT_LIST_HEAD(&this->handleHead);
	
	/*
		�༭��۽���ֱ�ӽ�������״̬��
		��˲��ܵ�����Ϊ�ؼ�����ʾ��ǰ����λ�á�
		��Ҫ���ı��ؼ�������Ҫ���ð�ť��
	 */
	{
		LIST_HANDLE_TypeDef *listHandle = new(sizeof(LIST_HANDLE_TypeDef));
		
		listHandle->handleLevel1 = _CreateButton(hWin,
												FRAME_BUTTON_EFFECT,
												"", 
												GUI_ID_BUTTON0, 
												x+TEXT_LENTH, 
												y, 
												EDIT_BOTTOM_BUTTON_LENTH,  
												EDIT_BOTTOM_BUTTON_WIDTH);
		listHandle->handleLevel2 = _CreateEdit(hWin, 
												FRAME_EDIT_EFFECT,
												"", 
												GUI_ID_EDIT0, 
												TEXT_CF_LEFT | TEXT_CF_VCENTER,
												MAX_TEXT_PUTIN_LENTH,
												x+TEXT_LENTH+EDIT_BOTTOM_BUTTON_LENTH_DISTANCE, 
												y+EDIT_BOTTOM_BUTTON_WIDTH_DISTANCE, 
												EDIT_LENTH,  
												EDIT_WIDTH);
		listHandle->handleLevel3 = _CreateText(hWin,
												_GetLang(1),
												GUI_ID_TEXT0,
												TEXT_CF_RIGHT | TEXT_CF_VCENTER,
												x, 
												y+EDIT_BOTTOM_BUTTON_WIDTH_DISTANCE, 
												TEXT_LENTH,  
												TEXT_WIDTH);
		listHandle->cursorHandle = listHandle->handleLevel1;
		list_add_tail(&listHandle->list, &this->handleHead);
	}
	y += WIDGET_ROWS_DISTANCE + EDIT_BOTTOM_BUTTON_WIDTH;
	{
		LIST_HANDLE_TypeDef *listHandle = new(sizeof(LIST_HANDLE_TypeDef));
		
		listHandle->handleLevel1 = _CreateButton(hWin, 
												FRAME_BUTTON_EFFECT,
												"", 
												GUI_ID_BUTTON1, 
												x+TEXT_LENTH, 
												y, 
												EDIT_BOTTOM_BUTTON_LENTH,  
												EDIT_BOTTOM_BUTTON_WIDTH);
		listHandle->handleLevel2 = _CreateEdit(hWin,
												FRAME_EDIT_EFFECT,
												"", 
												GUI_ID_EDIT1, 
												TEXT_CF_LEFT | TEXT_CF_VCENTER,
												MAX_TEXT_PUTIN_LENTH,
												x+TEXT_LENTH+EDIT_BOTTOM_BUTTON_LENTH_DISTANCE, 
												y+EDIT_BOTTOM_BUTTON_WIDTH_DISTANCE, 
												EDIT_LENTH,  
												EDIT_WIDTH);	
		listHandle->handleLevel3 = _CreateText(hWin,
												_GetLang(2),
												GUI_ID_TEXT1,
												TEXT_CF_RIGHT | TEXT_CF_VCENTER,
												x, 
												y+EDIT_BOTTOM_BUTTON_WIDTH_DISTANCE, 
												TEXT_LENTH,  
												TEXT_WIDTH);
		listHandle->cursorHandle = listHandle->handleLevel1;
		list_add_tail(&listHandle->list, &this->handleHead);
	}
	

	WM_SendMessageNoPara(hWin, WM_SET_FOCUS);
	
	ECHO(DEBUG_APP_WINDOWS, "[APP] ���� <ϵͳ��������> ����");
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
static void Destructor(WM_MESSAGE* pMsg) 
{
	WM_DeleteWindow(pMsg->hWin);
	
	LIST_HANDLE_TypeDef *handle;
	list_for_each_entry(handle, &this->handleHead, LIST_HANDLE_TypeDef, list)
	{
		delete(handle);
	}
	
	delete(this);
	
	ECHO(DEBUG_APP_WINDOWS, "[APP] ���� <ϵͳ��������> ����");
}

/*
*********************************************************************************************************
* Function Name : GetFocusListHandle
* Description	: ��ȡ�Ѿ��۽���������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static LIST_HANDLE_TypeDef *GetFocusListHandle(void)
{
	LIST_HANDLE_TypeDef *handle;
	
	list_for_each_entry(handle, &this->handleHead, LIST_HANDLE_TypeDef, list)
	{
		if (WM_HasFocus(handle->cursorHandle) == true)
		{
			return handle;
		}
	}
	
	return NULL;
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
	
	switch (pMsg->MsgId) 
	{
		case WM_CREATE:	
			Constructor(pMsg);
			break;
		case WM_PAINT:		
			_PaintFrame();			
			break;
		case WM_KEY:
		{
			int key = ((WM_KEY_INFO*)(pMsg->Data.p))->Key;			
			switch (key)
			{
				case GUI_KEY_UP:
				{
					LIST_HANDLE_TypeDef *handle = GetFocusListHandle();
					if (handle)
					{
						if (handle == list_first_entry(&this->handleHead, LIST_HANDLE_TypeDef, list))
						{
							handle = list_last_entry(&this->handleHead, LIST_HANDLE_TypeDef, list);
						}	
						else					
						{
							handle = list_prev_entry(handle, LIST_HANDLE_TypeDef, list);						
						}
						if (handle)
						{
							WM_SetFocus(handle->cursorHandle);
						}
					}
					break;				
				}
				case GUI_KEY_DOWN:	
				{
					LIST_HANDLE_TypeDef *handle = GetFocusListHandle();
					if (handle)
					{
						if (handle == list_last_entry(&this->handleHead, LIST_HANDLE_TypeDef, list))
						{
							handle = list_first_entry(&this->handleHead, LIST_HANDLE_TypeDef, list);
						}	
						else					
						{
							handle = list_next_entry(handle, LIST_HANDLE_TypeDef, list);						
						}
						if (handle)
						{
							WM_SetFocus(handle->cursorHandle);
						}
					}					
					break;				
				}
				case GUI_KEY_ESCAPE:
					Destructor(pMsg);
					App_MenuTaskCreate();
					break;
				default:
					
					break;
			}
			break;
		}
		case WM_NOTIFY_PARENT:
		{
			int Id    = WM_GetId(pMsg->hWinSrc);      
			int NCode = pMsg->Data.v;  
			
			if (NCode == WM_NOTIFICATION_RELEASED)
			{
				/* ��¼��ǰ������ڿؼ���Ϣ */
				{
					this->curWidgetHandle = *GetFocusListHandle();
					EDIT_GetText(this->curWidgetHandle.handleLevel2, this->stringBuff, MAX_STRING_NUM);
				}
				
				switch (Id)
				{
					case GUI_ID_BUTTON0:
						_CreateNumPad(hWin, CONST_FRAME_WIDTH >> 1, 150);	
						break;
					case GUI_ID_BUTTON1:
						_CreateNumPad(hWin, CONST_FRAME_WIDTH >> 1, 200);
						break;
				}
			}			
			break;
		}
		case MSG_USER_PAD_CHANGED:
		case MSG_USER_PAD_DELETE:
		case MSG_USER_PAD_CLEAR:
		{		
			EDIT_SetText(this->curWidgetHandle.handleLevel2, pMsg->Data.p);
			break;
		}		
		case MSG_USER_PAD_CANCEL:	
		{		
			EDIT_SetText(this->curWidgetHandle.handleLevel2, this->stringBuff);
			WM_SetFocus(this->curWidgetHandle.handleLevel1);
			break;
		}
		case MSG_USER_PAD_OK:	
		{
			WM_SetFocus(this->curWidgetHandle.handleLevel1);
			break;
		}
		case WM_SET_FOCUS:
		{
			LIST_HANDLE_TypeDef *handle = NULL;
			handle = list_first_entry(&this->handleHead, LIST_HANDLE_TypeDef, list);
			WM_SetFocus(handle->cursorHandle);
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
* Description	: ����ϵͳ��������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void App_SystemParameterTaskCreate( void )
{
	_CreateFrame(_cbCallback);
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
