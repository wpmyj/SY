/*
*********************************************************************************************************
* @file    	bsp.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-10-11 15:10:28
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	板级开发驱动包源文件
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
#include "kernel.h"
#include "usbh_usr.h"
#include "ethernetif.h"
#include "file.h"
#include "emWinUtils.h"
#include "SeqQueue.h"
#include "SeqList.h"
#include "usbh_hid.h"

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
uint32_t g_KeyValue;
extern SWITCH_TASK_TypeDef g_ETH_Switch;
static CRC_HandleTypeDef hcrc;

/*
*********************************************************************************************************
*                              				Private function prototypes
*********************************************************************************************************
*/
static void bsp_InitCRC( void );
void LCD_DebugViewTask( void );
void BSP_Tick_Init (void);
void ECHO_Init(void);

/*
*********************************************************************************************************
*                              				Private functions
*********************************************************************************************************
*/
/*
*********************************************************************************************************
* Function Name : BSP_Init
* Description	: 板级驱动包初始化
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void BSP_Init( void )
{
	BSP_Tick_Init();
	
	bsp_InitCOM();
	bsp_InitTimer();
	bsp_InitI2C_Bus();
	bsp_InitSPI_Bus();
	bsp_InitFSMC_Bus();
	bsp_InitCRC();
	bsp_InitIO();
	bsp_InitPCF8563();
	bsp_InitSFlash();
	bsp_InitLCD();
	bsp_InitFatFs();
	bsp_InitNetwork();

	ECHO(DEBUG_BSP_INIT, "==================== END ====================\r\n");
}

/*
*********************************************************************************************************
* Function Name : bsp_InitCRC
* Description	: 初始化CRC(STemWin是SEGGER公司授权给ST的,初始化STemWin时，emWin会进行CRC校验,
*				: 因此，使用之前一定要使能CRC时钟)
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void bsp_InitCRC( void )
{
	hcrc.Instance = CRC;
	if (HAL_CRC_Init(&hcrc) != HAL_OK)
	{
		bsp_ErrorHandler();
	}
}

/*
*********************************************************************************************************
* Function Name : HAL_CRC_MspInit
* Description	: CRC MSP初始化
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void HAL_CRC_MspInit(CRC_HandleTypeDef* crcHandle)
{
	if(crcHandle->Instance==CRC)
	{
		__HAL_RCC_CRC_CLK_ENABLE();
	}
}

/*
*********************************************************************************************************
* Function Name : AppTaskStartUtils
* Description	: 起始任务工具类
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void  AppTaskStartUtils (void)
{
	/* 按键任务 */
	{
		g_KeyValue = bsp_74HC165_Read();
		KeyScanCycleTask(g_keyPtr);		
	}

	static __IO uint32_t count = 0;
	if (++count >= 10)
	{
		count = 0;	
		
		/* 蜂鸣器任务 */
		RepeatExecuteTaskCycle(GetBeepHandle());
		
		/* USB任务 */
		USBH_Process(&hUsbHostHS);
		
		/* 以太网任务 */
		{
			HAL_ETH_RxCpltCallback(&EthHandle);
			sys_check_timeouts();
		}

		/* 以太网开关任务 */
		SwitchTask(&g_ETH_Switch);
	}
	SystemTick_Inc(1);
}

/*
*********************************************************************************************************
* Function Name : bsp_ErrorHandler
* Description	: 错误处理
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_ErrorHandler(void)
{
  while(1)
  {
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 168000000
  *            HCLK(Hz)                       = 168000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 336
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    bsp_ErrorHandler();
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    bsp_ErrorHandler();
  }
}

/**
  * @brief  Sets the vector table location and Offset.
  * @param  NVIC_VectTab: specifies if the vector table is in RAM or FLASH memory.
  *   This parameter can be one of the following values:
  *     @arg NVIC_VectTab_RAM: Vector Table in internal SRAM.
  *     @arg NVIC_VectTab_FLASH: Vector Table in internal FLASH.
  * @param  Offset: Vector Table base offset field. This value must be a multiple of 0x200.
  * @retval None
  */
void NVIC_SetVectorTable(uint32_t NVIC_VectTab, uint32_t Offset)
{ 
  SCB->VTOR = NVIC_VectTab | (Offset & (uint32_t)0x1FFFFF80);
}

/*
*********************************************************************************************************
*                                            BSP_CPU_ClkFreq()
*
* Description : Read CPU registers to determine the CPU clock frequency of the chip.
*
* Argument(s) : none.
*
* Return(s)   : The CPU clock frequency, in Hz.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_INT32U  BSP_CPU_ClkFreq (void)
{
    CPU_INT32U  hclk_freq;

    hclk_freq = HAL_RCC_GetHCLKFreq();
    return (hclk_freq);
}

/*
*********************************************************************************************************
*                                            BSP_Tick_Init()
*
* Description : Initialize all the peripherals that required OS Tick services (OS initialized)
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/
void BSP_Tick_Init (void)
{
    CPU_INT32U  cpu_clk_freq;
    CPU_INT32U  cnts;

    cpu_clk_freq = BSP_CPU_ClkFreq();                           /* Determine SysTick reference freq.                    */

#if (OS_VERSION >= 30000u)
    cnts  = cpu_clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz;       /* Determine nbr SysTick increments.                    */
#else
    cnts  = cpu_clk_freq / (CPU_INT32U)OS_TICKS_PER_SEC;        /* Determine nbr SysTick increments.                    */
#endif

    OS_CPU_SysTickInit(cnts);                                   /* Init uC/OS periodic time src (SysTick).              */
}

/*
*********************************************************************************************************
*                                            HAL_InitTick()
*
* Description : This function has been overwritten from the STM32F4xx HAL libraries because Micrium's RTOS
*               has its own Systick initialization and because it is recomended to initialize the tick after
*               multi-tasking has started.
*
* Argument(s) : TickPriority          Tick interrupt priority.
*
* Return(s)   : HAL_OK.
*
* Caller(s)   : HAL_InitTick ()) is called automatically at the beginning of the program after reset by
*               HAL_Init() or at any time when clock is configured, by HAL_RCC_ClockConfig().
*
* Note(s)     : none.
*********************************************************************************************************
*/

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
    if (OSRunning > 0u) {                                       /*Check if multi-tasking has started.                   */
        ;
    }

    return (HAL_OK);
}

/*
*********************************************************************************************************
* Function Name : HAL_MspInit
* Description	: HAL MSP任务
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void HAL_MspInit(void)
{
	/* 使用BootLoader后，中断向量表需要重定位 */
	NVIC_SetVectorTable(FLASH_BASE, 0x20000);
	
	/* 开总中断，一定要在重定位中断向量表之后开启 */
	__enable_irq();
}

/*
*********************************************************************************************************
* Function Name : AppObjCreate
* Description	: 创建Obj
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void AppObjCreate(void)
{
	ECHO_Init();
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
