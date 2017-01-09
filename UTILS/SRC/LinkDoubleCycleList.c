/*
*********************************************************************************************************
* @file    	LinkDoubleCycleList.c
* @author  	SY
* @version 	V1.1.0
* @date    	2017-1-9 08:50:46
* @IDE	 	Keil V5.22.0.0
* @Chip    	STM32F407VE
* @brief   	˫��ѭ������Դ�ļ�
*********************************************************************************************************
* @attention
*		˫������Ҳ��˫�����������һ�֣�����ÿ�����ݽ���ж�������ָ�룬�ֱ�ָ��ֱ�Ӻ�̺�ֱ��ǰ����
*	���ԣ���˫�������е�����һ����㿪ʼ�������Ժܷ���ط�������ǰ�����ͺ�̽�㡣һ�����Ƕ�����
*	˫��ѭ������
* 
* ---------------------------------------------------------
* �汾��V1.1.0 	�޸��ˣ�SY		�޸����ڣ�2017-1-9 08:50:46
* 
* 1�������̰߳�ȫ������
* -------------------------------------------------------------------------------------------------------	
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Private Includes
*********************************************************************************************************
*/
#include "utils.h"
#include "LinkDoubleCycleList.h"


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
* Function Name : CreateLinkDoubleCycleList
* Description	: ����˫��ѭ������(�ڴ�ռ����ڲ��ṩ����̬�ڴ�)
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
LINK_DOUBLE_CYCLE_LIST_TypeDef *CreateLinkDoubleCycleList( void )
{
	/* ����ͷ�ڵ㣬ͷָ�����ⲿ�ṩ */
	LINK_DOUBLE_CYCLE_LIST_TypeDef *pHead = (LINK_DOUBLE_CYCLE_LIST_TypeDef *)new(sizeof(LINK_DOUBLE_CYCLE_LIST_TypeDef));
	if (pHead == NULL)
	{
		return NULL;
	}
	
	pHead->data = NULL;
	pHead->prev = pHead;
	pHead->next = pHead;

	return pHead;
}

/*
*********************************************************************************************************
* Function Name : LinkDoubleCycleListIsEmpty
* Description	: ˫��ѭ�������Ƿ�Ϊ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM LinkDoubleCycleListIsEmpty( LINK_DOUBLE_CYCLE_LIST_TypeDef *pHead )
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
	
	if ((pHead->prev == pHead) &&\
		(pHead->next == pHead))
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif
		
		return STATUS_DATA_STRUCT_TRUE;
	}
	else
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif
		
		return STATUS_DATA_STRUCT_FALSE;
	}
}

/*
*********************************************************************************************************
* Function Name : GetLinkDoubleCycleListLenth
* Description	: ��ȡ˫��ѭ��������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
uint32_t GetLinkDoubleCycleListLenth( LINK_DOUBLE_CYCLE_LIST_TypeDef *pHead )
{
	uint32_t lenth = 0;
	
#if (OS_EN)
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
#endif
	
	if (pHead != NULL)
	{
		LINK_DOUBLE_CYCLE_LIST_TypeDef *pNode = pHead;
		
		while (pNode->next != pHead)
		{
			lenth++;
			pNode = pNode->next;
		}
	}
	
#if (OS_EN)
	CPU_CRITICAL_EXIT();
#endif
	
	return lenth;
}

/*
*********************************************************************************************************
* Function Name : ClearLinkDoubleCycleList
* Description	: ���˫��ѭ������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void ClearLinkDoubleCycleList( LINK_DOUBLE_CYCLE_LIST_TypeDef *pHead )
{
#if (OS_EN)
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
#endif
	
	if (pHead != NULL)
	{
		while (pHead->next != pHead)
		{
			LINK_DOUBLE_CYCLE_LIST_TypeDef *oldNode = pHead->next;
			pHead->next = oldNode->next;
			oldNode->next->prev = pHead;
			oldNode->next = NULL;
			oldNode->prev = NULL;
			
			if (oldNode->data != NULL)
			{
				delete(oldNode->data);
				oldNode->data = NULL;
			}
			
			delete(oldNode);
			oldNode = NULL;				
		}
	}
	
#if (OS_EN)
	CPU_CRITICAL_EXIT();
#endif
}

/*
*********************************************************************************************************
* Function Name : DestoryLinkDoubleCycleList
* Description	: ����˫��ѭ������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void DestoryLinkDoubleCycleList( LINK_DOUBLE_CYCLE_LIST_TypeDef **pHead )
{
	LINK_DOUBLE_CYCLE_LIST_TypeDef *this = *pHead;
	
#if (OS_EN)
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
#endif
	
	if (this != NULL)
	{
		ClearLinkDoubleCycleList(this);
		delete(this);		
		*pHead = NULL;
	}
	
#if (OS_EN)
	CPU_CRITICAL_EXIT();
#endif
}

/*
*********************************************************************************************************
* Function Name : InsertLinkDoubleCycleList
* Description	: ˫��ѭ���������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM InsertLinkDoubleCycleList( LINK_DOUBLE_CYCLE_LIST_TypeDef *pHead, void *dataIn, uint32_t dataSize,\
		void *matchData, bool (*match_CallBack)( void *nodeData, void *matchData ) )
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
	
	LINK_DOUBLE_CYCLE_LIST_TypeDef *prevNode = pHead;
	{
		LINK_DOUBLE_CYCLE_LIST_TypeDef *pNode = pHead->next;	
		while (pNode != pHead)
		{
			if (match_CallBack(pNode->data, matchData) == true)
			{
				break;
			}
			
			prevNode = pNode;
			pNode = pNode->next;
		}
	}
	
	/* �����½ڵ� */
	LINK_DOUBLE_CYCLE_LIST_TypeDef *pNode = (LINK_DOUBLE_CYCLE_LIST_TypeDef *)new(sizeof(LINK_DOUBLE_CYCLE_LIST_TypeDef));
	if (pNode == NULL)
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif	
		
		return STATUS_DATA_STRUCT_FALSE;
	}
	pNode->next = prevNode->next;	
	pNode->prev = prevNode;
	pNode->next->prev = pNode;
	prevNode->next = pNode;
	
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
* Function Name : DeleteLinkDoubleCycleList
* Description	: ˫��ѭ������ɾ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM DeleteLinkDoubleCycleList( LINK_DOUBLE_CYCLE_LIST_TypeDef *pHead,\
		void *matchData, bool (*match_CallBack)( void *nodeData, void *matchData ) )
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
	
	bool isFind = false;
	LINK_DOUBLE_CYCLE_LIST_TypeDef *prevNode = pHead;
	{
		LINK_DOUBLE_CYCLE_LIST_TypeDef *pNode = pHead->next;	
		while (pNode != pHead)
		{
			if (match_CallBack(pNode->data, matchData) == true)
			{
				isFind = true;
				break;
			}
			
			prevNode = pNode;
			pNode = pNode->next;
		}
	}
	
	if (isFind == false)
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif
		
		return STATUS_DATA_STRUCT_FALSE;
	}
	
	LINK_DOUBLE_CYCLE_LIST_TypeDef *oldNode = prevNode->next;	
	prevNode->next = oldNode->next;
	oldNode->next->prev = prevNode;
	oldNode->next = NULL;
	oldNode->prev = NULL;
	
	if (oldNode->data != NULL)
	{
		delete(oldNode->data);
		oldNode->data = NULL;
	}
	delete(oldNode);
	oldNode = NULL;
	
#if (OS_EN)
	CPU_CRITICAL_EXIT();
#endif
	
	return STATUS_DATA_STRUCT_TRUE;
}

/*
*********************************************************************************************************
* Function Name : PushLinkDoubleCycleList
* Description	: ѹ��˫��ѭ������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM PushLinkDoubleCycleList( LINK_DOUBLE_CYCLE_LIST_TypeDef *pHead, void *dataIn, uint32_t dataSize,\
		void *matchData, bool (*match_CallBack)( void *nodeData, void *matchData ) )
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
	
	bool isFind = false;
	LINK_DOUBLE_CYCLE_LIST_TypeDef *pNode = pHead->next;	
	while (pNode != pHead)
	{
		if (match_CallBack(pNode->data, matchData) == true)
		{
			isFind = true;
			break;
		}
		
		pNode = pNode->next;
	}

	if (isFind == false)
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif	
		
		return STATUS_DATA_STRUCT_FALSE;
	}	
	
	DATA_STRUCT_STATUS_ENUM isDataOK = STATUS_DATA_STRUCT_FALSE;
	if (pNode->data != NULL)
	{
		memcpy(pNode->data,dataIn,dataSize);
		isDataOK = STATUS_DATA_STRUCT_TRUE;
	}
	
#if (OS_EN)
	CPU_CRITICAL_EXIT();
#endif
	
	return isDataOK;
}

/*
*********************************************************************************************************
* Function Name : PopLinkDoubleCycleList
* Description	: ����˫��ѭ������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM PopLinkDoubleCycleList( LINK_DOUBLE_CYCLE_LIST_TypeDef *pHead, void *dataOut, uint32_t dataSize,\
		void *matchData, bool (*match_CallBack)( void *nodeData, void *matchData ) )
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
	
	bool isFind = false;
	LINK_DOUBLE_CYCLE_LIST_TypeDef *pNode = pHead->next;	
	while (pNode != pHead)
	{
		if (match_CallBack(pNode->data, matchData) == true)
		{
			isFind = true;
			break;
		}
		
		pNode = pNode->next;
	}

	if (isFind == false)
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif		
		
		return STATUS_DATA_STRUCT_FALSE;
	}	
	
	DATA_STRUCT_STATUS_ENUM isDataOK = STATUS_DATA_STRUCT_FALSE;
	if (pNode->data != NULL)
	{
		memcpy(dataOut,pNode->data,dataSize);
		isDataOK = STATUS_DATA_STRUCT_TRUE;
	}
	
#if (OS_EN)
	CPU_CRITICAL_EXIT();
#endif	
	
	return isDataOK;
}

/*
*********************************************************************************************************
* Function Name : TraverseNextLinkDoubleCycleList
* Description	: ������˫��ѭ������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM TraverseNextLinkDoubleCycleList( LINK_DOUBLE_CYCLE_LIST_TypeDef *pHead,\
		void (*show_CallBack)( void *nodeData ) )
{
	LINK_DOUBLE_CYCLE_LIST_TypeDef *pNode = pHead;
	
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
	
	while (pNode->next != pHead)
	{
		pNode = pNode->next;
		if (pNode->data == NULL)
		{
		#if (OS_EN)
			CPU_CRITICAL_EXIT();
		#endif
			
			return STATUS_DATA_STRUCT_FALSE;
		}
		show_CallBack(pNode->data);
	}
	
#if (OS_EN)
	CPU_CRITICAL_EXIT();
#endif
	
	return STATUS_DATA_STRUCT_TRUE;
}

/*
*********************************************************************************************************
* Function Name : TraversePrevLinkDoubleCycleList
* Description	: ��ǰ����˫��ѭ������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM TraversePrevLinkDoubleCycleList( LINK_DOUBLE_CYCLE_LIST_TypeDef *pHead,\
		void (*show_CallBack)( void *nodeData ) )
{
	LINK_DOUBLE_CYCLE_LIST_TypeDef *pNode = pHead;
	
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
	
	while (pNode->prev != pHead)
	{
		pNode = pNode->prev;
		if (pNode->data == NULL)
		{
		#if (OS_EN)
			CPU_CRITICAL_EXIT();
		#endif	
			
			return STATUS_DATA_STRUCT_FALSE;
		}
		show_CallBack(pNode->data);
	}
	
#if (OS_EN)
	CPU_CRITICAL_EXIT();
#endif
	
	return STATUS_DATA_STRUCT_TRUE;
}

/*
*********************************************************************************************************
* Function Name : SpliceLinkDoubleCycleList
* Description	: ����˫��ѭ������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM SpliceLinkDoubleCycleList( LINK_DOUBLE_CYCLE_LIST_TypeDef *pHead,\
		LINK_DOUBLE_CYCLE_LIST_TypeDef *pHeadNew )
{
#if (OS_EN)
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
#endif	
	
	if ((pHead == NULL) || (pHeadNew == NULL))
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif
		
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	if (LinkDoubleCycleListIsEmpty(pHeadNew) == STATUS_DATA_STRUCT_TRUE)
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif	
		
		return STATUS_DATA_STRUCT_FALSE;
	}
	
	LINK_DOUBLE_CYCLE_LIST_TypeDef *first = pHeadNew->next;
	LINK_DOUBLE_CYCLE_LIST_TypeDef *last = pHeadNew->prev;
	LINK_DOUBLE_CYCLE_LIST_TypeDef *at = pHead->next;
	
	pHead->next = first;
	first->prev = pHead;
	at->prev = last;
	last->next = at;
	
	/* �½�㲢δ�ͷ�ͷ����ڴ棬��Ҫ�ⲿ free() */
	pHeadNew->next = pHeadNew;
	pHeadNew->prev = pHeadNew;
	
#if (OS_EN)
	CPU_CRITICAL_EXIT();
#endif
	
	return STATUS_DATA_STRUCT_TRUE;
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
