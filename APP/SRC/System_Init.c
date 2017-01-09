/*
*********************************************************************************************************
* @file    	System_Init.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-10-11 15:10:28
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	ϵͳ��ʼ��Դ�ļ�
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
#include "System_Init.h"
#include "bsp.h"
#include "kernel.h"
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
__ALIGN_RAM	static  OS_TCB      AppTaskStartTCB;
__ALIGN_RAM static  CPU_STK     AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];

__ALIGN_RAM	static  OS_TCB		AppTaskUpdateTCB;
					OS_TCB 		*AppTaskUpdateTCB_Ptr = &AppTaskUpdateTCB;
__ALIGN_RAM	static  CPU_STK  	AppTaskUpdateStk[APP_CFG_TASK_UPDATE_STK_SIZE];

__ALIGN_RAM	static  OS_TCB   	AppTaskCOMTCB;
					OS_TCB 		*AppTaskCOMTCB_Ptr = &AppTaskCOMTCB;
__ALIGN_RAM	static  CPU_STK  	AppTaskCOMStk[APP_CFG_TASK_COM_STK_SIZE];

__ALIGN_RAM	static  OS_TCB   	AppTaskUserIFTCB;
					OS_TCB 		*AppTaskUserIFTCB_Ptr = &AppTaskUserIFTCB;
__ALIGN_RAM	static  CPU_STK  	AppTaskUserIFStk[APP_CFG_TASK_USER_IF_STK_SIZE];

__ALIGN_RAM	static  OS_TCB   	AppTaskGUITCB;
					OS_TCB 		*AppTaskGUITCB_Ptr = &AppTaskGUITCB;
__ALIGN_RAM	static  CPU_STK  	AppTaskGUIStk[APP_CFG_TASK_GUI_STK_SIZE];

__ALIGN_RAM	static  OS_TCB   	AppTaskGUIRefreshTCB;
					OS_TCB 		*AppTaskGUIRefreshTCB_Ptr = &AppTaskGUIRefreshTCB;
__ALIGN_RAM	static  CPU_STK  	AppTaskGUIRefreshStk[APP_CFG_TASK_GUI_REFRESH_STK_SIZE];

__ALIGN_RAM	static  OS_TCB   	AppTaskIOTCB;
					OS_TCB 		* const AppTaskIOTCB_Ptr = &AppTaskIOTCB;
__ALIGN_RAM	static  CPU_STK  	AppTaskIOStk[APP_CFG_TASK_IO_STK_SIZE];


/*
*********************************************************************************************************
*                              				Private function prototypes
*********************************************************************************************************
*/
static  void  AppTaskStart (void *p_arg);
static  void  AppTaskCreate (void);
void DispTaskInfo(void);

/*
*********************************************************************************************************
*                              				Private functions
*********************************************************************************************************
*/
/*
*********************************************************************************************************
* Function Name : AppTaskStartUtils
* Description	: ��ʼ���� Utils
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
__weak void AppTaskStartUtils(void)
{
	;
}

/*
*********************************************************************************************************
*                                          AppObjCreate()
*
* Description : Create Application Kernel Objects.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : AppTaskStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/
__weak void AppObjCreate(void)
{
	;
}

/*
*********************************************************************************************************
* Function Name : AppTaskGUIUpdate
* Description	: GUI��������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
__weak void AppTaskGUIUpdate(void *p_arg)
{
	(void)p_arg;
	while (1)
	{
		BSP_OS_TimeDlyMs(1000);
	}
}

/*
*********************************************************************************************************
* Function Name : AppTaskCOM
* Description	: COM����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
__weak void AppTaskCOM(void *p_arg)
{
	(void)p_arg;
	while (1)
	{
		BSP_OS_TimeDlyMs(1000);
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
__weak void AppTaskUserIF(void *p_arg)
{
	(void)p_arg;
	while (1)
	{
		BSP_OS_TimeDlyMs(1000);
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
__weak void AppTaskGUI(void *p_arg)
{
	(void)p_arg;
	while (1)
	{
		BSP_OS_TimeDlyMs(1000);
	}
}

/*
*********************************************************************************************************
* Function Name : AppTaskGUIRefresh
* Description	: GUIˢ������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
__weak void AppTaskGUIRefresh(void *p_arg)
{
	(void)p_arg;
	while (1)
	{
		BSP_OS_TimeDlyMs(1000);
	}
}

/*
*********************************************************************************************************
* Function Name : AppTaskIO
* Description	: IO����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
__weak void AppTaskIO(void *p_arg)
{
	(void)p_arg;
	while (1)
	{
		BSP_OS_TimeDlyMs(1000);
	}
}

/*
*********************************************************************************************************
* Function Name : System_Init
* Description	: ϵͳ��ʼ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void System_Init( void )
{
	OS_ERR   err;
	
	HAL_Init();
	
	SystemClock_Config();										/* Initialize RCC functions                             */

	CPU_Init();                                                 /* Initialize the uC/CPU services                       */
	
	Mem_Init();                                                 /* Initialize Memory Management Module                  */
    Math_Init();                                                /* Initialize Mathematical Module                       */
	
	OSInit(&err);                                               /* Init uC/OS-III.  */
	App_OS_SetAllHooks();
	
	OSTaskCreate((OS_TCB       *)&AppTaskStartTCB,         		/* ������������ */
                 (CPU_CHAR     *)"App Task Start",
                 (OS_TASK_PTR   )AppTaskStart, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_START_PRIO,
                 (CPU_STK      *)&AppTaskStartStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_START_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_START_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
                 (OS_ERR       *)&err);
	
	OSStart(&err);                                              /* Start multitasking (i.e. give control to uC/OS-III). */

    while (DEF_ON) {                                            /* Should Never Get Here.                               */
        ;
    }
}

/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static void AppTaskStart(void *p_arg)
{
	(void)p_arg;
	OS_ERR err;
	
	BSP_Init();													/* Initialize BSP functions                             */
	KERNEL_Init();												/* Initialize KERNEL functions                          */
	APP_Init();													/* Initialize APP functions                             */
	
#if OS_CFG_STAT_TASK_EN > 0u
	OSStatTaskCPUUsageInit(&err);
#endif
#ifdef CPU_CFG_INT_DIS_MEAS_EN
	CPU_IntDisMeasMaxCurReset();
#endif
	
#if (DEBUG_BSP_INIT)	
	{
		char buff[10];
		sprintf(buff, "%d", OSVersion(&err));
		
		char versionBuff[10] = {0};
		memcpy(&versionBuff[0], &buff[0], 1);
		strcat(versionBuff,".");
		memcpy(&versionBuff[2], &buff[1], 2);
		strcat(versionBuff,".");
		memcpy(&versionBuff[5], &buff[3], 2);
		
		ECHO(DEBUG_BSP_INIT, "uCOS-III Version��%s", versionBuff);
	}
#endif	
	
	AppObjCreate();                                             /* Create Applicaiton kernel objects                    */
	AppTaskCreate();                                            /* Create Application tasks                             */

	while (1)
	{			
		AppTaskStartUtils();
		BSP_OS_TimeDlyMs(1);
	}
}

/*
*********************************************************************************************************
*                                          AppTaskCreate()
*
* Description : Create Application Tasks.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : AppTaskStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/

static void AppTaskCreate(void)
{
	OS_ERR      err;
	
	/**************����UPDATE����*********************/
	OSTaskCreate((OS_TCB       *)&AppTaskUpdateTCB,             
                 (CPU_CHAR     *)"App Task Update",
                 (OS_TASK_PTR   )AppTaskGUIUpdate, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_UPDATE_PRIO,
                 (CPU_STK      *)&AppTaskUpdateStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_UPDATE_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_UPDATE_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
                 (OS_ERR       *)&err);

	/**************����COM����*********************/
	OSTaskCreate((OS_TCB       *)&AppTaskCOMTCB,            
                 (CPU_CHAR     *)"App Task COM",
                 (OS_TASK_PTR   )AppTaskCOM, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_COM_PRIO,
                 (CPU_STK      *)&AppTaskCOMStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_COM_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_COM_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
                 (OS_ERR       *)&err);
	
	/**************����USER IF����*********************/
	OSTaskCreate((OS_TCB       *)&AppTaskUserIFTCB,             
                 (CPU_CHAR     *)"App Task UserIF",
                 (OS_TASK_PTR   )AppTaskUserIF, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_USER_IF_PRIO,
                 (CPU_STK      *)&AppTaskUserIFStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_USER_IF_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_USER_IF_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
                 (OS_ERR       *)&err);
	
	/**************����GUI����*********************/			 
	OSTaskCreate((OS_TCB       *)&AppTaskGUITCB,              
                 (CPU_CHAR     *)"App Task GUI",
                 (OS_TASK_PTR   )AppTaskGUI, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_GUI_PRIO,
                 (CPU_STK      *)&AppTaskGUIStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_GUI_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_GUI_STK_SIZE,
                 (OS_MSG_QTY    )1,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
                 (OS_ERR       *)&err);
	
	/**************����GUIˢ������*********************/			 
	OSTaskCreate((OS_TCB       *)&AppTaskGUIRefreshTCB,              
                 (CPU_CHAR     *)"App Task GUIRefresh",
                 (OS_TASK_PTR   )AppTaskGUIRefresh, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_GUI_REFRESH_PRIO,
                 (CPU_STK      *)&AppTaskGUIRefreshStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_GUI_REFRESH_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_GUI_REFRESH_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
                 (OS_ERR       *)&err);		

	/**************����IO����*********************/			 
	OSTaskCreate((OS_TCB       *)&AppTaskIOTCB,              
                 (CPU_CHAR     *)"App Task IO",
                 (OS_TASK_PTR   )AppTaskIO, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_IO_PRIO,
                 (CPU_STK      *)&AppTaskIOStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_IO_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_IO_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
                 (OS_ERR       *)&err);	
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
