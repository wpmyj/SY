/*
*********************************************************************************************************
* @file    	app.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-10-11 15:10:28
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	应用层源文件
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
#include "version.h"

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
KEY_MSG_REMAP_TypeDef *hKeyMsgRemapHeadPtr;
WM_HWIN hSuperWindows;

/*
*********************************************************************************************************
*                              				Private function prototypes
*********************************************************************************************************
*/
void APP_DebugViewTaskCreate( void );


/*
*********************************************************************************************************
*                              				Private functions
*********************************************************************************************************
*/
/*
*********************************************************************************************************
* Function Name : PrintControllerInfo
* Description	: 打印控制器信息
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void PrintControllerInfo(void)
{
	ECHO(DEBUG_BSP_INIT, "\r\n========== [控制器信息] ==========");
	ECHO(DEBUG_BSP_INIT, "[系统型号] %s", SYSTEM_MODEL);
	ECHO(DEBUG_BSP_INIT, "[PCB 版本] %s", PCB_VERSION);
	ECHO(DEBUG_BSP_INIT, "[固件版本] %s", SOFT_VERSION);
	ECHO(DEBUG_BSP_INIT, "[开 发 者] %s", DEVELOPER_NAME);
	ECHO(DEBUG_BSP_INIT, "[发行日期] %s", RELEASE_DATE);
	ECHO(DEBUG_BSP_INIT,"");
}

/*
*********************************************************************************************************
* Function Name : APP_Init
* Description	: APP初始化
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void APP_Init( void )
{
	ECHO(DEBUG_APP_INIT, "==================== APP ====================");
	
	hKeyMsgRemapHeadPtr = CreateKeyMsgRemap();
	if (hKeyMsgRemapHeadPtr != NULL)
	{		
		ECHO(DEBUG_APP_INIT, "按键消息重映射初始化 .......... OK");
	}
	else
	{
		ECHO(DEBUG_APP_INIT, "按键消息重映射初始化 .......... ERR");
		while (1);
	}
	
	PrintControllerInfo();
	
	ECHO(DEBUG_APP_INIT, "==================== END ====================\r\n");
}

/*
*********************************************************************************************************
*	函 数 名: _cbDesktop
*	功能说明: 桌面回调函数 
*	形    参：WM_MESSAGE * pMsg
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbDesktop(WM_MESSAGE * pMsg) 
{	
	switch (pMsg->MsgId) 
	{
		case WM_PAINT:	
		{
			ECHO(DEBUG_APP_WINDOWS, "桌面创建");
			
			GUI_SetBkColor(MAIN_BKCOLOR);
			GUI_SetColor(MAIN_TEXTCOLOR);
			GUI_SetFont(MAIN_FONT);
			GUI_Clear();
			
			{
				int x, y, w, h;
				
				x = MAIN_BORDER;
				y = MAIN_TITLE_HEIGHT;
				w = FRAME_TOTAL_WIDTH - (MAIN_BORDER * 2);
				h = FRAME_TOTAL_HEIGHT - (MAIN_BORDER + MAIN_TITLE_HEIGHT);
				_DrawDownRect(FRAME_EFFECT, x, y, x + w - 1, y + h - 1);
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
* Function Name : _cbWindows
* Description	: 窗口回调函数
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void _cbWindows(WM_MESSAGE *pMsg) 
{
	WM_HWIN hWin = pMsg->hWin;
	(void)hWin;
	
	switch (pMsg->MsgId) 
	{
		case WM_CREATE:
			ECHO(DEBUG_APP_WINDOWS, "窗口创建");
			break;
		case WM_DELETE:
			ECHO(DEBUG_APP_WINDOWS, "窗口删除");
			break;
		case WM_PAINT:
			_PaintFrame();
			break;		
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}

/*
*********************************************************************************************************
* Function Name : NotifyFlashFontProgress_CallBack
* Description	: 通知字库更新进度
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void NotifyFlashFontProgress_CallBack( uint8_t fontSize, uint8_t progress )
{
	ECHO(DEBUG_APP_INIT, "font = %d, progress = %d\r\n",fontSize,progress);
}

/*
*********************************************************************************************************
* Function Name : AppTaskCreate
* Description	: 创建应用程序任务
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void AppTaskCreate(void)
{
	/* 设置桌面回调函数 */
	WM_SetCallback(WM_HBKWIN, _cbDesktop);	
	hSuperWindows = _CreateFrame(_cbWindows);

	App_MenuTaskCreate();
}

/*
*********************************************************************************************************
* Function Name : MainTask
* Description	: GUI主任务（所有的GUI任务都是该任务的子类）
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void MainTask(void) 
{
	/* emWin 初始化 */
	GUI_Init();  
	
	/* 防止窗口闪烁 */	
	WM_SetCreateFlags(WM_CF_MEMDEV);
		
	/* 设置默认皮肤 */
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
	CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
	MULTIPAGE_SetDefaultSkin(MULTIPAGE_SKIN_FLEX);
	
	/* 更新字库 */
	UpdateFlashFontTask(NotifyFlashFontProgress_CallBack);
	
	/* 创建任务 */
	AppTaskCreate();
	
	while (1)
	{			
		GUI_Delay(50);
	}
}

/*
*********************************************************************************************************
* Function Name : AppTaskGUI
* Description	: GUI任务
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void AppTaskGUI(void *p_arg)
{
	(void)p_arg;
	
	while (1)
	{
		MainTask();
	}
}

/*
*********************************************************************************************************
* Function Name : AppTaskUserIF
* Description	: USER IF
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void AppTaskUserIF(void *p_arg)
{
	(void)p_arg;
	
	uint32_t keyCount = 0;
#if 0	
	uint32_t pidCount =0;
#endif	
	while (1)
	{
		if (++keyCount > 20)
		{
			keyCount = 0;
			
			uint32_t key = ReadKeyValue(g_keyPtr);
			if (key != KEY_VALUE_NONE)
			{		
				switch (ReadKeyStatus(g_keyPtr))
				{
					case STATUS_KEY_PRESS:
					case STATUS_KEY_LONG:
					case STATUS_KEY_HIGH_SPEED:	
					{	
					#if 0
						const uint16_t STEP_DISTANCE = 10;
						GUI_PID_STATE pid;
						GUI_PID_GetState(&pid);
						pid.Pressed = false;	
						
						switch ( key )		
						{
							case KEY_VALUE_LEFT:
								pid.x -= STEP_DISTANCE;
								break;
							case KEY_VALUE_RIGHT:
								pid.x += STEP_DISTANCE;
								break;
							case KEY_VALUE_UP:
								pid.y -= STEP_DISTANCE;
								break;
							case KEY_VALUE_DOWN:
								pid.y += STEP_DISTANCE;
								break;
							case KEY_VALUE_ENTER:
								pid.Pressed = true;
								break;
						}
						if (pid.x < 0)
						{
							pid.x = 0;
						}
						if (pid.x > LCD_GetWidth())
						{
							pid.x = LCD_GetWidth();
						}
						if (pid.y < 0)
						{
							pid.y = 0;
						}
						if (pid.y > LCD_GetHeight())
						{
							pid.y = LCD_GetHeight();
						}
						
						GUI_PID_StoreState(&pid);
						GUI_CURSOR_SetPosition(pid.x, pid.y);
						
						ECHO(DEBUG_KERNEL_KEY, "[key] = %d", key);
					#endif
						
					#if 1
						switch ( key )		
						{		
							case KEY_VALUE_UP:
							{
								KEY_MSG_REMAP_TypeDef *node = FindKeyMsgRemap(GetKeyMsgRemapHandle(), \
																GUI_KEY_UP);
								
								if ((node != NULL) && (node->sendKeyMsg_CallBack != NULL))
								{
									node->sendKeyMsg_CallBack();
								}
								else
								{
									GUI_SendKeyMsg(GUI_KEY_UP, true);
								}
								break;
							}
							case KEY_VALUE_DOWN:
							{
								KEY_MSG_REMAP_TypeDef *node = FindKeyMsgRemap(GetKeyMsgRemapHandle(), \
																GUI_KEY_DOWN);
								
								if ((node != NULL) && (node->sendKeyMsg_CallBack != NULL))
								{
									node->sendKeyMsg_CallBack();
								}
								else
								{
									GUI_SendKeyMsg(GUI_KEY_DOWN, true);
								}
								break;
							}
							case KEY_VALUE_LEFT:
							{
								KEY_MSG_REMAP_TypeDef *node = FindKeyMsgRemap(GetKeyMsgRemapHandle(), \
																GUI_KEY_LEFT);
								
								if ((node != NULL) && (node->sendKeyMsg_CallBack != NULL))
								{
									node->sendKeyMsg_CallBack();
								}
								else
								{
									GUI_SendKeyMsg(GUI_KEY_LEFT, true);
								}
								break;
							}
							case KEY_VALUE_RIGHT:
							{
								KEY_MSG_REMAP_TypeDef *node = FindKeyMsgRemap(GetKeyMsgRemapHandle(), \
																GUI_KEY_RIGHT);
								
								if ((node != NULL) && (node->sendKeyMsg_CallBack != NULL))
								{
									node->sendKeyMsg_CallBack();
								}
								else
								{
									GUI_SendKeyMsg(GUI_KEY_RIGHT, true);
								}
								break;
							}
							case KEY_VALUE_ENTER:
							{
								KEY_MSG_REMAP_TypeDef *node = FindKeyMsgRemap(GetKeyMsgRemapHandle(), \
																GUI_KEY_ENTER);
								
								if ((node != NULL) && (node->sendKeyMsg_CallBack != NULL))
								{
									node->sendKeyMsg_CallBack();
								}
								else
								{
									GUI_SendKeyMsg(GUI_KEY_ENTER, true);
								}
								break;
							}
							case KEY_VALUE_ESC:
							{
								GUI_SendKeyMsg(GUI_KEY_ESCAPE, true);
								break;
							}
							case KEY_VALUE_BACK:
								GUI_SendKeyMsg(GUI_KEY_BACKSPACE, true);
								break;	
							case KEY_VALUE_1:
								GUI_SendKeyMsg(GUI_KEY_BACKTAB, true);
								break;
							case KEY_VALUE_2:
								GUI_SendKeyMsg(GUI_KEY_HOME, true);
								break;
							case KEY_VALUE_3:
								GUI_SendKeyMsg(GUI_KEY_END, true);
								break;
							case KEY_VALUE_4:
								GUI_SendKeyMsg(GUI_KEY_SHIFT, true);
								break;
							case KEY_VALUE_5:
								GUI_SendKeyMsg(GUI_KEY_CONTROL, true);
								break;
							case KEY_VALUE_6:
								GUI_SendKeyMsg(GUI_KEY_INSERT, true);
								break;
							case KEY_VALUE_7:
								GUI_SendKeyMsg(GUI_KEY_DELETE, true);
								break;
							case KEY_VALUE_8:
								GUI_SendKeyMsg(GUI_KEY_SPACE, true);
								break;
							case KEY_VALUE_9:
								GUI_SendKeyMsg(GUI_KEY_PGUP, true);
								break;
							case KEY_VALUE_0:
								GUI_SendKeyMsg(GUI_KEY_PGDOWN, true);
								break;
							case KEY_VALUE_F1:
								GUI_SendKeyMsg(GUI_KEY_F1, true);
								break;
							case KEY_VALUE_F2:
								GUI_SendKeyMsg(GUI_KEY_F2, true);
								break;
							case KEY_VALUE_F4:
								GUI_SendKeyMsg(GUI_KEY_TAB, true);
								break;
						}
					#endif	
					}
					case STATUS_KEY_LEAVE:
					{
						break;
					}
					default:
					{		
						break;
					}
				}			
				ReleaseKeyMsg(g_keyPtr);
			}
		}
		
		/* 鼠标检测任务 */
	#if 0	
		if (++pidCount > 1000)
		{
			pidCount = 0;
			HID_HostDetectTask(&HID_HostState);
		}
	#endif	
		
		BSP_OS_TimeDlyMs(1);
	}
}

/*
*********************************************************************************************************
* Function Name : GetKeyMsgRemapHandle
* Description	: 获取按键消息重映射句柄
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
KEY_MSG_REMAP_TypeDef *GetKeyMsgRemapHandle(void)
{
	return hKeyMsgRemapHeadPtr;
}


/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
