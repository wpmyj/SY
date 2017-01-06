/*
*********************************************************************************************************
* @file    	app.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-10-11 15:10:28
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	Ӧ�ò�Դ�ļ�
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
#include "App_Language.h"

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
* Function Name : APP_Init
* Description	: APP��ʼ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void APP_Init( void )
{
	ECHO(DEBUG_APP_INIT, "==================== APP ====================");
	
	
	
	ECHO(DEBUG_APP_INIT, "==================== END ====================\r\n");
}

/*
*********************************************************************************************************
*	�� �� ��: _cbDesktop
*	����˵��: ���洰�ڵĻص����� 
*	��    �Σ�WM_MESSAGE * pMsg
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void _cbDesktop(WM_MESSAGE * pMsg) 
{	
	switch (pMsg->MsgId) 
	{
		case WM_PAINT:	
		{
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
* Function Name : NotifyFlashFontProgress_CallBack
* Description	: ֪ͨ�ֿ���½���
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
* Description	: ����Ӧ�ó�������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void AppTaskCreate(void)
{
	App_MenuTaskCreate();
}

/*
*********************************************************************************************************
* Function Name : MainTask
* Description	: GUI���������е�GUI�����Ǹ���������ࣩ
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void MainTask(void) 
{ 
	/* emWin ��ʼ�� */
	GUI_Init();  
	
	/* ��ֹ������˸ */	
	WM_SetCreateFlags(WM_CF_MEMDEV);
		
	/* ����Ĭ��Ƥ�� */
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
	
	/* �����ֿ� */
	UpdateFlashFontTask(NotifyFlashFontProgress_CallBack);

	/* ��������ص����� */
	WM_SetCallback(WM_HBKWIN, _cbDesktop);
	
	/* ��ʾ�α� */
#if 0
	GUI_CURSOR_Show();
	GUI_CURSOR_SetPosition(0,0);
	GUI_CURSOR_Select(&GUI_CursorArrowLI);
#endif	

	/* �������� */
	AppTaskCreate();
	
	while (1)
	{		
		GUI_Delay(100);
	}
}

/*
*********************************************************************************************************
* Function Name : AppTaskGUI
* Description	: GUI����
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
							case KEY_VALUE_ENTER:
								GUI_SendKeyMsg(GUI_KEY_ENTER, true);
								break;
							case KEY_VALUE_ESC:
								GUI_SendKeyMsg(GUI_KEY_ESCAPE, true);
								break;
							case KEY_VALUE_BACK:
								GUI_SendKeyMsg(GUI_KEY_BACKSPACE, true);
								break;
							case KEY_VALUE_LEFT:
								GUI_SendKeyMsg(GUI_KEY_LEFT, true);
								break;
							case KEY_VALUE_RIGHT:
								GUI_SendKeyMsg(GUI_KEY_RIGHT, true);
								break;
							case KEY_VALUE_UP:
								GUI_SendKeyMsg(GUI_KEY_UP, true);
								break;
							case KEY_VALUE_DOWN:
								GUI_SendKeyMsg(GUI_KEY_DOWN, true);
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
		
		/* ��������� */
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

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
