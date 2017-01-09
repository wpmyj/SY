/*
*********************************************************************************************************
* @file    	LinkSingleList.c
* @author  	SY
* @version 	V1.1.0
* @date    	2017-1-9 08:50:46
* @IDE	 	Keil V5.22.0.0
* @Chip    	STM32F407VE
* @brief   	��������Դ�ļ�
*********************************************************************************************************
* @attention
*		��������һ����ʽ��ȡ�����ݽṹ����һ���ַ����Ĵ洢��Ԫ������Ա��е�����Ԫ�ء�
*   �����е��������Խڵ�����ʾ�ģ�ÿ���ڵ��������ֹ��ɣ�һ���������򣬴洢����ֵ��
*	��һ����ָ���򣬴洢ָ����һ���ڵ��ָ�롣
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
#include "linkSingleList.h"


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
* Function Name : CreateLinkSingleList
* Description	: ������������(�ڴ�ռ����ڲ��ṩ����̬�ڴ�)
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
LINK_SINGLE_LIST_TypeDef *CreateLinkSingleList( void )
{
	/* ����ͷ�ڵ㣬ͷָ�����ⲿ�ṩ */
	LINK_SINGLE_LIST_TypeDef *pHead = (LINK_SINGLE_LIST_TypeDef *)new(sizeof(LINK_SINGLE_LIST_TypeDef));
	if (pHead == NULL)
	{
		return NULL;
	}
	
	pHead->data = NULL;
	pHead->next = NULL;

	return pHead;
}

/*
*********************************************************************************************************
* Function Name : LinkSingleListIsEmpty
* Description	: ���������Ƿ�Ϊ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM LinkSingleListIsEmpty( LINK_SINGLE_LIST_TypeDef *pHead )
{
	if (pHead == NULL)
	{
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	if (pHead->next == NULL)
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
* Function Name : GetLinkSingleListLenth
* Description	: ��ȡ����������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
uint32_t GetLinkSingleListLenth( LINK_SINGLE_LIST_TypeDef *pHead )
{
	uint32_t lenth = 0;
	
#if (OS_EN)
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
#endif
	
	if (pHead != NULL)
	{
		LINK_SINGLE_LIST_TypeDef *pNode = pHead;
		
		while (pNode->next != NULL)
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
* Function Name : ClearLinkSingleList
* Description	: ��յ�������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void ClearLinkSingleList( LINK_SINGLE_LIST_TypeDef *pHead )
{
#if (OS_EN)
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
#endif	
	
	if (pHead != NULL)
	{
		while (pHead->next != NULL)
		{
			LINK_SINGLE_LIST_TypeDef *oldNode = pHead->next;
			pHead->next = oldNode->next;
			oldNode->next = NULL;
			
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
* Function Name : DestoryLinkSingleList
* Description	: ���ٵ�������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void DestoryLinkSingleList( LINK_SINGLE_LIST_TypeDef **pHead )
{
	LINK_SINGLE_LIST_TypeDef *this = *pHead;

#if (OS_EN)
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
#endif
	
	if (this != NULL)
	{
		ClearLinkSingleList(this);
		
		/* �ͷ�ջ���ڵ� */
		delete(this);
		
		*pHead = NULL;
	}
	
#if (OS_EN)
	CPU_CRITICAL_EXIT();
#endif
}

/*
*********************************************************************************************************
* Function Name : InsertLinkSingleList
* Description	: �����������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM InsertLinkSingleList( LINK_SINGLE_LIST_TypeDef *pHead, void *dataIn, uint32_t dataSize,\
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
	
	LINK_SINGLE_LIST_TypeDef *prevNode = pHead;
	{
		LINK_SINGLE_LIST_TypeDef *pNode = pHead->next;	
		while (pNode != NULL)
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
	LINK_SINGLE_LIST_TypeDef *pNode = (LINK_SINGLE_LIST_TypeDef *)new(sizeof(LINK_SINGLE_LIST_TypeDef));
	if (pNode == NULL)
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif
		
		return STATUS_DATA_STRUCT_FALSE;
	}
	pNode->next = prevNode->next;	
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
* Function Name : DeleteLinkSingleList
* Description	: ��������ɾ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM DeleteLinkSingleList( LINK_SINGLE_LIST_TypeDef *pHead,\
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
	LINK_SINGLE_LIST_TypeDef *prevNode = pHead;
	{
		LINK_SINGLE_LIST_TypeDef *pNode = pHead->next;	
		while (pNode != NULL)
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
	
	LINK_SINGLE_LIST_TypeDef *oldNode = prevNode->next;	
	prevNode->next = oldNode->next;
	oldNode->next = NULL;

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
* Function Name : PushLinkSingleList
* Description	: ѹ�뵥������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM PushLinkSingleList( LINK_SINGLE_LIST_TypeDef *pHead, void *dataIn, uint32_t dataSize,\
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
	LINK_SINGLE_LIST_TypeDef *pNode = pHead->next;	
	while (pNode != NULL)
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
* Function Name : PopLinkSingleList
* Description	: ������������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM PopLinkSingleList( LINK_SINGLE_LIST_TypeDef *pHead, void *dataOut, uint32_t dataSize,\
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
	LINK_SINGLE_LIST_TypeDef *pNode = pHead->next;	
	while (pNode != NULL)
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
* Function Name : ReverseLinkSingleList
* Description	: ����������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM ReverseLinkSingleList( LINK_SINGLE_LIST_TypeDef *pHead )
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
	
	if (pHead->next == NULL)
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif	
		
		return STATUS_DATA_STRUCT_TRUE;
	}
	
	LINK_SINGLE_LIST_TypeDef *pNode1 = pHead->next;
	LINK_SINGLE_LIST_TypeDef *pNode2 = pNode1->next;
	
	pNode1->next = NULL;
	pNode1 = pNode2;
	pNode2 = pNode2->next;
	while (pNode1 != NULL)
	{
		pNode1->next = pHead->next;
		pHead->next = pNode1;
		
		pNode1 = pNode2;
		pNode2 = pNode2->next;
	}
	
#if (OS_EN)
	CPU_CRITICAL_EXIT();
#endif
	
	return STATUS_DATA_STRUCT_TRUE;
}

/*
*********************************************************************************************************
* Function Name : TraverseLinkSingleList
* Description	: ������������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM TraverseLinkSingleList( LINK_SINGLE_LIST_TypeDef *pHead,\
		void (*show_CallBack)( void *nodeData ) )
{
	LINK_SINGLE_LIST_TypeDef *pNode = pHead;
	
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
	
	while (pNode->next != NULL)
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

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
