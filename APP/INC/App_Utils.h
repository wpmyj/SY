/*
*********************************************************************************************************
* @file    	App_Utils.h
* @author  	SY
* @version 	V1.0.0
* @date    	2016-12-1 11:28:11
* @IDE	 	Keil V5.22.0.0
* @Chip    	STM32F407VE
* @brief   	应用层工具类头文件
*********************************************************************************************************
* @attention
*
* 
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Define to prevent recursive inclusion
*********************************************************************************************************
*/
#ifndef __APP_UTILS_H
#define __APP_UTILS_H

/*
*********************************************************************************************************
*                              				Exported Includes
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                              				Exported define
*********************************************************************************************************
*/
#define SUPPORT_LANGUAGE_NUMS				(2)
#define MAX_TEXT_PUTIN_LENTH				(32)
#define LANG_EXCEPTION						"LangException"

/* 自定义 [软键盘] 消息 */
#define WM_USER_PAD_CHANGED     			(WM_USER + 0)
#define WM_USER_PAD_OK         				(WM_USER + 1)
#define WM_USER_PAD_CANCEL      			(WM_USER + 2)
#define WM_USER_PAD_DELETE       			(WM_USER + 3)
#define WM_USER_PAD_CLEAR       			(WM_USER + 4)
#define WM_USER_UPDATE       				(WM_USER + 5)
#define WM_USER_ESC       					(WM_USER + 6)

/*
*********************************************************************************************************
*                              				Exported types
*********************************************************************************************************
*/
typedef enum {
	LANG_CHINESE = 0,
	LANG_ENGLISH,
}LANGUAGE_TYPE_TypeDef;

/**
  *	由于存在多个控件表示一个逻辑项的情况，
  *	需要有一种机制，通过链表找到任意一层的句柄。
  * 最多支持3层控件叠加。
  */
typedef struct {
	WM_HWIN cursorHandle;			//当前光标所在的句柄
	WM_HWIN handleLevel1;
	WM_HWIN handleLevel2;
	WM_HWIN handleLevel3;
	struct list_head list;
}LIST_HANDLE_TypeDef;

/*
*********************************************************************************************************
*                              				按键消息
*********************************************************************************************************
*/
typedef struct tagKEY_MSG_REMAP_TypeDef {
	int key;
	void (*sendKeyMsg_CallBack)(void);
	struct list_head list;
}KEY_MSG_REMAP_TypeDef;

/*
*********************************************************************************************************
*                              				OPERATE
*********************************************************************************************************
*/
typedef struct {
	void *up;
	void *down;
	void *left;
	void *right;
	
	uint32_t id;
	void *this;				
}OBJECT_OPERATE_TypeDef;

/*
*********************************************************************************************************
*                              				TEXT
*********************************************************************************************************
*/
typedef struct tagWIDGET_TEXT_TypeDef {
	uint32_t id;
	char data[MAX_STRING_NUM];
	
	void *paraPtr;
	void (*setValue_CallBack)(struct tagWIDGET_TEXT_TypeDef *this);
}WIDGET_TEXT_TypeDef;

/*
*********************************************************************************************************
*                              				BUTTON
*********************************************************************************************************
*/
typedef struct tagWIDGET_BUTTON_TypeDef {
	uint32_t id;
	char data[MAX_STRING_NUM];
	
	void *paraPtr;
	void (*pressNotify_CallBack)(struct tagWIDGET_BUTTON_TypeDef *this);
	void (*releaseNotify_CallBack)(struct tagWIDGET_BUTTON_TypeDef *this);
}WIDGET_BUTTON_TypeDef;

/*
*********************************************************************************************************
*                              				SPINBOX
*********************************************************************************************************
*/
typedef struct tagWIDGET_SPINBOX_TypeDef {
	uint32_t id;
	char data[MAX_STRING_NUM];
	void *stepValue;					//步进值
	void *minValue;						//最小值
	void *maxValue;						//最大值
	
	void *paraPtr;
	void (*pressNotify_CallBack)(struct tagWIDGET_SPINBOX_TypeDef *this);
	void (*releaseNotify_CallBack)(struct tagWIDGET_SPINBOX_TypeDef *this);
	void (*changeNotify_CallBack)(struct tagWIDGET_SPINBOX_TypeDef *this, bool cursor);
	void (*setValue_CallBack)(struct tagWIDGET_SPINBOX_TypeDef *this);
}WIDGET_SPINBOX_TypeDef;

/*
*********************************************************************************************************
*                              				ICON
*********************************************************************************************************
*/
typedef struct tagWIDGET_ICON_TypeDef {
	uint32_t id;
	char data[MAX_STRING_NUM];
	
	void *paraPtr;
	void (*pressNotify_CallBack)(struct tagWIDGET_SPINBOX_TypeDef *this);
	void (*releaseNotify_CallBack)(struct tagWIDGET_SPINBOX_TypeDef *this);
	void (*changeNotify_CallBack)(struct tagWIDGET_SPINBOX_TypeDef *this, bool cursor);
	void (*setValue_CallBack)(struct tagWIDGET_SPINBOX_TypeDef *this);
}WIDGET_ICON_TypeDef;

/*
*********************************************************************************************************
*                              				Exported constants
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Exported macro
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Exported variables
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Exported functions
*********************************************************************************************************
*/
void _DrawDownRect(const WIDGET_EFFECT* pEffect, int x0, int y0, int x1, int y1);
void _DrawUpRectEx(const WIDGET_EFFECT* pEffect, const GUI_RECT* pRect);
void _DrawUpRect(const WIDGET_EFFECT* pEffect, int x0, int y0, int x1, int y1);
WM_HWIN _CreateFrame(WM_CALLBACK* cb);
void _PaintFrame(void);

LANGUAGE_TYPE_TypeDef GetLanguageType( void );
void SetLanguageType( LANGUAGE_TYPE_TypeDef type);

/*
*********************************************************************************************************
*                              				按键消息映射
*********************************************************************************************************
*/
KEY_MSG_REMAP_TypeDef *CreateKeyMsgRemap(void);
bool AddKeyMsgRemap(KEY_MSG_REMAP_TypeDef *head, int key, \
		void (*sendKeyMsg_CallBack)(void));
KEY_MSG_REMAP_TypeDef *FindKeyMsgRemap(KEY_MSG_REMAP_TypeDef *head, int key);
bool DeleteKeyMsgRemap(KEY_MSG_REMAP_TypeDef *node);
void IteratorKeyMsgRemap(KEY_MSG_REMAP_TypeDef *head, \
		void (*iterator_CallBack)(KEY_MSG_REMAP_TypeDef *node));

bool Register_KeyMsgRemap(KEY_MSG_REMAP_TypeDef *head, int key, \
		void (*sendKeyMsg_CallBack)(void));
void DeleteKeyMsgRemap_CallBack(KEY_MSG_REMAP_TypeDef *node);
void SendKeyUpMsg_CallBack(void);
void SendKeyDownMsg_CallBack(void);
void SendKeyLeftMsg_CallBack(void);
void SendKeyRightMsg_CallBack(void);
void SendKeyEnterMsg_CallBack(void);
void SendKeyEscapeMsg_CallBack(void);
void SendKeySpaceMsg_CallBack(void);
void SendKeyTabMsg_CallBack(void);
void SendKeyBackTabMsg_CallBack(void);



WM_HWIN _CreateButton(WM_HWIN hParent, 
						const WIDGET_EFFECT* pEffect,
						const char* pText, 
						int Id,						
						int x, 
						int y, 
						int w, 
						int h);
WM_HWIN _CreateEdit(WM_HWIN hParent, 
					const WIDGET_EFFECT* pEffect,
					const char* pText, 
					int Id,
					int ExFlags, 
					int MaxLen,
					int x, 
					int y, 
					int w, 
					int h);
WM_HWIN _CreateText(WM_HWIN hParent, 
					const char* pText, 
					int Id,
					int ExFlags, 
					int x, 
					int y, 
					int w, 
					int h);	
WM_HWIN _CreateRadio(WM_HWIN hParent, 
					const char* pText[],
					int Id,
					int ExFlags, 
					int NumItems,
					int Spacing,
					int x, 
					int y, 
					int w, 
					int h);					


					



#endif
/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
