/*
*********************************************************************************************************
* @file    	LinkQueue.c
* @author  	SY
* @version 	V1.1.0
* @date    	2017-1-9 08:50:46
* @IDE	 	Keil V5.22.0.0
* @Chip    	STM32F407VE
* @brief   	��ʽ����Դ�ļ�
*********************************************************************************************************
* @attention
*	���������������˽��в��������ɾ�����������Ա�������д��������һ�˳�Ϊ����β����
* 	�������ɾ��������һ�˳�Ϊ����ͷ���������Ա���û��Ԫ��ʱ����Ϊ���նӡ����ص㣺�Ƚ��ȳ���FIFO����
* -------------------------------------------------------------------------------------------------------
* �汾��V1.0.1 	�޸��ˣ�SY		�޸����ڣ�2016-9-12 13:08:40
* 
* 1��������ʽ���еı�����
* -------------------------------------------------------------------------------------------------------	
* ---------------------------------------------------------
* �汾��V1.1.0 	�޸��ˣ�SY		�޸����ڣ�2017-1-9 08:50:46
* 
* 1�������̰߳�ȫ������
* -------------------------------------------------------------------------------------------------------	
*
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
#include "LinkQueue.h"


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
* Function Name : CreateLinkQueue
* Description	: ������ʽ����(�ڴ�ռ����ڲ��ṩ����̬�ڴ�)
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
LINK_QUEUE_TypeDef *CreateLinkQueue( void )
{
	/* ����ͷָ�� */
	LINK_QUEUE_TypeDef *pHead = (LINK_QUEUE_TypeDef *)new(sizeof(LINK_QUEUE_TypeDef));
	if (pHead == NULL)
	{
		return NULL;
	}
	
	/* ����ͷ��� */
	LINK_QUEUE_NODE_TypeDef *pNode = (LINK_QUEUE_NODE_TypeDef *)new(sizeof(LINK_QUEUE_NODE_TypeDef));
	if (pNode == NULL)
	{
		return NULL;
	}
	
	pNode->data = NULL;
	pNode->next = NULL;
	pHead->front = pNode;
	pHead->rear = pNode;

	return pHead;
}

/*
*********************************************************************************************************
* Function Name : LinkQueueIsEmpty
* Description	: ��ʽ�����Ƿ�Ϊ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM LinkQueueIsEmpty( LINK_QUEUE_TypeDef *pHead )
{
	if (pHead == NULL)
	{
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	if (pHead->front->next == NULL)
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
* Function Name : ClearLinkQueue
* Description	: �����ʽ����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void ClearLinkQueue( LINK_QUEUE_TypeDef *pHead )
{
#if (OS_EN)
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
#endif
	
	if (pHead != NULL)
	{
		LINK_QUEUE_NODE_TypeDef *oldNode = NULL;
		
		while (pHead->front->next != NULL)
		{
			oldNode = pHead->front->next;
			pHead->front->next = oldNode->next;
			oldNode->next = NULL;
			
			if (oldNode->data != NULL)
			{
				delete(oldNode->data);
				oldNode->data = NULL;
			}
			
			delete(oldNode);
			oldNode = NULL;				
		}
		
		pHead->rear = pHead->front;
	}

#if (OS_EN)
	CPU_CRITICAL_EXIT();
#endif
}

/*
*********************************************************************************************************
* Function Name : DestoryLinkQueue
* Description	: ������ʽ����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void DestoryLinkQueue( LINK_QUEUE_TypeDef **pHead )
{
	LINK_QUEUE_TypeDef *this = *pHead;

#if (OS_EN)
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
#endif
	
	if (this != NULL)
	{
		ClearLinkQueue(this);
		
		if (this->front != NULL)
		{
			/* �ͷ�ͷ��� */
			delete(this->front);
			this->front = this->rear = NULL;
			
			/* �ͷ�ͷָ�� */
			delete(this);
			
			*pHead = NULL;
		}
	}
	
#if (OS_EN)
	CPU_CRITICAL_EXIT();
#endif
}

/*
*********************************************************************************************************
* Function Name : PushLinkQueue
* Description	: ��ʽ�������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM PushLinkQueue( LINK_QUEUE_TypeDef *pHead, void *dataIn, uint32_t dataSize )
{
#if (OS_EN)
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
#endif	

	if (pHead == NULL)
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif	
		
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	/* �����½ڵ� */
	LINK_QUEUE_NODE_TypeDef *pNode = (LINK_QUEUE_NODE_TypeDef *)new(sizeof(LINK_QUEUE_NODE_TypeDef));
	if (pNode == NULL)
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif
		
		return STATUS_DATA_STRUCT_FALSE;
	}
	pNode->next = NULL;	
	
	if (pHead->front->next == NULL)
	{
		pHead->rear = pHead->front;
	}
	pHead->rear->next = pNode;
	pHead->rear = pNode;
	
	/* �洢�û����� */
	pNode->data = (void *)new(dataSize);
	if (pNode->data == NULL)
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif	
		
		return STATUS_DATA_STRUCT_FALSE;
	}	
	memcpy(pNode->data,dataIn,dataSize);
	
#if (OS_EN)
	CPU_CRITICAL_EXIT();
#endif
	
	return STATUS_DATA_STRUCT_TRUE;
}

/*
*********************************************************************************************************
* Function Name : PopLinkQueue
* Description	: ��ʽ���г���
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM PopLinkQueue( LINK_QUEUE_TypeDef *pHead, void *dataOut, uint32_t dataSize )
{
#if (OS_EN)
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
#endif	
	
	if (pHead == NULL)
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif	
		
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	if (LinkQueueIsEmpty(pHead) == STATUS_DATA_STRUCT_TRUE)
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif	
		
		return STATUS_DATA_STRUCT_FALSE;
	}
	
	/* ����ʱ��ͷָ��ʼ��ָ��ͷ��㣬����ͷ������Ľڵ� */
	LINK_QUEUE_NODE_TypeDef *oldNode = pHead->front->next;	
	pHead->front->next = oldNode->next;
	oldNode->next = NULL;
	if (pHead->front->next == NULL)
	{
		pHead->rear = pHead->front;
	}
	
	DATA_STRUCT_STATUS_ENUM isDataOK = STATUS_DATA_STRUCT_FALSE;
	if (oldNode->data != NULL)
	{
		memcpy(dataOut,oldNode->data,dataSize);
		delete(oldNode->data);
		oldNode->data = NULL;
		isDataOK = STATUS_DATA_STRUCT_TRUE;
	}
	delete(oldNode);
	oldNode = NULL;
	
#if (OS_EN)
	CPU_CRITICAL_EXIT();
#endif	
	
	return isDataOK;
}

/*
*********************************************************************************************************
* Function Name : GetLinkQueueElement
* Description	: ��ȡ��ʽ����Ԫ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM GetLinkQueueElement( LINK_QUEUE_TypeDef *pHead, void *dataOut, uint32_t dataSize )
{
#if (OS_EN)
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
#endif	
	
	if (pHead == NULL)
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif
		
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	if (LinkQueueIsEmpty(pHead) == STATUS_DATA_STRUCT_TRUE)
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif	
		
		return STATUS_DATA_STRUCT_FALSE;
	}
	
	/* ����ʱ��ͷָ��ʼ��ָ��ͷ��㣬����ͷ������Ľڵ� */
	DATA_STRUCT_STATUS_ENUM isDataOK = STATUS_DATA_STRUCT_FALSE;
	LINK_QUEUE_NODE_TypeDef *oldNode = pHead->front->next;		
	if (oldNode->data != NULL)
	{
		memcpy(dataOut,oldNode->data,dataSize);
		isDataOK = STATUS_DATA_STRUCT_TRUE;
	}
	
#if (OS_EN)
	CPU_CRITICAL_EXIT();
#endif
	
	return isDataOK;
}

/*
*********************************************************************************************************
* Function Name : TraverseLinkQueue
* Description	: ������ʽ����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void TraverseLinkQueue( LINK_QUEUE_TypeDef *pHead, void *dataOut, uint32_t dataSize,\
		void (*show_CallBack)( void *dataOut ) )
{
	LINK_QUEUE_TypeDef queue = *pHead;
	LINK_QUEUE_NODE_TypeDef *pNode = queue.front->next;
	
#if (OS_EN)
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
#endif
	
	while (pNode != NULL)
	{
		memcpy(dataOut,pNode->data,dataSize);
		
		if (show_CallBack)
		{
			show_CallBack(dataOut);
		}
		
		pNode = pNode->next;
	}
	
#if (OS_EN)
	CPU_CRITICAL_EXIT();
#endif
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
