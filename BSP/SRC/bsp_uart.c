/*
*********************************************************************************************************
* @file    	bsp_uart.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-10-17 13:05:46
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	����ģ��Դ�ļ�
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
#include "SeqQueue.h"

/*
*********************************************************************************************************
*                              				Private define
*********************************************************************************************************
*/
#if 0
	#define	USART1_TX_GPIO_CLK_ENABLE()			__HAL_RCC_GPIOB_CLK_ENABLE()
	#define	USART1_RX_GPIO_CLK_ENABLE()			__HAL_RCC_GPIOB_CLK_ENABLE()		
	#define USART1_CLK_ENABLE()					__HAL_RCC_USART1_CLK_ENABLE()
	#define USART1_TX_AF						GPIO_AF7_USART1
	#define USART1_RX_AF						GPIO_AF7_USART1
	#define	USART1_TX_GPIO_PORT 				GPIOB
	#define	USART1_TX_GPIO_PIN					GPIO_PIN_6
	#define	USART1_RX_GPIO_PORT 				GPIOB
	#define	USART1_RX_GPIO_PIN					GPIO_PIN_7
#else
	#define	USART1_TX_GPIO_CLK_ENABLE()			__HAL_RCC_GPIOA_CLK_ENABLE()
	#define	USART1_RX_GPIO_CLK_ENABLE()			__HAL_RCC_GPIOA_CLK_ENABLE()
	#define USART1_CLK_ENABLE()					__HAL_RCC_USART1_CLK_ENABLE()
	#define USART1_TX_AF						GPIO_AF7_USART1
	#define USART1_RX_AF						GPIO_AF7_USART1
	#define	USART1_TX_GPIO_PORT 				GPIOA
	#define	USART1_TX_GPIO_PIN					GPIO_PIN_9
	#define	USART1_RX_GPIO_PORT 				GPIOA
	#define	USART1_RX_GPIO_PIN					GPIO_PIN_10
#endif

#if 0
	#define	USART6_TX_GPIO_CLK_ENABLE()			__HAL_RCC_GPIOG_CLK_ENABLE()
	#define	USART6_RX_GPIO_CLK_ENABLE()			__HAL_RCC_GPIOG_CLK_ENABLE()
	#define USART6_CLK_ENABLE()					__HAL_RCC_USART6_CLK_ENABLE()
	#define USART6_TX_AF						GPIO_AF8_USART6
	#define USART6_RX_AF						GPIO_AF8_USART6
	#define	USART6_TX_GPIO_PORT 				GPIOG
	#define	USART6_TX_GPIO_PIN					GPIO_PIN_14
	#define	USART6_RX_GPIO_PORT 				GPIOG
	#define	USART6_RX_GPIO_PIN					GPIO_PIN_9
#else
	#define	USART6_TX_GPIO_CLK_ENABLE()			__HAL_RCC_GPIOC_CLK_ENABLE()
	#define	USART6_RX_GPIO_CLK_ENABLE()			__HAL_RCC_GPIOC_CLK_ENABLE()
	#define USART6_CLK_ENABLE()					__HAL_RCC_USART6_CLK_ENABLE()
	#define USART6_TX_AF						GPIO_AF8_USART6
	#define USART6_RX_AF						GPIO_AF8_USART6
	#define	USART6_TX_GPIO_PORT 				GPIOC
	#define	USART6_TX_GPIO_PIN					GPIO_PIN_6
	#define	USART6_RX_GPIO_PORT 				GPIOC
	#define	USART6_RX_GPIO_PIN					GPIO_PIN_7
#endif

/*
*********************************************************************************************************
*                              				Private typedef
*********************************************************************************************************
*/
/* �����豸�ṹ�� */
typedef struct
{
	USART_TypeDef *uart;			/* STM32�ڲ������豸ָ�� */
	UART_HandleTypeDef *handle;		/* ��� */
	SEQUEUE_TypeDef sendQueue;		/* ���Ͷ��� */
	SEQUEUE_TypeDef receiveQueue;	/* ���ն��� */
	uint8_t receiveByte;			/* �����ֽ� */
}UART_T;  
 


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
/* ����ÿ�����ڽṹ����� */
#if UART1_FIFO_EN == 1
	static UART_T g_tUart1;
	UART_HandleTypeDef g_Uart1Handle;
	static uint8_t g_TxBuf1[UART1_TX_BUF_SIZE];		/* ���ͻ����� */
	static uint8_t g_RxBuf1[UART1_RX_BUF_SIZE];		/* ���ջ����� */
#endif

#if UART2_FIFO_EN == 1
	static UART_T g_tUart2;
	UART_HandleTypeDef g_Uart2Handle;
	static uint8_t g_TxBuf2[UART2_TX_BUF_SIZE];		/* ���ͻ����� */
	static uint8_t g_RxBuf2[UART2_RX_BUF_SIZE];		/* ���ջ����� */
#endif

#if UART3_FIFO_EN == 1
	static UART_T g_tUart3;
	UART_HandleTypeDef g_Uart3Handle;
	static uint8_t g_TxBuf3[UART3_TX_BUF_SIZE];		/* ���ͻ����� */
	static uint8_t g_RxBuf3[UART3_RX_BUF_SIZE];		/* ���ջ����� */
#endif

#if UART4_FIFO_EN == 1
	static UART_T g_tUart4;
	UART_HandleTypeDef g_Uart4Handle;
	static uint8_t g_TxBuf4[UART4_TX_BUF_SIZE];		/* ���ͻ����� */
	static uint8_t g_RxBuf4[UART4_RX_BUF_SIZE];		/* ���ջ����� */
#endif

#if UART5_FIFO_EN == 1
	static UART_T g_tUart5;
	UART_HandleTypeDef g_Uart5Handle;
	static uint8_t g_TxBuf5[UART5_TX_BUF_SIZE];		/* ���ͻ����� */
	static uint8_t g_RxBuf5[UART5_RX_BUF_SIZE];		/* ���ջ����� */
#endif

#if UART6_FIFO_EN == 1
	static UART_T g_tUart6;
	UART_HandleTypeDef g_Uart6Handle;
	static uint8_t g_TxBuf6[UART6_TX_BUF_SIZE];		/* ���ͻ����� */
	static uint8_t g_RxBuf6[UART6_RX_BUF_SIZE];		/* ���ջ����� */
#endif


/*
*********************************************************************************************************
*                              				Private function prototypes
*********************************************************************************************************
*/
static UART_T *ComToUart(COM_PORT_E _ucPort);


/*
*********************************************************************************************************
*                              				Private functions
*********************************************************************************************************
*/
/*
*********************************************************************************************************
* Function Name : UartVarInit
* Description	: ��ʼ��������صı���
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void UartVarInit(void)
{
#if UART1_FIFO_EN == 1
	g_tUart1.uart = USART1;						
	g_tUart1.handle = &g_Uart1Handle;
	CreateSeqQueue(&g_tUart1.sendQueue, g_TxBuf1, UART1_TX_BUF_SIZE);
	CreateSeqQueue(&g_tUart1.receiveQueue, g_RxBuf1, UART1_RX_BUF_SIZE);
	HAL_UART_Receive_IT(&g_Uart1Handle, &g_tUart1.receiveByte, 1);
#endif

#if UART2_FIFO_EN == 1
	g_tUart2.uart = USART2;						
	g_tUart2.handle = &g_Uart2Handle;
	CreateSeqQueue(&g_tUart2.sendQueue,g_TxBuf2,UART2_TX_BUF_SIZE);
	CreateSeqQueue(&g_tUart2.receiveQueue,g_RxBuf2,UART2_RX_BUF_SIZE);
	HAL_UART_Receive_IT(&g_Uart2Handle, &g_tUart2.receiveByte, 1);
#endif

#if UART3_FIFO_EN == 1
	g_tUart3.uart = USART3;						
	g_tUart3.handle = &g_Uart3Handle;
	CreateSeqQueue(&g_tUart3.sendQueue,g_TxBuf3,UART3_TX_BUF_SIZE);
	CreateSeqQueue(&g_tUart3.receiveQueue,g_RxBuf3,UART3_RX_BUF_SIZE);
	HAL_UART_Receive_IT(&g_Uart3Handle, &g_tUart3.receiveByte, 1);
#endif

#if UART4_FIFO_EN == 1
	g_tUart4.uart = UART4;			
	g_tUart4.handle = &g_Uart4Handle;
	CreateSeqQueue(&g_tUart4.sendQueue,g_TxBuf4,UART4_TX_BUF_SIZE);
	CreateSeqQueue(&g_tUart4.receiveQueue,g_RxBuf4,UART4_RX_BUF_SIZE);
	HAL_UART_Receive_IT(&g_Uart4Handle, &g_tUart4.receiveByte, 1);
#endif

#if UART5_FIFO_EN == 1
	g_tUart5.uart = UART5;				
	g_tUart5.handle = &g_Uart5Handle;
	CreateSeqQueue(&g_tUart5.sendQueue,g_TxBuf5,UART5_TX_BUF_SIZE);
	CreateSeqQueue(&g_tUart5.receiveQueue,g_RxBuf5,UART5_RX_BUF_SIZE);
	HAL_UART_Receive_IT(&g_Uart5Handle, &g_tUart5.receiveByte, 1);
#endif

#if UART6_FIFO_EN == 1
	g_tUart6.uart = USART6;					
	g_tUart6.handle = &g_Uart6Handle;
	CreateSeqQueue(&g_tUart6.sendQueue,g_TxBuf6,UART6_TX_BUF_SIZE);
	CreateSeqQueue(&g_tUart6.receiveQueue,g_RxBuf6,UART6_RX_BUF_SIZE);
	HAL_UART_Receive_IT(&g_Uart6Handle, &g_tUart6.receiveByte, 1);
#endif
}

/*
*********************************************************************************************************
* Function Name : UartHardInit
* Description	: ���ô��ڵ�Ӳ������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void UartHardInit(void)
{
#if UART1_FIFO_EN == 1
	g_Uart1Handle.Instance = USART1;
	
	g_Uart1Handle.Init.BaudRate     = UART1_BAUD;
	g_Uart1Handle.Init.WordLength   = UART_WORDLENGTH_8B;
	g_Uart1Handle.Init.StopBits     = UART_STOPBITS_1;
	g_Uart1Handle.Init.Parity       = UART_PARITY_NONE;
	g_Uart1Handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
	g_Uart1Handle.Init.Mode         = UART_MODE_TX_RX;
	if(HAL_UART_DeInit(&g_Uart1Handle) != HAL_OK)
	{
		bsp_ErrorHandler();
	}  
	if(HAL_UART_Init(&g_Uart1Handle) != HAL_OK)
	{
		bsp_ErrorHandler();
	}
#endif
	
#if UART6_FIFO_EN == 1
	g_Uart6Handle.Instance = USART6;
	
	g_Uart6Handle.Init.BaudRate     = UART6_BAUD;
	g_Uart6Handle.Init.WordLength   = UART_WORDLENGTH_8B;
	g_Uart6Handle.Init.StopBits     = UART_STOPBITS_1;
	g_Uart6Handle.Init.Parity       = UART_PARITY_NONE;
	g_Uart6Handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
	g_Uart6Handle.Init.Mode         = UART_MODE_TX_RX;
	g_Uart6Handle.Init.OverSampling = UART_OVERSAMPLING_16;
	if(HAL_UART_DeInit(&g_Uart6Handle) != HAL_OK)
	{
		bsp_ErrorHandler();
	}  
	if(HAL_UART_Init(&g_Uart6Handle) != HAL_OK)
	{
		bsp_ErrorHandler();
	}
	
#endif	
}

/*
*********************************************************************************************************
* Function Name : bsp_InitCOM
* Description	: ���ڳ�ʼ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void bsp_InitCOM(void)
{
	UartHardInit();
	UartVarInit();
	ECHO(DEBUG_BSP_INIT, "==================== BSP ====================");
	ECHO(DEBUG_BSP_INIT, "USART��ʼ�� .......... OK");
}

/*
*********************************************************************************************************
* Function Name : ComToUart
* Description	: ��COM�˿ں�ת��ΪUARTָ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static UART_T *ComToUart(COM_PORT_E _ucPort)
{
	if (_ucPort == COM1)
	{
		#if UART1_FIFO_EN == 1
			return &g_tUart1;
		#else
			return 0;
		#endif
	}
	else if (_ucPort == COM2)
	{
		#if UART2_FIFO_EN == 1
			return &g_tUart2;
		#else
			return 0;
		#endif
	}
	else if (_ucPort == COM3)
	{
		#if UART3_FIFO_EN == 1
			return &g_tUart3;
		#else
			return 0;
		#endif
	}
	else if (_ucPort == COM4)
	{
		#if UART4_FIFO_EN == 1
			return &g_tUart4;
		#else
			return 0;
		#endif
	}
	else if (_ucPort == COM5)
	{
		#if UART5_FIFO_EN == 1
			return &g_tUart5;
		#else
			return 0;
		#endif
	}
	else if (_ucPort == COM6)
	{
		#if UART6_FIFO_EN == 1
			return &g_tUart6;
		#else
			return 0;
		#endif
	}
	else
	{
		/* �����κδ��� */
		return 0;
	}
}  
	
/*
*********************************************************************************************************
* Function Name : ComSend
* Description	: ��COM�ڷ���һ�����ݡ����ݷŵ����ͻ��������������أ����жϷ�������ں�̨��ɷ���
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
bool ComSend(COM_PORT_E _ucPort, uint8_t *_ucaBuf, uint16_t _usLen)
{
	UART_T *pUart = ComToUart(_ucPort);
	if (pUart == 0)
	{		
		return false;
	}
	
	/**
	 *	ʹ�� HAL_UART_Transmit_IT() �жϷ��ͺ�����ʹ��<���ͼĴ���Ϊ��>�жϡ�
	 *	�����жϷ�������������ֻ�������δ���ͣ���������͡�
	 *	������ɺ�ʹ��<�������>�жϣ�Ȼ����ûص�����֪ͨ�û���
	 *	�û�����ѡ���ڻص������м����������ݡ�
	 */
	for (uint16_t i=0; i<_usLen; ++i)
	{
		if (PushSeqQueue(&pUart->sendQueue, &_ucaBuf[i], PushSeqQueueU8_CallBack) != STATUS_DATA_STRUCT_TRUE)
		{
			return false;
		}
	}
	
	if (pUart->handle->gState == HAL_UART_STATE_READY)
	{
		static uint8_t txData = 0;
		if (PopSeqQueue(&pUart->sendQueue, &txData, PopSeqQueueU8_CallBack) == STATUS_DATA_STRUCT_TRUE)
		{
			if (HAL_UART_Transmit_IT(pUart->handle, &txData, 1) != HAL_OK)
			{
				return false;
			}
		}
	}
	
	return true;
}

/*
*********************************************************************************************************
* Function Name : ComGet
* Description	: �Ӵ��ڻ�������ȡ1�ֽڣ��������������������ݾ���������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
bool ComGet(COM_PORT_E _ucPort, uint8_t *_pByte)
{
	UART_T *pUart = ComToUart(_ucPort);
	if (pUart == 0)
	{		
		return false;
	}
	
	if (PopSeqQueue(&pUart->receiveQueue, _pByte, PopSeqQueueU8_CallBack) == STATUS_DATA_STRUCT_TRUE)
	{
		return true;
	}
	
	return false;
}

/*
*********************************************************************************************************
* Function Name : USART1_IRQHandler��USART2_IRQHandler...USART6_IRQHandler
* Description	: USART�жϷ������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
#if UART1_FIFO_EN == 1
void USART1_IRQHandler(void)
{
#if (OS_EN)
	CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();                                          
    CPU_CRITICAL_EXIT();
#endif
	
	HAL_UART_IRQHandler(&g_Uart1Handle);
	
#if (OS_EN)
	OSIntExit(); 
#endif	
}
#endif

#if UART2_FIFO_EN == 1
void USART2_IRQHandler(void)
{
#if (OS_EN)
	CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();                                          
    CPU_CRITICAL_EXIT();
#endif
	
	HAL_UART_IRQHandler(&g_Uart2Handle);
	
#if (OS_EN)
	OSIntExit(); 
#endif		
}
#endif

#if UART3_FIFO_EN == 1
void USART3_IRQHandler(void)
{
#if (OS_EN)
	CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();                                          
    CPU_CRITICAL_EXIT();
#endif
	
	HAL_UART_IRQHandler(&g_Uart3Handle);
	
#if (OS_EN)
	OSIntExit(); 
#endif		
}
#endif

#if UART4_FIFO_EN == 1
void UART4_IRQHandler(void)
{
#if (OS_EN)
	CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();                                          
    CPU_CRITICAL_EXIT();
#endif
	
	HAL_UART_IRQHandler(&g_Uart4Handle);
	
#if (OS_EN)
	OSIntExit(); 
#endif	
}
#endif

#if UART5_FIFO_EN == 1
void UART5_IRQHandler(void)
{
#if (OS_EN)
	CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();                                          
    CPU_CRITICAL_EXIT();
#endif
	
	HAL_UART_IRQHandler(&g_Uart5Handle);
	
#if (OS_EN)
	OSIntExit(); 
#endif	
}
#endif

#if UART6_FIFO_EN == 1
void USART6_IRQHandler(void)
{
#if (OS_EN)
	CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();                                          
    CPU_CRITICAL_EXIT();
#endif
	
	HAL_UART_IRQHandler(&g_Uart6Handle);
	
#if (OS_EN)
	OSIntExit(); 
#endif	
}
#endif

/*
*********************************************************************************************************
*                              				MSP
*********************************************************************************************************
*/
/*
*********************************************************************************************************
* Function Name : HAL_USART_MspInit
* Description	: ����MSP��ʼ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void HAL_UART_MspInit(UART_HandleTypeDef *husart)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	if (husart->Instance == USART1)
	{
		/* ��ʼ��ʱ�� */
		USART1_TX_GPIO_CLK_ENABLE();
		USART1_RX_GPIO_CLK_ENABLE();
		USART1_CLK_ENABLE();
		
		/* ��ʼ��GPIO-TX */
		GPIO_InitStructure.Pin = USART1_TX_GPIO_PIN;
		GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStructure.Pull = GPIO_PULLUP;
		GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;	
		GPIO_InitStructure.Alternate = USART1_TX_AF;
		HAL_GPIO_Init(USART1_TX_GPIO_PORT, &GPIO_InitStructure);
		
		/* ��ʼ��GPIO-RX */
		GPIO_InitStructure.Pin = USART1_RX_GPIO_PIN;
		GPIO_InitStructure.Alternate = USART1_RX_AF;
		HAL_GPIO_Init(USART1_RX_GPIO_PORT, &GPIO_InitStructure);
		
		/* �������ȼ� */
		HAL_NVIC_SetPriority(USART1_IRQn, 3, 0);
		HAL_NVIC_EnableIRQ(USART1_IRQn);
	}
	
	if (husart->Instance == USART6)
	{
		/* ��ʼ��ʱ�� */
		USART6_TX_GPIO_CLK_ENABLE();
		USART6_RX_GPIO_CLK_ENABLE();
		USART6_CLK_ENABLE();
		
		/* ��ʼ��GPIO-TX */
		GPIO_InitStructure.Pin = USART6_TX_GPIO_PIN;
		GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStructure.Pull = GPIO_PULLUP;
		GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;	
		GPIO_InitStructure.Alternate = USART6_TX_AF;
		HAL_GPIO_Init(USART6_TX_GPIO_PORT, &GPIO_InitStructure);
		
		/* ��ʼ��GPIO-RX */
		GPIO_InitStructure.Pin = USART6_RX_GPIO_PIN;
		GPIO_InitStructure.Alternate = USART6_RX_AF;
		HAL_GPIO_Init(USART6_RX_GPIO_PORT, &GPIO_InitStructure);
		
		/* �������ȼ� */
		HAL_NVIC_SetPriority(USART6_IRQn, 3, 0);
		HAL_NVIC_EnableIRQ(USART6_IRQn);
	}
}

/*
*********************************************************************************************************
* Function Name : HAL_UART_TxCpltCallback
* Description	: ���ڷ�����ɻص�����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	if (UartHandle->Instance == USART1)
	{
		if (UartHandle->gState == HAL_UART_STATE_READY)
		{
			static uint8_t txData = 0;
			if (PopSeqQueue(&g_tUart1.sendQueue, &txData, PopSeqQueueU8_CallBack) == STATUS_DATA_STRUCT_TRUE)
			{
				HAL_UART_Transmit_IT(UartHandle, &txData, 1);
			}
		}
	}
	
	if (UartHandle->Instance == USART6)
	{
		if (UartHandle->gState == HAL_UART_STATE_READY)
		{
			static uint8_t txData = 0;
			if (PopSeqQueue(&g_tUart6.sendQueue, &txData, PopSeqQueueU8_CallBack) == STATUS_DATA_STRUCT_TRUE)
			{
				HAL_UART_Transmit_IT(UartHandle, &txData, 1);
			}
		}
	}
}

/*
*********************************************************************************************************
* Function Name : HAL_UART_RxCpltCallback
* Description	: ���ڽ�����ɻص�����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	if (UartHandle->Instance == USART1)
	{
		if (PushSeqQueue(&g_tUart1.receiveQueue, &g_tUart1.receiveByte, PushSeqQueueU8_CallBack) == STATUS_DATA_STRUCT_TRUE)
		{
			HAL_UART_Receive_IT(&g_Uart1Handle, &g_tUart1.receiveByte, 1);
		}
	}
	
	if (UartHandle->Instance == USART6)
	{
		if (PushSeqQueue(&g_tUart6.receiveQueue, &g_tUart6.receiveByte, PushSeqQueueU8_CallBack) == STATUS_DATA_STRUCT_TRUE)
		{
			HAL_UART_Receive_IT(&g_Uart6Handle, &g_tUart6.receiveByte, 1);
		}
	}
}



/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
