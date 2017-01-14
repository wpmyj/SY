/*
*********************************************************************************************************
* @file    	App_DebugView.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-11-21 11:12:33
* @IDE	 	Keil V5.22.0.0
* @Chip    	STM32F407VE
* @brief   	Debug View����Դ�ļ�
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
#include "App_DebugView.h"
#include "emWinUtils.h"
#include "SeqQueue.h"

/*
*********************************************************************************************************
*                              				Private define
*********************************************************************************************************
*/
/* ����ID */
#define ID_FRAMEWIN_0     					(GUI_ID_USER + 0x00)
#define ID_MULTIEDIT_0    					(GUI_ID_USER + 0x01)

#define DEBUG_VIEW_MAX_BUFF_SIZE			(128)


/*
*********************************************************************************************************
*                              				Private typedef
*********************************************************************************************************
*/
typedef struct
{
	bool init_OK;							//��ʼ��
	SEQUEUE_TypeDef queue;					//˳�����
}DEBUG_VIEW_TypeDef;

/*
*********************************************************************************************************
*                              				Private constants
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO DebugViewDialogResource[] = {
	{ FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 520, 300, 0, FRAMEWIN_CF_MOVEABLE, 0 },
	{ MULTIEDIT_CreateIndirect, "Multiedit", ID_MULTIEDIT_0, 10, 10, 500, 250, 0, 0x0, 0 },
};

static const uint32_t QUEUE_MEMORY_BUFF_SIZE = 1024;

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
__ALIGN_RAM	static  OS_TCB   	AppTaskDebugViewTCB;
					OS_TCB 		*AppTaskDebugViewTCB_Ptr = &AppTaskDebugViewTCB;
__ALIGN_RAM static  CPU_STK  	AppTaskDebugViewStk[APP_CFG_TASK_DEBUG_VIEW_STK_SIZE];

__ALIGN_RAM	static  OS_TCB   	AppTaskDebugViewRefreshTCB;
					OS_TCB 		*AppTaskDebugViewRefreshTCB_Ptr = &AppTaskDebugViewRefreshTCB;
__ALIGN_RAM static  CPU_STK  	AppTaskDebugViewRefreshStk[APP_CFG_TASK_DEBUG_VIEW_REFRESH_STK_SIZE];

static uint8_t queueMemory[QUEUE_MEMORY_BUFF_SIZE];
static DEBUG_VIEW_TypeDef g_debugView;

/*
*********************************************************************************************************
*                              				Private function prototypes
*********************************************************************************************************
*/
void AppTaskDebugViewRefresh(void *p_arg);
void AppTaskDebugView(void *p_arg);

/*
*********************************************************************************************************
*                              				Private functions
*********************************************************************************************************
*/
/*
*********************************************************************************************************
* Function Name : App_DebugViewTaskCreate
* Description	: ���� DebugView ����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void APP_DebugViewTaskCreate( void )
{
	OS_ERR err;
	
	/**************���� DEBUG VIEW ����*********************/			 
	OSTaskCreate((OS_TCB       *)&AppTaskDebugViewTCB,              
                 (CPU_CHAR     *)"App Task DbgView",
                 (OS_TASK_PTR   )AppTaskDebugView, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_DEBUG_VIEW_PRIO,
                 (CPU_STK      *)&AppTaskDebugViewStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_DEBUG_VIEW_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_DEBUG_VIEW_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
                 (OS_ERR       *)&err);		

	/**************���� DEBUG VIEW REFRESH ����*********************/			 
	OSTaskCreate((OS_TCB       *)&AppTaskDebugViewRefreshTCB,              
                 (CPU_CHAR     *)"App Task DbgViewRefresh",
                 (OS_TASK_PTR   )AppTaskDebugViewRefresh, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_DEBUG_VIEW_REFRESH_PRIO,
                 (CPU_STK      *)&AppTaskDebugViewRefreshStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_DEBUG_VIEW_REFRESH_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_DEBUG_VIEW_REFRESH_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
                 (OS_ERR       *)&err);	
}

/*
*********************************************************************************************************
* Function Name : bsp_InitDebugView
* Description	: ��ʼ��Debug View
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_InitDebugView( DEBUG_VIEW_TypeDef *this )
{
	memset(this, 0x00, sizeof(DEBUG_VIEW_TypeDef));
	memset(queueMemory, 0x00, QUEUE_MEMORY_BUFF_SIZE);
	
	this->init_OK = true;
	CreateSeqQueue(&this->queue, queueMemory, QUEUE_MEMORY_BUFF_SIZE);
}

/*
*********************************************************************************************************
* Function Name : bsp_DebugViewPushData
* Description	: ѹ������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static bool bsp_DebugViewPushData( DEBUG_VIEW_TypeDef *this, uint8_t dataIn )
{
	if (this->init_OK == false)
	{
		return false;
	}
	
	if (PushSeqQueue(&this->queue, &dataIn, PushSeqQueueU8_CallBack) == STATUS_DATA_STRUCT_TRUE)
	{
		return true;
	}
	
	return false;
}

/*
*********************************************************************************************************
* Function Name : bsp_DebugViewQueueDataHandler
* Description	: �������ݴ���
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
uint32_t bsp_DebugViewQueueDataHandler( DEBUG_VIEW_TypeDef *this, uint8_t *wrBuff,\
										uint32_t buffSize )
{
	uint32_t wrLenth = 0;
	
	if (this->init_OK == false)
	{
		return wrLenth;
	}
	
	uint8_t dataOut = 0;
	DATA_STRUCT_STATUS_ENUM result;
	bool isLeave = false;
	while (1)
	{
		result = PopSeqQueue(&this->queue, &dataOut, PopSeqQueueU8_CallBack);		
		if (result == STATUS_DATA_STRUCT_TRUE)
		{	
			if (wrLenth > buffSize)
			{
				return wrLenth;
			}
			
			if (dataOut == '\r')
			{
				dataOut = '\n';
			}
			else if (dataOut == '\n')
			{
				dataOut = NULL;
				isLeave = true;
			}
			wrBuff[wrLenth++] = dataOut;			
			
			if (isLeave == true)
			{
				return wrLenth;
			}
		}
		else
		{
			return wrLenth;
		}
	}
}

/*
*********************************************************************************************************
* Function Name : bsp_fputc
* Description	: fputc����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_fputc( uint8_t ch )
{
	bsp_DebugViewPushData(&g_debugView, ch);
}

/*
*********************************************************************************************************
* Function Name : DebugView_InitDialog
* Description	: ��ʼ���Ի���
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void DebugView_InitDialog(WM_MESSAGE * pMsg)
{
	WM_HWIN hItem;
	
	/* Framewin */
	hItem = pMsg->hWin;
	FRAMEWIN_SetText(hItem, "Debug View");
	FRAMEWIN_SetFont(hItem,&GUI_Font32B_ASCII);
    FRAMEWIN_SetTextAlign(hItem,GUI_TA_VCENTER|GUI_TA_CENTER);
	FRAMEWIN_SetTextColor(hItem, GUI_BLUE);
    FRAMEWIN_SetTitleHeight(hItem,35);
	FRAMEWIN_AddCloseButton(hItem, FRAMEWIN_BUTTON_RIGHT, 0);
    	
	/* Multiedit */
	hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIEDIT_0);
	
    MULTIEDIT_SetFont(hItem, &GUI_FontSongTi_16X16);	
	
	MULTIEDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_TOP);
	MULTIEDIT_SetTextColor(hItem, MULTIEDIT_CI_EDIT, GUI_GREEN);
	MULTIEDIT_SetBkColor(hItem, MULTIEDIT_CI_EDIT, GUI_BLACK);
	MULTIEDIT_SetWrapChar(hItem);
	MULTIEDIT_SetInsertMode(hItem, true);
	
	MULTIEDIT_SetAutoScrollV(hItem, true);
}

/*
*********************************************************************************************************
* Function Name : DebugView_PaintDialog
* Description	: ���ƶԻ���
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void DebugView_PaintDialog(WM_MESSAGE * pMsg)
{
	GUI_SetBkColor(GUI_BLACK);	
	GUI_Clear();
}

/*
*********************************************************************************************************
* Function Name : DebugView_Callback
* Description	: �Ի���ص�����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void _cbCallback(WM_MESSAGE * pMsg) 
{
    switch (pMsg->MsgId) 
    {
        case WM_PAINT:
			DebugView_PaintDialog(pMsg);
            break;
        case WM_INIT_DIALOG:
            DebugView_InitDialog(pMsg);
            break;
        case WM_KEY:
			break;
        case WM_NOTIFY_PARENT:
            break;
        default:
            WM_DefaultProc(pMsg);
			break;
    }
}

/*
*********************************************************************************************************
* Function Name : LCD_DebugViewTask
* Description	: LCD ��ӡ������Ϣ
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void LCD_DebugViewTask( void )
{
	WM_HWIN hWM_Father = GUI_CreateDialogBox(DebugViewDialogResource,
											  GUI_COUNTOF(DebugViewDialogResource),
											  &_cbCallback, 
											  WM_HBKWIN, 
											  0, 
											  0);	
	WM_HWIN hMultiEdit = WM_GetDialogItem(hWM_Father, ID_MULTIEDIT_0);
	
	uint32_t msgSize = 0;
	OS_ERR err;
	void *rxDataPtr = NULL;
	
	char dbgBuff[DEBUG_VIEW_MAX_BUFF_SIZE+1] = {0};
	const uint32_t MULTIEDIT_BUFF_SIZE = 900;
	while (1)
	{
		rxDataPtr = OSTaskQPend((OS_TICK )0,
								(OS_OPT)OS_OPT_PEND_BLOCKING,
								(OS_MSG_SIZE *)&msgSize,
								(CPU_TS *)0,
								(OS_ERR *)&err);
		if (err == OS_ERR_NONE)
		{		
			/* �ı�������ֵ��������Ļ */
			uint32_t textSize = MULTIEDIT_GetTextSize(hMultiEdit);
			if (textSize > MULTIEDIT_BUFF_SIZE * 0.8)
			{
				MULTIEDIT_SetBufferSize(hMultiEdit, MULTIEDIT_BUFF_SIZE);
			}
			
			memcpy(dbgBuff, rxDataPtr, msgSize);					
			MULTIEDIT_AddText(hMultiEdit, dbgBuff);
		}
		
		GUI_Delay(1);
	}
}

/*
*********************************************************************************************************
* Function Name : AppTaskDebugView
* Description	: Debug View����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void AppTaskDebugView(void *p_arg)
{
	(void)p_arg;
	OS_ERR err;
	
	bsp_InitDebugView(&g_debugView);
	
	uint8_t dbgBuff[DEBUG_VIEW_MAX_BUFF_SIZE] = {0};
	uint32_t dispCnt = 0;
	
	while (1)
	{
		if (++dispCnt >= 20)
		{
			dispCnt = 0;
		#if DEBUG_BSP_INIT
			//DispTaskInfo();
		#endif			
		}
		
		uint32_t rxLenth = bsp_DebugViewQueueDataHandler(&g_debugView, 
														 dbgBuff,
														 DEBUG_VIEW_MAX_BUFF_SIZE);
		if (rxLenth)
		{
			do {
				OSTaskQPost((OS_TCB *)AppTaskDebugViewRefreshTCB_Ptr,
							(void *)dbgBuff,
							(OS_MSG_SIZE)rxLenth,
							(OS_OPT)OS_OPT_POST_FIFO,
							(OS_ERR *)&err);
				if (err != OS_ERR_NONE)
				{
					BSP_OS_TimeDlyMs(1);			
				}
			} while (err != OS_ERR_NONE);
		}
		
		BSP_OS_TimeDlyMs(1);
	}
}

/*
*********************************************************************************************************
* Function Name : AppTaskDebugViewRefresh
* Description	: Debug View Refresh����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void AppTaskDebugViewRefresh(void *p_arg)
{
	(void)p_arg;
	
	while (1)
	{	
		LCD_DebugViewTask();
		
		BSP_OS_TimeDlyMs(1000);
	}
}



/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
