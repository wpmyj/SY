/*
*********************************************************************************************************
* @file    	SeqQueue.c
* @author  	SY
* @version 	V1.1.0
* @date    	2017-1-9 08:50:46
* @IDE	 	Keil V5.22.0.0
* @Chip    	STM32F407VE
* @brief   	˳�����Դ�ļ�
*********************************************************************************************************
* @attention
*	���������������˽��в��������ɾ�����������Ա�������д��������һ�˳�Ϊ����β����
* 	�������ɾ��������һ�˳�Ϊ����ͷ���������Ա���û��Ԫ��ʱ����Ϊ���նӡ����ص㣺�Ƚ��ȳ���FIFO����
* ---------------------------------------------------------
* �汾��V1.0.1 	�޸��ˣ�SY		�޸����ڣ�2016-9-1 09:59:09	
* 
* 1���޸Ķ���ͷβ˳�򣬽���ͷ�����룬β��ɾ�����޸�Ϊ��β�����룬ͷ��ɾ����!
* -------------------------------------------------------------------------------------------------------
* �汾��V1.0.2 	�޸��ˣ�SY		�޸����ڣ�2016-9-12 09:12:17
* 
* 1������˳����еı�����
* -------------------------------------------------------------------------------------------------------	
* ---------------------------------------------------------
* �汾��V1.1.0 	�޸��ˣ�SY		�޸����ڣ�2017-1-9 08:50:46
* 
* 1�������̰߳�ȫ������
* -------------------------------------------------------------------------------------------------------	
*
* 
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Private Includes
*********************************************************************************************************
*/
#include "utils.h"
#include "SeqQueue.h"


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

/*
*********************************************************************************************************
*                              				Private functions
*********************************************************************************************************
*/
/*
*********************************************************************************************************
* Function Name : CreateSeqQueue
* Description	: ����˳�����(�ڴ�ռ����ⲿ�ṩ����̬�ڴ�)
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void CreateSeqQueue( SEQUEUE_TypeDef *queuePtr, void *pBase, uint32_t lenth )
{
	queuePtr->basePtr = pBase;
	queuePtr->maxLenth = lenth;
	queuePtr->front = 0;
	queuePtr->rear = 0;
}

/*
*********************************************************************************************************
* Function Name : SeqQueueIsFull
* Description	: ˳������Ƿ�Ϊ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM SeqQueueIsFull( SEQUEUE_TypeDef *queuePtr )
{
	if (((queuePtr->rear + 1) % queuePtr->maxLenth) == queuePtr->front)
	{
		return STATUS_DATA_STRUCT_TRUE;
	}
	else
	{
		return STATUS_DATA_STRUCT_FALSE;
	}
}

/*
*********************************************************************************************************
* Function Name : SeqQueueIsEmpty
* Description	: ˳������Ƿ�Ϊ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM SeqQueueIsEmpty( SEQUEUE_TypeDef *queuePtr )
{
	if (queuePtr->front == queuePtr->rear)
	{
		return STATUS_DATA_STRUCT_TRUE;
	}
	else
	{
		return STATUS_DATA_STRUCT_FALSE;
	}
}

/*
*********************************************************************************************************
* Function Name : ClearSeqQueue
* Description	: ���˳�����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void ClearSeqQueue( SEQUEUE_TypeDef *queuePtr )
{
#if (OS_EN)
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
#endif
	
	queuePtr->front = 0;
	queuePtr->rear = 0;

#if (OS_EN)
	CPU_CRITICAL_EXIT();
#endif
}

/*
*********************************************************************************************************
* Function Name : GetSeqQueueFront
* Description	: ��ȡ˳�����ͷ�ڵ�
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
uint32_t GetSeqQueueFront( SEQUEUE_TypeDef *queuePtr )
{
	return queuePtr->front;
}

/*
*********************************************************************************************************
* Function Name : GetSeqQueueRear
* Description	: ��ȡ˳�����β�ڵ�
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
uint32_t GetSeqQueueRear( SEQUEUE_TypeDef *queuePtr )
{
	return queuePtr->rear;
}

/*
*********************************************************************************************************
* Function Name : GetSeqQueueLenth
* Description	: ��ȡ˳����г���
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
uint32_t GetSeqQueueLenth( SEQUEUE_TypeDef *queuePtr )
{
	return queuePtr->maxLenth;
}

/*
*********************************************************************************************************
* Function Name : PushSeqQueue
* Description	: ˳��������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM PushSeqQueue( SEQUEUE_TypeDef *queuePtr, void *dataIn,\
		void (*push_CallBack)( void *base, uint32_t offset, void *data ) )
{
#if (OS_EN)
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
#endif	
	
	if (SeqQueueIsFull(queuePtr) == STATUS_DATA_STRUCT_TRUE)
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif	
		
		return STATUS_DATA_STRUCT_FALSE;
	}
	
	push_CallBack(queuePtr->basePtr, queuePtr->rear, dataIn);
	queuePtr->rear = (queuePtr->rear + 1) % queuePtr->maxLenth;
	
#if (OS_EN)
	CPU_CRITICAL_EXIT();
#endif
	
	return STATUS_DATA_STRUCT_TRUE;
}

/*
*********************************************************************************************************
* Function Name : PopSeqQueue
* Description	: ˳����г���
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM PopSeqQueue( SEQUEUE_TypeDef *queuePtr, void *dataOut,\
		void (*pop_CallBack)( void *base, uint32_t offset, void *data ) )
{
#if (OS_EN)
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
#endif
	
	if (SeqQueueIsEmpty(queuePtr) == STATUS_DATA_STRUCT_TRUE)
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif
		
		return STATUS_DATA_STRUCT_FALSE;
	}
	
	pop_CallBack(queuePtr->basePtr, queuePtr->front, dataOut);
	queuePtr->front = (queuePtr->front + 1) % queuePtr->maxLenth;
	
#if (OS_EN)
	CPU_CRITICAL_EXIT();
#endif
	
	return STATUS_DATA_STRUCT_TRUE;
}

/*
*********************************************************************************************************
* Function Name : GetSeqQueueElement
* Description	: ��ȡ˳�����Ԫ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM GetSeqQueueElement( SEQUEUE_TypeDef *queuePtr, void *dataOut,\
		void (*pop_CallBack)( void *base, uint32_t offset, void *data ) )
{
	SEQUEUE_TypeDef queue = *queuePtr;
	
	return PopSeqQueue(&queue,dataOut,pop_CallBack);
}

/*
*********************************************************************************************************
* Function Name : GetSeqQueueDisposeDataRate
* Description	: ��ȡ˳�����δ�����������(���ֻ�ܵ���80%����Ϊ��ѭ�����У�
*				: Ϊ�����ֶ����������пգ���һ�����ݵ�û��)
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
uint8_t GetSeqQueueDisposeDataRate( SEQUEUE_TypeDef *queuePtr )
{
	uint32_t front = queuePtr->front;
	uint32_t rear = queuePtr->rear;
	uint32_t lenth = queuePtr->maxLenth;
	uint32_t diff = 0;
	
	if (rear >= front)
	{
		diff = rear - front;
	}
	else
	{
		diff = lenth + rear - front;
	}
	
	return ((float)diff / lenth * 100);
}

/*
*********************************************************************************************************
* Function Name : TraverseSeqQueue
* Description	: ����˳�����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void TraverseSeqQueue( SEQUEUE_TypeDef *queuePtr, void *dataOut,\
		void (*pop_CallBack)( void *base, uint32_t offset, void *data ),\
		void (*show_CallBack)( void *dataOut ) )
{
	SEQUEUE_TypeDef queue = *queuePtr;

#if (OS_EN)
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
#endif
	
	while ( PopSeqQueue(&queue, dataOut, pop_CallBack) == STATUS_DATA_STRUCT_TRUE)
	{
		if (show_CallBack)
		{
			show_CallBack(dataOut);
		}
	}
	
#if (OS_EN)
	CPU_CRITICAL_EXIT();
#endif
}


/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
