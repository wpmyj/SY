/*
*********************************************************************************************************
* @file    	SeqListHash.c
* @author  	SY
* @version 	V1.1.0
* @date    	2017-1-9 08:50:46
* @IDE	 	Keil V5.22.0.0
* @Chip    	STM32F407VE
* @brief   	˳���ϣ��Դ�ļ�
*********************************************************************************************************
* @attention
*		��ϣ����Կ��ٲ��ҵ�Ŀ���ֵ��Ӧ��Ԫ�أ���������ڹ�ϣ��ͻ������£�ʱ�临�Ӷȿ��ԴﵽO(1)��
*	ԭ��
*	��1������һ���ı�����򣬽�Ŀ���ֵ���±���Ϊ��ϣ���ֵ���������ݴ洢�ڹ�ϣ���ֵ��Ӧ��λ�á�
*	��2������Ĺ��̿������Ϊ�����㷨��һ�������ֱ�ӵ�ַ����������������
*	��3������Ŀ���ֵʱ����Ȼͨ�������㷨��Ŀ���ֵ����Ϊ��ϣ���ֵ�����ҹ�ϣ����ȡ�洢�����ݡ�
*	��4��ͨ�������㷨�õ��Ĺ�ϣ���ֵ�����ܻ�����ظ������Ҳ���ǹ�ϣ��ͻ��
*		һ������ͻ�ķ��������ŵ�ַ��������ַ����
*	��5������ͨ�����ŵ�ַ�������ͻ����������Ϊ������ͨ����ϣ���������ϣ���ֵ����ȡ�ü�ֵ��Ӧ��ԭʼ���ݣ�
*		�����а����Ѵ洢�ļ�ֵ�������ֵ��Ϊ�ռ�Ϊ�г�ͻ������Ѱ����һ����ַ�洢�����ݡ�
*		��ʽΪ��Hash(key[New]) = (Hash(key[Old]) + di) % M,(M��ʾ��ϣ��ĳ���)��
*		���ŵ�ַ��һ��ͨ���ڹ�ϣ���ֵ�����������·����ı��ֵ��
*			1��di = 1, 2, 3...M-1, ��Ϊ����̽����ɢ��
*			2��di = 1^2, -1^2, 2^2, -2^2...k^2,-k^2(k<=M/2)����Ϊ����̽����ɢ�С�
*			3��diȡ�������,��Ϊ���̽��
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
#include "SeqListHash.h"


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
* Function Name : InitSeqListHashTable
* Description	: ��ʼ��˳���ϣ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void InitSeqListHashTable( SEQ_LIST_HASH_TABLE_TypeDef *hashPtr, void *pBase, uint32_t lenth )
{
	hashPtr->basePtr = pBase;
	hashPtr->maxLenth = lenth;
	
	memset(pBase, NULL, lenth*sizeof(SEQ_LIST_HASH_TABLE_NODE_TypeDef));
}

/*
*********************************************************************************************************
* Function Name : SearchSeqListHashTable
* Description	: ����˳���ϣ��
* Input			: hashPtr����ϣ����; keyPtr���ؼ��ֵ�ַ; hashKeyPtr:��ϣ���ֵ
*				: getHashKey_CallBack����Ŀ���ֵת��Ϊ��ϣ���ֵ
*				: matchHashKey_CallBack��Ŀ���ֵ���ϣ��洢�ļ�ֵ�Ƿ�ƥ��
*				: nextHashKey_CallBack����ȡ��һ�����õĹ�ϣ���ֵ
* Output		: None
* Return		: None
*********************************************************************************************************
*/
STATUS_HASH_ENUM SearchSeqListHashTable( SEQ_LIST_HASH_TABLE_TypeDef *hashPtr, void *keyPtr, uint32_t *hashKeyPtr,\
		bool (*getHashKey_CallBack)( SEQ_LIST_HASH_TABLE_TypeDef *hashPtr, void *keyPtr, uint32_t *hashKeyPtr ),\
		bool (*matchHashKey_CallBack)( SEQ_LIST_HASH_TABLE_NODE_TypeDef *base, uint32_t hashKey, void *keyPtr ),\
		bool (*nextHashKey_CallBack)( SEQ_LIST_HASH_TABLE_TypeDef *hashPtr, uint32_t *hashKeyPtr ) )
{
#if (OS_EN)
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
#endif	

	if ((getHashKey_CallBack == NULL) || (matchHashKey_CallBack == NULL) || \
		(nextHashKey_CallBack == NULL))
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif	
		
		return STATUS_HASH_UNDEFINED;
	}
	
	if (getHashKey_CallBack(hashPtr, keyPtr, hashKeyPtr) == false)
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif	
		
		return STATUS_HASH_UNDEFINED;
	}
		
	while (1)
	{
		SEQ_LIST_HASH_TABLE_NODE_TypeDef *nodePtr = (SEQ_LIST_HASH_TABLE_NODE_TypeDef *)hashPtr->basePtr + *hashKeyPtr;
		
		/* �����޼�¼ */
		if (nodePtr->key == NULL)
		{
		#if (OS_EN)
			CPU_CRITICAL_EXIT();
		#endif	
			
			return STATUS_HASH_FALSE;
		}
		else if (matchHashKey_CallBack(hashPtr->basePtr, *hashKeyPtr, keyPtr) == true)
		{
		#if (OS_EN)
			CPU_CRITICAL_EXIT();
		#endif		
			
			return STATUS_HASH_TRUE;
		}
		else if (nextHashKey_CallBack(hashPtr, hashKeyPtr) == false)
		{
		#if (OS_EN)
			CPU_CRITICAL_EXIT();
		#endif		
			
			return STATUS_HASH_UNDEFINED;
		}
	}
}

/*
*********************************************************************************************************
* Function Name : InsertSeqListHashTable
* Description	: ����˳���ϣ��
* Input			: hashPtr����ϣ����; keyPtr���ؼ��ֵ�ַ; valuePtr�����ݵĵ�ַ
*				: getHashKey_CallBack����Ŀ���ֵת��Ϊ��ϣ���ֵ
*				: matchHashKey_CallBack��Ŀ���ֵ���ϣ��洢�ļ�ֵ�Ƿ�ƥ��
*				: nextHashKey_CallBack����ȡ��һ�����õĹ�ϣ���ֵ
*				: insertHashValue_CallBack���ڹ�ϣ���в����½ڵ�
* Output		: None
* Return		: None
*********************************************************************************************************
*/
STATUS_HASH_ENUM InsertSeqListHashTable( SEQ_LIST_HASH_TABLE_TypeDef *hashPtr, void *keyPtr, void *valuePtr,\
		bool (*getHashKey_CallBack)( SEQ_LIST_HASH_TABLE_TypeDef *hashPtr, void *keyPtr, uint32_t *hashKeyPtr ),\
		bool (*matchHashKey_CallBack)( SEQ_LIST_HASH_TABLE_NODE_TypeDef *base, uint32_t hashKey, void *keyPtr ),\
		bool (*nextHashKey_CallBack)( SEQ_LIST_HASH_TABLE_TypeDef *hashPtr, uint32_t *hashKeyPtr ),\
		void (*insertHashValue_CallBack)( SEQ_LIST_HASH_TABLE_NODE_TypeDef *base, uint32_t hashKey, \
			void *keyPtr, void *valuePtr ))
{
#if (OS_EN)
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
#endif		
	
	if ((getHashKey_CallBack == NULL) || (matchHashKey_CallBack == NULL) ||\
		(nextHashKey_CallBack == NULL) || (insertHashValue_CallBack == NULL))
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif	
		
		return STATUS_HASH_UNDEFINED;
	}
	
	uint32_t hashKey = 0;
	STATUS_HASH_ENUM status = SearchSeqListHashTable(hashPtr, keyPtr, &hashKey,\
								getHashKey_CallBack,\
								matchHashKey_CallBack,\
								nextHashKey_CallBack);
	
	switch( status )
	{
		case STATUS_HASH_FALSE:
			insertHashValue_CallBack(hashPtr->basePtr, hashKey, keyPtr, valuePtr);
		#if (OS_EN)
			CPU_CRITICAL_EXIT();
		#endif
			return STATUS_HASH_TRUE;
		
		case STATUS_HASH_TRUE:
		#if (OS_EN)
			CPU_CRITICAL_EXIT();
		#endif
			return STATUS_HASH_FALSE;
		
		default:
		#if (OS_EN)
			CPU_CRITICAL_EXIT();
		#endif
			return STATUS_HASH_UNDEFINED;
	}
}

/*
*********************************************************************************************************
* Function Name : DeleteSeqListHashTable
* Description	: ɾ��˳���ϣ��
*				: getHashKey_CallBack����Ŀ���ֵת��Ϊ��ϣ���ֵ
*				: matchHashKey_CallBack��Ŀ���ֵ���ϣ��洢�ļ�ֵ�Ƿ�ƥ��
*				: nextHashKey_CallBack����ȡ��һ�����õĹ�ϣ���ֵ
* Output		: None
* Return		: None
*********************************************************************************************************
*/
STATUS_HASH_ENUM DeleteSeqListHashTable( SEQ_LIST_HASH_TABLE_TypeDef *hashPtr, void *keyPtr,\
		bool (*getHashKey_CallBack)( SEQ_LIST_HASH_TABLE_TypeDef *hashPtr, void *keyPtr, uint32_t *hashKeyPtr ),\
		bool (*matchHashKey_CallBack)( SEQ_LIST_HASH_TABLE_NODE_TypeDef *base, uint32_t hashKey, void *keyPtr ),\
		bool (*nextHashKey_CallBack)( SEQ_LIST_HASH_TABLE_TypeDef *hashPtr, uint32_t *hashKeyPtr ),\
		void (*deleteHashValue_CallBack)( SEQ_LIST_HASH_TABLE_NODE_TypeDef *base, uint32_t hashKey))
{
#if (OS_EN)
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
#endif	
	
	if ((getHashKey_CallBack == NULL) || (matchHashKey_CallBack == NULL) || \
		(nextHashKey_CallBack == NULL) || (deleteHashValue_CallBack == NULL))
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif	
		
		return STATUS_HASH_UNDEFINED;
	}
	
	uint32_t hashKey = 0;
	if (getHashKey_CallBack(hashPtr, keyPtr, &hashKey) == false)
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif	
		
		return STATUS_HASH_UNDEFINED;
	}
		
	while (1)
	{
		if (matchHashKey_CallBack(hashPtr->basePtr, hashKey, keyPtr) == true)
		{
			deleteHashValue_CallBack(hashPtr->basePtr, hashKey);
			
		#if (OS_EN)
			CPU_CRITICAL_EXIT();
		#endif
			
			return STATUS_HASH_TRUE;
		}
		else if (nextHashKey_CallBack(hashPtr, &hashKey) == false)
		{
		#if (OS_EN)
			CPU_CRITICAL_EXIT();
		#endif	
			
			return STATUS_HASH_FALSE;
		}
	}
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
