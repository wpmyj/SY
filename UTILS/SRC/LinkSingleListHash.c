/*
*********************************************************************************************************
* @file    	LinkSingleListHash.c
* @author  	SY
* @version 	V1.1.0
* @date    	2017-1-9 08:50:46
* @IDE	 	Keil V5.22.0.0
* @Chip    	STM32F407VE
* @brief   	��ʽ��ϣ��Դ�ļ�
*********************************************************************************************************
* @attention
*		��ϣ����Կ��ٲ��ҵ�Ŀ���ֵ��Ӧ��Ԫ�أ���������ڹ�ϣ��ͻ������£�ʱ�临�Ӷȿ��ԴﵽO(1)��
*	ԭ��
*	��1������һ���ı�����򣬽�Ŀ���ֵ���±���Ϊ��ϣ���ֵ���������ݴ洢�ڹ�ϣ���ֵ��Ӧ��λ�á�
*	��2������Ĺ��̿������Ϊ�����㷨��һ�������ֱ�ӵ�ַ����������������
*	��3������Ŀ���ֵʱ����Ȼͨ�������㷨��Ŀ���ֵ����Ϊ��ϣ���ֵ�����ҹ�ϣ����ȡ�洢�����ݡ�
*	��4��ͨ�������㷨�õ��Ĺ�ϣ���ֵ�����ܻ�����ظ������Ҳ���ǹ�ϣ��ͻ��
*		һ������ͻ�ķ��������ŵ�ַ��������ַ����
*	��5������ͨ������ַ�������ͻ����������Ϊ������ͨ����ϣ���������ϣ���ֵ����ȡ�ü�ֵ��Ӧ��ԭʼ���ݣ�
*		�����а����Ѵ洢�ļ�ֵ�������ֵ��Ϊ�ռ�Ϊ�г�ͻ������Ѱ����һ����ַ�洢�����ݡ�����ַ��ͨ��
*		���г�ͻ�Ľڵ��������½ڵ㣬�γ�һ����������ͻ��
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
#include "LinkSingleListHash.h"


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
* Function Name : InitLinkSingleListHashTable
* Description	: ��ʼ����ʽ��ϣ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void InitLinkSingleListHashTable( LINK_SINGLE_LIST_HASH_TABLE_TypeDef *hashPtr,\
		LINK_SINGLE_LIST_HASH_TABLE_NODE_TypeDef **pBase, uint32_t lenth )
{
	hashPtr->basePtr = pBase;
	hashPtr->maxLenth = lenth;
	
	memset(pBase, NULL, lenth*sizeof(LINK_SINGLE_LIST_HASH_TABLE_NODE_TypeDef *));
}

/*
*********************************************************************************************************
* Function Name : SearchLinkSingleListHashTable
* Description	: ������ʽ��ϣ��
* Input			: hashPtr����ϣ����; nodePtr���ڵ�; keyPtr���ؼ��ֵ�ַ; 
*				: getHashKey_CallBack����Ŀ���ֵת��Ϊ��ϣ���ֵ
*				: matchHashKey_CallBack��Ŀ���ֵ���ϣ��洢�ļ�ֵ�Ƿ�ƥ��
*				: nextHashKey_CallBack����ȡ��һ�����õĹ�ϣ���ֵ
* Output		: nodePtr������ҵ�ƥ���ֵ������ƥ��ڵ㡣���򷵻����һ����Ч�ڵ㡣
* Return		: None
*********************************************************************************************************
*/
STATUS_HASH_ENUM SearchLinkSingleListHashTable( LINK_SINGLE_LIST_HASH_TABLE_TypeDef *hashPtr,\
		LINK_SINGLE_LIST_HASH_TABLE_NODE_TypeDef **nodePtr, void *keyPtr,\
		uint32_t (*getHashKey_CallBack)( LINK_SINGLE_LIST_HASH_TABLE_TypeDef *hashPtr, void *keyPtr ),\
		bool (*matchHashKey_CallBack)( LINK_SINGLE_LIST_HASH_TABLE_NODE_TypeDef *base, void *keyPtr ) )
{
#if (OS_EN)
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
#endif	
	
	if ((getHashKey_CallBack == NULL) || (matchHashKey_CallBack == NULL))
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif	
		
		return STATUS_HASH_UNDEFINED;
	}
	
	uint32_t hashKey = getHashKey_CallBack(hashPtr, keyPtr);	
	*nodePtr = hashPtr->basePtr[hashKey];	
	while (1)
	{
		if ((*nodePtr) == NULL)
		{
		#if (OS_EN)
			CPU_CRITICAL_EXIT();
		#endif	
			
			return STATUS_HASH_FALSE;
		}
		else if (matchHashKey_CallBack((*nodePtr), keyPtr) == true)
		{
		#if (OS_EN)
			CPU_CRITICAL_EXIT();
		#endif	
			
			return STATUS_HASH_TRUE;
		}
		else if ((*nodePtr)->next == NULL)
		{
		#if (OS_EN)
			CPU_CRITICAL_EXIT();
		#endif	
			
			return STATUS_HASH_FALSE;
		}
		else
		{
			*nodePtr = (*nodePtr)->next;
		}
	}
}

/*
*********************************************************************************************************
* Function Name : InsertLinkSingleListHashTable
* Description	: ������ʽ��ϣ��
* Input			: hashPtr����ϣ����; nodePtr:�ڵ�
*				: getHashKey_CallBack����Ŀ���ֵת��Ϊ��ϣ���ֵ
*				: matchHashKey_CallBack��Ŀ���ֵ���ϣ��洢�ļ�ֵ�Ƿ�ƥ��
*				: insertHashValue_CallBack���ڹ�ϣ���в����½ڵ�
* Output		: None
* Return		: None
*********************************************************************************************************
*/
STATUS_HASH_ENUM InsertLinkSingleListHashTable( LINK_SINGLE_LIST_HASH_TABLE_TypeDef *hashPtr, void *keyPtr, void *valuePtr,\
		uint32_t (*getHashKey_CallBack)( LINK_SINGLE_LIST_HASH_TABLE_TypeDef *hashPtr, void *keyPtr ),\
		bool (*matchHashKey_CallBack)( LINK_SINGLE_LIST_HASH_TABLE_NODE_TypeDef *base, void *keyPtr ) )
{
#if (OS_EN)
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
#endif	
	
	if ((getHashKey_CallBack == NULL) || (matchHashKey_CallBack == NULL))
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif	
		
		return STATUS_HASH_UNDEFINED;
	}
	
	LINK_SINGLE_LIST_HASH_TABLE_NODE_TypeDef tempNode;
	LINK_SINGLE_LIST_HASH_TABLE_NODE_TypeDef *lastNodePtr = &tempNode;
	STATUS_HASH_ENUM status = SearchLinkSingleListHashTable(hashPtr, &lastNodePtr, keyPtr,\
								getHashKey_CallBack, matchHashKey_CallBack);
	
	if (status == STATUS_HASH_FALSE)
	{
		LINK_SINGLE_LIST_HASH_TABLE_NODE_TypeDef *newNodePtr = \
			(LINK_SINGLE_LIST_HASH_TABLE_NODE_TypeDef *)new(sizeof(LINK_SINGLE_LIST_HASH_TABLE_NODE_TypeDef));
		if (newNodePtr == NULL)
		{
		#if (OS_EN)
			CPU_CRITICAL_EXIT();
		#endif	
			
			return STATUS_HASH_FALSE;
		}
		
		newNodePtr->key = keyPtr;
		newNodePtr->value = valuePtr;
		newNodePtr->next = NULL;
		
		/* ͷ���Ϊ�գ����½�����ͷ��� */	
		if (lastNodePtr == NULL)
		{		
			uint32_t hashKey = getHashKey_CallBack(hashPtr, keyPtr);	
			hashPtr->basePtr[hashKey] = newNodePtr;
		}
		else
		{		
			lastNodePtr->next = newNodePtr;
		}
		
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif
		
		return STATUS_HASH_TRUE;
	}
	else if (status == STATUS_HASH_TRUE)
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif	
		
		return STATUS_HASH_FALSE;
	}
	else
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif	
		
		return STATUS_HASH_UNDEFINED;
	}
}

/*
*********************************************************************************************************
* Function Name : DeleteLinkSingleListHashTableNode
* Description	: ɾ����ʽ��ϣ��ڵ�
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
STATUS_HASH_ENUM DeleteLinkSingleListHashTableNode( LINK_SINGLE_LIST_HASH_TABLE_TypeDef *hashPtr, void *keyPtr,\
		uint32_t (*getHashKey_CallBack)( LINK_SINGLE_LIST_HASH_TABLE_TypeDef *hashPtr, void *keyPtr ),\
		bool (*matchHashKey_CallBack)( LINK_SINGLE_LIST_HASH_TABLE_NODE_TypeDef *base, void *keyPtr ) )
{
#if (OS_EN)
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
#endif	
	
	if ((getHashKey_CallBack == NULL) || (matchHashKey_CallBack == NULL) )
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif	
		
		return STATUS_HASH_UNDEFINED;
	}
	
	uint32_t hashKey = getHashKey_CallBack(hashPtr, keyPtr);
	LINK_SINGLE_LIST_HASH_TABLE_NODE_TypeDef *nodePtr = hashPtr->basePtr[hashKey];
	LINK_SINGLE_LIST_HASH_TABLE_NODE_TypeDef *prevNodePtr = nodePtr;
	while (1)
	{
		if (nodePtr == NULL)
		{
		#if (OS_EN)
			CPU_CRITICAL_EXIT();
		#endif	
			
			return STATUS_HASH_FALSE;
		}
		else if (matchHashKey_CallBack(nodePtr, keyPtr) == true)
		{
			/* ͷ��� */
			if (nodePtr == hashPtr->basePtr[hashKey])
			{
				hashPtr->basePtr[hashKey] = nodePtr->next;
			}
			else
			{
				prevNodePtr->next = nodePtr->next;
			}
			delete(nodePtr);
			nodePtr = NULL;
			
		#if (OS_EN)
			CPU_CRITICAL_EXIT();
		#endif		
			
			return STATUS_HASH_TRUE;
		}
		else
		{
			prevNodePtr = nodePtr;
			nodePtr = nodePtr->next;
		}
	}
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
