/*
*********************************************************************************************************
* @file    	App_Utils.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-12-1 11:27:29
* @IDE	 	Keil V5.22.0.0
* @Chip    	STM32F407VE
* @brief   	应用层工具类源文件
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

/*
*********************************************************************************************************
*                              				Private define
*********************************************************************************************************
*/


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
static LANGUAGE_TYPE_TypeDef g_languageType = LANG_CHINESE;
extern WM_HWIN hWindowsSuper;


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
* Function Name : _DrawUpRectEx
* Description	: 画凸类型矩形
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void _DrawUpRectEx(const WIDGET_EFFECT* pEffect, const GUI_RECT* pRect) 
{
	WM_LOCK();
	pEffect->pfDrawUpRect(pRect);
	WM_UNLOCK();
}

/*
*********************************************************************************************************
* Function Name : _DrawUpRect
* Description	: 画凸类型矩形
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void _DrawUpRect(const WIDGET_EFFECT* pEffect, int x0, int y0, int x1, int y1) 
{
	GUI_RECT r;
	r.x0 = x0;
	r.y0 = y0;
	r.x1 = x1;
	r.y1 = y1;
	_DrawUpRectEx(pEffect, &r);
}

/*
*********************************************************************************************************
* Function Name : _DrawDownRect
* Description	: 画凹类型矩形
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void _DrawDownRectEx(const WIDGET_EFFECT* pEffect, const GUI_RECT* pRect) 
{
	WM_LOCK();
	pEffect->pfDrawDownRect(pRect);
	WM_UNLOCK();
}

/*
*********************************************************************************************************
* Function Name : _DrawDownRect
* Description	: 画凹类型矩形
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void _DrawDownRect(const WIDGET_EFFECT* pEffect, int x0, int y0, int x1, int y1) 
{
	GUI_RECT r;
	r.x0 = x0;
	r.y0 = y0;
	r.x1 = x1;
	r.y1 = y1;
	_DrawDownRectEx(pEffect, &r);
}

/*
*********************************************************************************************************
* Function Name : _CreateFrame
* Description	: 创建框架
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
WM_HWIN _CreateFrame(WM_CALLBACK* cb) 
{
	int x;
	int y;
	x = FRAME_BORDER + MAIN_BORDER;
	y = FRAME_BORDER + MAIN_TITLE_HEIGHT;
	
	WM_HWIN hFrame = WM_CreateWindowAsChild(x, y, FRAME_WIDTH, FRAME_HEIGHT, \
				hWindowsSuper, WM_CF_SHOW, cb, 0);
	
	return hFrame;
}

/*
*********************************************************************************************************
* Function Name : _PaintFrame
* Description	: 绘制框架
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void _PaintFrame(void)
{
	GUI_RECT r;
	WM_GetClientRect(&r);
	
	GUI_SetBkColor(FRAME_BKCOLOR);
	GUI_SetColor(FRAME_TEXTCOLOR);
	GUI_SetFont(FRAME_FONT);
	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_ClearRectEx(&r);
}

/*
*********************************************************************************************************
* Function Name : GetLanguageType
* Description	: 获取语言类型
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
LANGUAGE_TYPE_TypeDef GetLanguageType( void )
{
	return g_languageType;
}

/*
*********************************************************************************************************
* Function Name : SetLanguageType
* Description	: 设置语言类型
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void SetLanguageType( LANGUAGE_TYPE_TypeDef type)
{
	g_languageType = type;
}

/*
*********************************************************************************************************
* Function Name : _CreateButton
* Description	: 创建按钮控件
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
WM_HWIN _CreateButton(WM_HWIN hParent, 
						const WIDGET_EFFECT* pEffect,
						const char* pText, 
						int Id,						
						int x, 
						int y, 
						int w, 
						int h)		
{
	WM_HWIN hButton;
	hButton = BUTTON_CreateEx(x, y, w, h, hParent, WM_CF_SHOW, 0, Id);
	BUTTON_SetText      (hButton,   pText);
	BUTTON_SetFont      (hButton,   FRAME_BUTTON_FONT);
	BUTTON_SetBkColor   (hButton, 	BUTTON_CI_UNPRESSED, 	FRAME_BUTTON_BKCOLOR0);
	BUTTON_SetBkColor   (hButton, 	BUTTON_CI_PRESSED, 		FRAME_BUTTON_BKCOLOR1);
	BUTTON_SetBkColor   (hButton, 	BUTTON_CI_DISABLED, 	FRAME_BUTTON_BKCOLOR2);
	BUTTON_SetTextColor (hButton, 	BUTTON_CI_UNPRESSED, 	FRAME_BUTTON_COLOR0);
	BUTTON_SetTextColor (hButton, 	BUTTON_CI_PRESSED, 		FRAME_BUTTON_COLOR1);
	BUTTON_SetTextColor (hButton, 	BUTTON_CI_DISABLED, 	FRAME_BUTTON_COLOR0);
	WIDGET_SetEffect    (hButton,   pEffect);
	BUTTON_SetFocussable(hButton,   true);
	
	return hButton;
}

/*
*********************************************************************************************************
* Function Name : _CreateEdit
* Description	: 创建编辑窗控件
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
WM_HWIN _CreateEdit(WM_HWIN hParent, 
					const WIDGET_EFFECT* pEffect,
					const char* pText, 
					int Id,
					int ExFlags, 
					int MaxLen,
					int x, 
					int y, 
					int w, 
					int h)
{
	WM_HWIN hEdit;
	hEdit = EDIT_CreateEx(x, y, w, h, hParent, WM_CF_SHOW, ExFlags, Id, MaxLen);
	EDIT_SetText      (hEdit,   pText);
	EDIT_SetFont      (hEdit,   FRAME_EDIT_FONT);
	EDIT_SetBkColor   (hEdit, 	EDIT_CI_DISABLED, 	FRAME_EDIT_BKCOLOR0);
	EDIT_SetBkColor   (hEdit, 	EDIT_CI_ENABLED, 	FRAME_EDIT_BKCOLOR1);
	EDIT_SetBkColor   (hEdit, 	EDIT_CI_CURSOR, 	FRAME_EDIT_BKCOLOR2);
	EDIT_SetTextColor (hEdit, 	EDIT_CI_DISABLED, 	FRAME_EDIT_COLOR0);
	EDIT_SetTextColor (hEdit,	EDIT_CI_ENABLED, 	FRAME_EDIT_COLOR1);
	EDIT_SetTextColor (hEdit, 	EDIT_CI_CURSOR, 	FRAME_EDIT_COLOR2);
	WIDGET_SetEffect  (hEdit,   pEffect);
	EDIT_SetFocussable(hEdit,   true);
	
	return hEdit;
}

/*
*********************************************************************************************************
* Function Name : _CreateText
* Description	: 创建文本控件
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
WM_HWIN _CreateText(WM_HWIN hParent, 
					const char* pText, 
					int Id,
					int ExFlags, 
					int x, 
					int y, 
					int w, 
					int h)
{
	WM_HWIN hText;
	hText = TEXT_CreateEx(x, y, w, h, hParent, WM_CF_SHOW, ExFlags, Id, pText);
	TEXT_SetFont      (hText,   FRAME_TEXT_FONT);
	TEXT_SetBkColor   (hText, 	FRAME_TEXT_BKCOLOR0);
	TEXT_SetTextColor (hText, 	FRAME_TEXT_COLOR0);
	WIDGET_SetEffect  (hText,   FRAME_TEXT_EFFECT);
	TEXT_SetWrapMode  (hText,	GUI_WRAPMODE_NONE);
	
	return hText;
}

/*
*********************************************************************************************************
* Function Name : _CreateRadio
* Description	: 创建单选控件
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
WM_HWIN _CreateRadio(WM_HWIN hParent, 
					const char *pText[],
					int Id,
					int ExFlags, 
					int NumItems,
					int Spacing,					
					int x, 
					int y, 
					int w, 
					int h)						
{
	WM_HWIN hRadio;
	
	hRadio = RADIO_CreateEx(x, y, w, h, hParent, WM_CF_SHOW, ExFlags, Id, NumItems, Spacing);
	RADIO_SetFont      (hRadio,   	FRAME_RADIO_FONT);	
	for (uint8_t i=0; i<NumItems; ++i)
	{
		RADIO_SetText(hRadio, 		pText[i], i);
	}
	WIDGET_SetEffect   (hRadio,   	FRAME_RADIO_EFFECT);	
	
	return hRadio;
}

/*
*********************************************************************************************************
*                              				按键消息
*********************************************************************************************************
*/
/*
*********************************************************************************************************
* Function Name : CreateKeyMsgRemap
* Description	: 创建按键消息
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
KEY_MSG_REMAP_TypeDef *CreateKeyMsgRemap(void)
{
	KEY_MSG_REMAP_TypeDef *head = new(sizeof(KEY_MSG_REMAP_TypeDef));
	
	INIT_LIST_HEAD(&head->list);
	
	return head;
}

/*
*********************************************************************************************************
* Function Name : AddKeyMsgRemap
* Description	: 添加按键消息
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
bool AddKeyMsgRemap(KEY_MSG_REMAP_TypeDef *head, int key, \
		void (*sendKeyMsg_CallBack)(void))
{
	KEY_MSG_REMAP_TypeDef *newNode = new(sizeof(KEY_MSG_REMAP_TypeDef));
	newNode->key = key;	
	newNode->sendKeyMsg_CallBack = sendKeyMsg_CallBack;
	
	list_add_tail(&newNode->list, &head->list);
	
	return true;
}

/*
*********************************************************************************************************
* Function Name : FindKeyMsgRemap
* Description	: 查找按键消息
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
KEY_MSG_REMAP_TypeDef *FindKeyMsgRemap(KEY_MSG_REMAP_TypeDef *head, int key)
{
	KEY_MSG_REMAP_TypeDef *node;
	list_for_each_entry(node, &head->list, KEY_MSG_REMAP_TypeDef, list)
	{
		if (node->key == key)
		{
			return node;
		}
	}
	
	return NULL;
}

/*
*********************************************************************************************************
* Function Name : DeleteKeyMsgRemap
* Description	: 删除按键消息
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
bool DeleteKeyMsgRemap(KEY_MSG_REMAP_TypeDef *node)
{
	if (node != NULL)
	{	
		list_del(&node->list);
		delete(node);
		
		return true;
	}
	
	return false;
}

/*
*********************************************************************************************************
* Function Name : DeleteAllKeyMsgRemap
* Description	: 删除所有按键消息
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void DeleteAllKeyMsgRemap(KEY_MSG_REMAP_TypeDef *head)
{
	IteratorKeyMsgRemap(head, DeleteKeyMsgRemap_CallBack);
}

/*
*********************************************************************************************************
* Function Name : IteratorKeyMsgRemap
* Description	: 遍历按键消息映射
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void IteratorKeyMsgRemap(KEY_MSG_REMAP_TypeDef *head, \
		void (*iterator_CallBack)(KEY_MSG_REMAP_TypeDef *node))
{
	KEY_MSG_REMAP_TypeDef *node;
	list_for_each_entry(node, &head->list, KEY_MSG_REMAP_TypeDef, list)
	{
		if (iterator_CallBack)
		{
			iterator_CallBack(node);
		}
	}
}

/*
*********************************************************************************************************
* Function Name : Register_KeyMsgRemap
* Description	: 注册按键消息重映射
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
bool Register_KeyMsgRemap(KEY_MSG_REMAP_TypeDef *head, int key, \
		void (*sendKeyMsg_CallBack)(void))
{
	KEY_MSG_REMAP_TypeDef *node;
	list_for_each_entry(node, &head->list, KEY_MSG_REMAP_TypeDef, list)
	{
		if (node->key == key)
		{
			node->sendKeyMsg_CallBack = sendKeyMsg_CallBack;
			
			return true;
		}
	}
	
	return false;
}

/*
*********************************************************************************************************
* Function Name : DeleteKeyMsgRemap_CallBack
* Description	: 删除按键消息映射
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void DeleteKeyMsgRemap_CallBack(KEY_MSG_REMAP_TypeDef *node)
{
	DeleteKeyMsgRemap(node);
}

/*
*********************************************************************************************************
* Function Name : SendKeyUpMsg_CallBack
* Description	: 发送向上按键消息
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void SendKeyUpMsg_CallBack(void)
{
	GUI_SendKeyMsg(GUI_KEY_UP, true);
}

/*
*********************************************************************************************************
* Function Name : SendKeyDownMsg_CallBack
* Description	: 发送向下按键消息
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void SendKeyDownMsg_CallBack(void)
{
	GUI_SendKeyMsg(GUI_KEY_DOWN, true);
}

/*
*********************************************************************************************************
* Function Name : SendKeyLeftMsg_CallBack
* Description	: 发送向左按键消息
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void SendKeyLeftMsg_CallBack(void)
{
	GUI_SendKeyMsg(GUI_KEY_LEFT, true);
}

/*
*********************************************************************************************************
* Function Name : SendKeyRightMsg_CallBack
* Description	: 发送向右按键消息
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void SendKeyRightMsg_CallBack(void)
{
	GUI_SendKeyMsg(GUI_KEY_RIGHT, true);
}

/*
*********************************************************************************************************
* Function Name : SendKeyEnterMsg_CallBack
* Description	: 发送ENTER按键消息
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void SendKeyEnterMsg_CallBack(void)
{
	GUI_SendKeyMsg(GUI_KEY_ENTER, true);
}

/*
*********************************************************************************************************
* Function Name : SendKeyEscapeMsg_CallBack
* Description	: 发送ESCAPE按键消息
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void SendKeyEscapeMsg_CallBack(void)
{
	GUI_SendKeyMsg(GUI_KEY_ESCAPE, true);
}

/*
*********************************************************************************************************
* Function Name : SendKeySpaceMsg_CallBack
* Description	: 发送SPACE按键消息
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void SendKeySpaceMsg_CallBack(void)
{
	GUI_SendKeyMsg(GUI_KEY_SPACE, true);
}

/*
*********************************************************************************************************
* Function Name : SendKeyTabMsg_CallBack
* Description	: 发送TAB按键消息
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void SendKeyTabMsg_CallBack(void)
{
	GUI_SendKeyMsg(GUI_KEY_TAB, true);
}

/*
*********************************************************************************************************
* Function Name : SendKeyBackTabMsg_CallBack
* Description	: 发送BACKTAB按键消息
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void SendKeyBackTabMsg_CallBack(void)
{
	GUI_SendKeyMsg(GUI_KEY_BACKTAB, true);
}



/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
