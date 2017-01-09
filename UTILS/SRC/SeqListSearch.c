/*
*********************************************************************************************************
* @file    	SeqListSearch.c
* @author  	SY
* @version 	V1.1.0
* @date    	2017-1-9 08:50:46
* @IDE	 	Keil V5.22.0.0
* @Chip    	STM32F407VE
* @brief   	˳�����Ա�����㷨Դ�ļ�
*********************************************************************************************************
* @attention
*		1�������Ҷ���һ���ɼ�ֵ��������������ɡ���ֵ�������Ψһ�ԣ����Ա�ʶ��Ӧ���ݡ�
*		2��˳����ң���ֵ���������֡��ַ��������ͣ�ͨ����һ�Ƚ�Ŀ���ֵ����ֵ�������ȣ�����ҳɹ���
*			���򣬲���ʧ�ܡ�
*		3���۰���ң���ֵ���������֡��ַ��������ͣ�����������ͨ�������ص�������֧��˳�������
*			ͨ��������������ֵ���Ƚ�Ŀ���ֵ������������Ӧ�ı��ֵ�����Ŀ���ֵ���ڱ��ֵ������ҳɹ���
*			���򣬱Ƚ�Ŀ���ֵ����ֵ�Ĵ�С��ϵ���ƶ�����ֵ�����±Ƚϡ�
*
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
#include "SeqListSearch.h"


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
* Function Name : InitSeqListSearching
* Description	: ��ʼ��˳������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void InitSeqListSearching( SEQ_LIST_SEARCH_TypeDef *listPtr, void *pBase, uint32_t lenth )
{
	listPtr->basePtr = pBase;
	listPtr->maxLenth = lenth;
}

/*
*********************************************************************************************************
* Function Name : InsertSeqListSearchingNode
* Description	: ����˳�����ҽڵ�
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
bool InsertSeqListSearchingNode( SEQ_LIST_SEARCH_TypeDef *listPtr,\
		uint32_t nodeIndex, void *key, void *value,\
		void (*setNode_CallBack)( void *base, uint32_t nodeIndex, void *key, void *value ) )
{
#if (OS_EN)
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
#endif	
	
	if (nodeIndex >= listPtr->maxLenth)
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif	
		
		return false;
	}
	
	if (setNode_CallBack)
	{
		setNode_CallBack(listPtr->basePtr, nodeIndex, key, value);
	}
	
#if (OS_EN)
	CPU_CRITICAL_EXIT();
#endif	
	
	return true;
}

/*
*********************************************************************************************************
* Function Name : SeqListSequenceSearching
* Description	: ˳�����Ա�˳�����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void *SeqListSequenceSearching( SEQ_LIST_SEARCH_TypeDef *listPtr, void *findKey,\
		void *(*matchKey_CallBack)( void *base, uint32_t nodeIndex, void *findKey ) )
{
	uint32_t nodeIndex = 0;
	
#if (OS_EN)
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
#endif

	while (nodeIndex < listPtr->maxLenth)
	{
		if (matchKey_CallBack)
		{
			void *value = matchKey_CallBack(listPtr->basePtr, nodeIndex, findKey);
			if (value != NULL)
			{
			#if (OS_EN)
				CPU_CRITICAL_EXIT();
			#endif	
				
				return value;
			}
		}
		nodeIndex++;
	}
	
#if (OS_EN)
	CPU_CRITICAL_EXIT();
#endif
	
	return NULL;
}

/*
*********************************************************************************************************
* Function Name : SeqListBinSearching
* Description	: ˳�����Ա��۰����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
bool SeqListBinSearching( SEQ_LIST_SEARCH_TypeDef *listPtr, void *findKey, uint32_t *keyIndex,\
		int8_t (*equalKey_CallBack)( void *base, uint32_t nodeIndex, void *findKey ) )
{
#if (OS_EN)
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
#endif	
	
	if (equalKey_CallBack == NULL)
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif	
		
		return NULL;
	}
	uint32_t low = 1;
	uint32_t high = listPtr->maxLenth;

	while (low <= high)
	{
		uint32_t middle = (low + high) / 2;	
		int8_t result = equalKey_CallBack(listPtr->basePtr, middle-1, findKey);
		if (result > 0)
		{
			low = middle + 1;
		}
		else if (result < 0)
		{
			high = middle - 1;
		}
		else
		{
			*keyIndex = middle - 1;
			
		#if (OS_EN)
			CPU_CRITICAL_EXIT();
		#endif
			
			return true;
		}
	}
	
#if (OS_EN)
	CPU_CRITICAL_EXIT();
#endif
	
	return false;
}



/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
