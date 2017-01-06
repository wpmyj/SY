/*
*********************************************************************************************************
* @file    	bsp_timer.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-10-12 13:15:44
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	��ʱ��Դ�ļ�
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
#include "bsp.h"


/*
*********************************************************************************************************
*                              				Private define
*********************************************************************************************************
*/			
#define	HEART_TIM							TIM3
#define HEART_TIM_IRQn                      TIM3_IRQn
#define HEART_TIM_IRQHandler				TIM3_IRQHandler	
#define HEART_TIM_CLK_ENABLE()				__HAL_RCC_TIM3_CLK_ENABLE()

#define	HEART_TIM_Int_Enable()				HAL_TIM_Base_Start_IT(&g_HeartTimHandle)		
#define	HEART_TIM_Int_Disable()				HAL_TIM_Base_Stop_IT(&g_HeartTimHandle)

#define	Systick_Int_Enable()				(SysTick->CTRL)|=(1<<SysTick_CTRL_TICKINT_Pos)
#define	Systick_Int_Disable()				(SysTick->CTRL)&=~(1<<SysTick_CTRL_TICKINT_Pos)

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
/* ��2��ȫ�ֱ���ת���� bsp_DelayMS() ���� */
static __IO uint32_t g_RunTime;

/* ���������ʱ���ṹ����� */
static SOFT_TMR s_tTmr[TMR_COUNT];
static SOFT_TMR_US TmrUs[TMR_COUNT_US];

static TIM_HandleTypeDef g_HeartTimHandle;


/*
*********************************************************************************************************
*                              				Private function prototypes
*********************************************************************************************************
*/
static void GeneralTimerInit( TIM_TypeDef *TIMx, TIM_HandleTypeDef *TimHandle, \
				uint32_t Prescaler, uint32_t Period );

/*
*********************************************************************************************************
*                              				Private functions
*********************************************************************************************************
*/
/*
*********************************************************************************************************
* Function Name : bsp_InitTimer
* Description	: ��ʱ����ʼ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_InitTimer(void)
{
	/* �������е������ʱ�� */
	for (uint8_t i = 0; i < TMR_COUNT; i++)
	{
		s_tTmr[i].Count = 0;
		s_tTmr[i].PreLoad = 0;
		s_tTmr[i].Flag = 0;
		s_tTmr[i].Mode = TMR_ONCE_MODE;	/* ȱʡ��1���Թ���ģʽ */
	}

	/** 
	 *	ʱ������	��84000000����ʾ������1������ۼӵ�84000000��
	 *	Ԥ��Ƶ	��84��		��ʾ������1��ֻ��Ҫ�ۼӵ�1000000��
	 *	����	��1000��	��ʾ������ֻ��Ҫ�ۼӵ�1000�����ѵ�ʱ��Ϊ1ms�������һ�θ����жϡ�						
	 */
	GeneralTimerInit(HEART_TIM, &g_HeartTimHandle, 84, 1000);
	
	ECHO(DEBUG_BSP_INIT, "Timer��ʼ�� .......... OK");
}

/*
*********************************************************************************************************
* Function Name : GeneralTimerInit
* Description	: ͨ�ö�ʱ����ʼ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void GeneralTimerInit( TIM_TypeDef *TIMx, TIM_HandleTypeDef *TimHandle, \
				uint32_t Prescaler, uint32_t Period )
{
	TimHandle->Instance = TIMx;
	
	TimHandle->Init.Period = Period - 1;
	TimHandle->Init.Prescaler = Prescaler - 1;
	TimHandle->Init.ClockDivision = 0;
	TimHandle->Init.CounterMode = TIM_COUNTERMODE_UP;
	TimHandle->Init.RepetitionCounter = 0;
	if (HAL_TIM_Base_DeInit(TimHandle) != HAL_OK)
	{
		/* Initialization Error */
		bsp_ErrorHandler();
	}
	if(HAL_TIM_Base_Init(TimHandle) != HAL_OK)
	{
		/* Initialization Error */
		bsp_ErrorHandler();
	}

	/*##-2- Start the TIM Base generation in interrupt mode ####################*/
	/* Start Channel1 */
	if(HAL_TIM_Base_Start_IT(TimHandle) != HAL_OK)
	{
		/* Starting Error */
		bsp_ErrorHandler();
	}
}

/*
*********************************************************************************************************
* Function Name : bsp_SoftTimerDec
* Description	: �����ж�ʱ��������1
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
__STATIC_INLINE void bsp_SoftTimerDec(SOFT_TMR *_tmr)
{
	if (_tmr->Count > 0)
	{
		/* �����ʱ����������1�����ö�ʱ�������־ */
		if (--_tmr->Count == 0)
		{
			_tmr->Flag = 1;

			/* ������Զ�ģʽ�����Զ���װ������ */
			if(_tmr->Mode == TMR_AUTO_MODE)
			{
				_tmr->Count = _tmr->PreLoad;
			}
		}
	}
}

/*
*********************************************************************************************************
* Function Name : HEART_TIM_IRQHandler
* Description	: �жϷ�����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void HEART_TIM_IRQHandler(void)
{
#if (uCOS_EN)
	CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();                                          
    CPU_CRITICAL_EXIT();
#endif
	
	HAL_TIM_IRQHandler(&g_HeartTimHandle);	

#if (uCOS_EN)
	OSIntExit(); 
#endif	
} 

/*
*********************************************************************************************************
* Function Name : HAL_TIM_PeriodElapsedCallback
* Description	: �����жϷ�����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == HEART_TIM)
	{	
		for (uint8_t i = 0; i < TMR_COUNT; i++)
		{
			bsp_SoftTimerDec(&s_tTmr[i]);
		}
		
		g_RunTime++;
	}
}

#if 1
/*
*********************************************************************************************************
* Function Name : bsp_DelayUS
* Description	: US��ʱ����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_DelayUS(uint32_t n)
{
    uint32_t ticks;
    uint32_t told;
    uint32_t tnow;
    uint32_t tcnt = 0;
    uint32_t reload;
       
	reload = SysTick->LOAD;                
    ticks = n * (SystemCoreClock / 1000000);	 /* ��Ҫ�Ľ����� */  
    
    tcnt = 0;
    told = SysTick->VAL;             /* �ս���ʱ�ļ�����ֵ */

    while (1)
    {
        tnow = SysTick->VAL;    
        if (tnow != told)
        {    
            /* SYSTICK��һ���ݼ��ļ����� */    
            if (tnow < told)
            {
                tcnt += told - tnow;    
            }
            /* ����װ�صݼ� */
            else
            {
                tcnt += reload - tnow + told;    
            }        
            told = tnow;

            /* ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳� */
            if (tcnt >= ticks)
            {
            	break;
            }
        }  
    }
} 

/*
*********************************************************************************************************
* Function Name : bsp_DelayMS
* Description	: MS��ʱ����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_DelayMS(uint32_t n)
{
	while(n--)
	{
		bsp_DelayUS(1000);
	}
}
#else
/*
*********************************************************************************************************
*	�� �� ��: bsp_DelayMS
*	����˵��: Ϊ���õײ������ڴ�RTOS�����������и��õļ�����
*             ר������һ������ʽ���ӳٺ������ڵײ�������ms�����ӳ���Ҫ���ڳ�ʼ����������Ӱ��ʵʱ�ԡ�
*	��    ��: n �ӳٳ��ȣ���λ1 ms
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_DelayMS(uint32_t _ulDelayTime)
{
	while (_ulDelayTime--)
	{
		bsp_DelayUS(1000);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_DelayUS
*	����˵��: �������ʱ����CPU���ڲ�����ʵ�֣�32λ������
*             	OSSchedLock(&err);
*				bsp_DelayUS(5);
*				OSSchedUnlock(&err); ����ʵ����������Ƿ���Ҫ�ӵ�������ѡ����ж�
*	��    ��: _ulDelayTime  �ӳٳ��ȣ���λ1 us
*	�� �� ֵ: ��
*   ˵    ��: 1. ��Ƶ168MHz������£�32λ������������2^32/168000000 = 25.565��
*                ����ʹ�ñ��������ӳٵĻ����ӳ���1�����¡�  
*             2. ʵ��ͨ��ʾ�������ԣ�΢���ӳٺ�����ʵ������ʵ�ʶ�����0.25us���ҵ�ʱ�䡣
*             �������ݲ���������
*             ��1��. MDK5.15���Ż��ȼ�0, ��ͬ��MDK�Ż��ȼ�����û��Ӱ�졣
*             ��2��. STM32F407IGT6
*             ��3��. ���Է�����
*				 GPIOI->BSRRL = GPIO_Pin_8;
*				 bsp_DelayUS(10);
*				 GPIOI->BSRRH = GPIO_Pin_8;
*             -------------------------------------------
*                ����                 ʵ��ִ��
*             bsp_DelayUS(1)          1.2360us
*             bsp_DelayUS(2)          2.256us
*             bsp_DelayUS(3)          3.256us
*             bsp_DelayUS(4)          4.256us
*             bsp_DelayUS(5)          5.276us
*             bsp_DelayUS(6)          6.276us
*             bsp_DelayUS(7)          7.276us
*             bsp_DelayUS(8)          8.276us
*             bsp_DelayUS(9)          9.276us
*             bsp_DelayUS(10)         10.28us
*            3. ����32λ�޷������������ȡ�Ľ���ٸ�ֵ��32λ�޷�������Ȼ������ȷ�Ļ�ȡ��ֵ��
*              ����A,B,C����32λ�޷�������
*              ���A > B  ��ôA - B = C������ܺ���⣬��ȫû������
*              ���A < B  ��ôA - B = C�� C����ֵ����0xFFFFFFFF - B + A + 1����һ��Ҫ�ر�ע�⣬�������ڱ�������
*********************************************************************************************************
*/
void bsp_DelayUS(uint32_t _ulDelayTime)
{
    uint32_t tCnt, tDelayCnt;
	uint32_t tStart;
		
	tStart = (uint32_t)CPU_TS_TmrRd();                       /* �ս���ʱ�ļ�����ֵ */
	tCnt = 0;
	tDelayCnt = _ulDelayTime * (SystemCoreClock / 1000000);	 /* ��Ҫ�Ľ����� */ 		      

	while(tCnt < tDelayCnt)
	{
		tCnt = (uint32_t)CPU_TS_TmrRd() - tStart; /* ��������У����������һ��32λ���������¼�������Ȼ������ȷ���� */	
	}
}
#endif

/*
*********************************************************************************************************
* Function Name : bsp_StartTimer
* Description	: ����һ����ʱ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_StartTimer(uint8_t _id, uint32_t _period)
{
	if (_id >= TMR_COUNT)
	{
		/* ��ӡ�����Դ�����ļ������������� */
		ECHO(DEBUG_BSP_TIMER,"Error: file %s, function %s()\r\n", __FILE__, __FUNCTION__);
		
		while(1); /* �����쳣�������ȴ����Ź���λ */
	}

	HEART_TIM_Int_Disable();  			/* ���ж� */

	s_tTmr[_id].Count = _period;		/* ʵʱ��������ֵ */
	s_tTmr[_id].PreLoad = _period;		/* �������Զ���װֵ�����Զ�ģʽ������ */
	s_tTmr[_id].Flag = 0;				/* ��ʱʱ�䵽��־ */
	s_tTmr[_id].Mode = TMR_ONCE_MODE;	/* 1���Թ���ģʽ */

	HEART_TIM_Int_Enable();  			/* ���ж� */
}

/*
*********************************************************************************************************
* Function Name : bsp_StartAutoTimer
* Description	: ����һ���Զ���ʱ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_StartAutoTimer(uint8_t _id, uint32_t _period)
{
	if (_id >= TMR_COUNT)
	{
		/* ��ӡ�����Դ�����ļ������������� */
		ECHO(DEBUG_BSP_TIMER,"Error: file %s, function %s()\r\n", __FILE__, __FUNCTION__);
		
		while(1); /* �����쳣�������ȴ����Ź���λ */
	}

	HEART_TIM_Int_Disable();  			/* ���ж� */

	s_tTmr[_id].Count = _period;		/* ʵʱ��������ֵ */
	s_tTmr[_id].PreLoad = _period;		/* �������Զ���װֵ�����Զ�ģʽ������ */
	s_tTmr[_id].Flag = 0;				/* ��ʱʱ�䵽��־ */
	s_tTmr[_id].Mode = TMR_AUTO_MODE;	/* �Զ�����ģʽ */

	HEART_TIM_Int_Enable();  			/* ���ж� */
}

/*
*********************************************************************************************************
* Function Name : bsp_StopTimer
* Description	: ֹͣһ����ʱ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_StopTimer(uint8_t _id)
{
	if (_id >= TMR_COUNT)
	{
		/* ��ӡ�����Դ�����ļ������������� */
		ECHO(DEBUG_BSP_TIMER,"Error: file %s, function %s()\r\n", __FILE__, __FUNCTION__);
		
		while(1); /* �����쳣�������ȴ����Ź���λ */
	}

	HEART_TIM_Int_Disable();  			/* ���ж� */

	s_tTmr[_id].Count = 0;				/* ʵʱ��������ֵ */
	s_tTmr[_id].Flag = 0;				/* ��ʱʱ�䵽��־ */
	s_tTmr[_id].Mode = TMR_ONCE_MODE;	/* 1���Թ���ģʽ */

	HEART_TIM_Int_Enable();  			/* ���ж� */
}

/*
*********************************************************************************************************
* Function Name : bsp_CheckTimer
* Description	: ��ⶨʱ���Ƿ�ʱ
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
uint8_t bsp_CheckTimer(uint8_t _id)
{
	if (_id >= TMR_COUNT)
	{
		return 0;
	}

	if (s_tTmr[_id].Flag == 1)
	{
		s_tTmr[_id].Flag = 0;
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
*********************************************************************************************************
* Function Name : bsp_GetRunTime
* Description	: ��ȡCPU����ʱ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
uint32_t bsp_GetRunTime(void)
{
	return g_RunTime;
}

/*
*********************************************************************************************************
* Function Name : bsp_StartTimeUS
* Description	: US����ʱ��ʼ
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_StartTimeUS(uint8_t _id)
{ 
	TmrUs[_id].startRunTime=bsp_GetRunTime();
	TmrUs[_id].startTick=SysTick->VAL;  
}

/*
*********************************************************************************************************
* Function Name : bsp_GetTimeUS
* Description	: ��ȷ��US��ʱ����������Լ�ʱ2147��,����1uS
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
uint32_t bsp_GetTimeUS(uint8_t _id)
{
	uint32_t Us;
	uint32_t endRunTime;
	uint32_t load=0;
	uint32_t endTick;
	
	Systick_Int_Disable();
	
	endTick=SysTick->VAL;
	load=SysTick->LOAD; 
	
	Systick_Int_Enable();
	
	endRunTime=bsp_GetRunTime();
	 
	Us=((endRunTime-TmrUs[_id].startRunTime)*(load+1)+TmrUs[_id].startTick-endTick)/(SystemCoreClock / 1000000L); 	 

	return Us;
}

/*
*********************************************************************************************************
*                              				MSP
*********************************************************************************************************
*/
/*
*********************************************************************************************************
* Function Name : HAL_TIM_Base_MspInit
* Description	: ��ʱ��MSP��ʼ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == HEART_TIM)
	{
		HEART_TIM_CLK_ENABLE();
		
		HAL_NVIC_SetPriority(HEART_TIM_IRQn, 0x0F, 0);
		HAL_NVIC_EnableIRQ(HEART_TIM_IRQn);
	}
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
