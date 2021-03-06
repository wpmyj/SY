/*
*********************************************************************************************************
* @file    	bsp_printf.c
* @author  	SY
* @version 	V1.0.0
* @date    	2017-1-13 09:33:05
* @IDE	 	Keil V5.22.0.0
* @Chip    	STM32F407VE
* @brief   	printf源文件
*********************************************************************************************************
* @attention
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Private Includes
*********************************************************************************************************
*/
#include "kernel.h"


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
static bool isInitOk;
static  OS_SEM  EchoSem;

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
* Function Name : ECHO_Init
* Description	: 打印输出初始化
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void ECHO_Init(void)
{
#if (OS_EN)
	
	OS_ERR     err;

	/* 用于资源共享 cnt = 1*/
    OSSemCreate((OS_SEM    *)&EchoSem,
                (CPU_CHAR  *)"EchoSem",
                (OS_SEM_CTR )1,
                (OS_ERR    *)&err);
	
	isInitOk = true;

#endif	
}

/*
*********************************************************************************************************
* Function Name : __ECHO
* Description	: 打印输出
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void __ECHO(char *format, ...)
{
	CPU_CHAR  buf_str[256];
    va_list   v_args;

	uint32_t index = 0;
	{
		char timeBuff[32] = {0};
		tTime time;
		GetSystemTime(&time);
		
		sprintf(timeBuff, "[%04d-%02d-%02d %02d:%02d:%02d] ", \
				time.usYear, time.ucMon, time.ucMday, time.ucHour, time.ucMin, time.ucSec);
		strcpy(buf_str, timeBuff);
		index = strlen(timeBuff);
	}
	
    va_start(v_args, format);
   (void)vsnprintf((char       *)&buf_str[index],
                   (size_t      ) sizeof(buf_str),
                   (char const *) format,
                                  v_args);
    va_end(v_args);

    printf("%s\r\n", buf_str);
}	

/*
*********************************************************************************************************
* Function Name : ECHO_SAFE
* Description	: 线程安全的打印信息
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void ECHO_SAFE(char *format, ...)
{
    CPU_CHAR  buf_str[256];
    va_list   v_args;
    OS_ERR     err;
	
	uint32_t index = 0;
	{
		char timeBuff[32] = {0};
		tTime time;
		GetSystemTime(&time);
		
		sprintf(timeBuff, "[%04d-%02d-%02d %02d:%02d:%02d] ", \
				time.usYear, time.ucMon, time.ucMday, time.ucHour, time.ucMin, time.ucSec);
		strcpy(buf_str, timeBuff);
		index = strlen(timeBuff);
	}

    va_start(v_args, format);
   (void)vsnprintf((char       *)&buf_str[index],
                   (size_t      ) sizeof(buf_str),
                   (char const *) format,
                                  v_args);
    va_end(v_args);
 
	if (isInitOk == true)
	{
		OSSemPend((OS_SEM *)&EchoSem,
				  (OS_TICK )0,
				  (OS_OPT  )OS_OPT_PEND_BLOCKING,
				  (CPU_TS  *)0,
				  (OS_ERR *)&err);
	}

    printf("%s\r\n", buf_str);

	if (isInitOk == true)
	{
		OSSemPost((OS_SEM *)&EchoSem,
				  (OS_OPT  )OS_OPT_POST_1,
				  (OS_ERR *)&err);
	}
}

/*
*********************************************************************************************************
* Function Name : bsp_fputc
* Description	: fputc处理
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
__weak void bsp_fputc( uint8_t ch )
{
	;
}

/*
*********************************************************************************************************
* Function Name : fputc
* Description	: 重定义putc函数，这样可以使用printf函数从串口1打印输出
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
int fputc(int ch, FILE *f)
{
	#if 1	/* 将需要printf的字符通过串口中断FIFO发送出去，printf函数会立即返回 */
		ComSend(DEBUG_COM,(uint8_t *)&ch,1);
		bsp_fputc(ch);
	
		return ch;
	#else	/* 采用阻塞方式发送每个字符,等待数据发送完毕 */
		/* 写一个字节到USART1 */
		USART_SendData(USART1, (uint8_t) ch);
		/* 等待发送结束 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
		{
			;
		}
		return ch;
	#endif
}

/*
*********************************************************************************************************
*	函 数 名: fgetc
*	功能说明: 重定义getc函数，这样可以使用getchar函数从串口1输入数据
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
int fgetc(FILE *f)
{
#if 1	/* 从串口接收FIFO中取1个数据, 只有取到数据才返回 */
	uint8_t ucData;

	while(ComGet(DEBUG_COM, &ucData) == 0)
	{
		BSP_OS_TimeDlyMs(100);
	}

	return ucData;
#else
	/* 等待串口1输入数据 */
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

	return (int)USART_ReceiveData(USART1);
#endif
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
