/*
*********************************************************************************************************
* @file    	SeqList.c
* @author  	SY
* @version 	V1.1.0
* @date    	2017-1-9 08:50:46
* @IDE	 	Keil V5.22.0.0
* @Chip    	STM32F407VE
* @brief   	˳�����Ա�Դ�ļ�
*********************************************************************************************************
* @attention
*		���Ա������������򵥡�Ҳ����õ�һ�����ݽṹ�����Ա�������Ԫ��֮��Ĺ�ϵ��һ��һ�Ĺ�ϵ��
*	�����˵�һ�������һ������Ԫ��֮�⣬��������Ԫ�ض�����β��ӵġ����Ա���߼��ṹ�򵥣�����ʵ�ֺͲ�����
*	��ˣ����Ա��������ݽṹ��ʵ��Ӧ�����ǹ㷺���õ�һ�����ݽṹ��
*   	���Ա���һ�����Խṹ������һ������n��0�������������У��������еĽ�㣬���ҽ���һ����ʼ���
*	û��ǰ������һ����̽�㣬���ҽ���һ���ն˽��û�к�̵���һ��ǰ����㣬�����Ľ�㶼���ҽ���һ��ǰ��
*	��һ����̽�㡣
*
* ---------------------------------------------------------
* �汾��V1.0.1 	�޸��ˣ�SY		�޸����ڣ�2016-9-12 15:26:19
* 
* 1������˳�����Ա�ı�����
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
#include "SeqList.h"


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
* Function Name : CreateSeqList
* Description	: ����˳�����Ա�(�ڴ�ռ����ⲿ�ṩ����̬�ڴ�)
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void CreateSeqList( SEQ_LIST_TypeDef *listPtr, void *pBase, uint32_t lenth )
{
	listPtr->basePtr = pBase;
	listPtr->maxLenth = lenth;
	listPtr->index = 0;
}

/*
*********************************************************************************************************
* Function Name : SeqListIsFull
* Description	: ˳�����Ա��Ƿ�Ϊ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM SeqListIsFull( SEQ_LIST_TypeDef *listPtr )
{
	if (listPtr->index == listPtr->maxLenth)
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
* Function Name : SeqListIsEmpty
* Description	: ˳�����Ա��Ƿ�Ϊ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM SeqListIsEmpty( SEQ_LIST_TypeDef *listPtr )
{
	if (listPtr->index == 0)
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
* Function Name : GetSeqListLenth
* Description	: ��ȡ˳�����Ա���
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
uint32_t GetSeqListLenth( SEQ_LIST_TypeDef *listPtr )
{
	return listPtr->index;
}

/*
*********************************************************************************************************
* Function Name : ClearSeqList
* Description	: ������Ա�
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void ClearSeqList( SEQ_LIST_TypeDef *listPtr )
{
#if (OS_EN)
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
#endif
	
	listPtr->index = 0;
	
#if (OS_EN)
	CPU_CRITICAL_EXIT();
#endif
}

/*
*********************************************************************************************************
* Function Name : PushSeqList
* Description	: ѹ�����Ա�
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM PushSeqList( SEQ_LIST_TypeDef *listPtr, void *dataIn, uint32_t pos,\
		void (*push_CallBack)( void *base, uint32_t pos, void *dataIn ) )
{
#if (OS_EN)
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
#endif
	
	if (pos >= listPtr->index)
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif
		
		return STATUS_DATA_STRUCT_FALSE;
	}
	
	push_CallBack(listPtr->basePtr, pos, dataIn);
	
#if (OS_EN)
	CPU_CRITICAL_EXIT();
#endif
	
	return STATUS_DATA_STRUCT_TRUE;
}

/*
*********************************************************************************************************
* Function Name : PopSeqList
* Description	: �������Ա�
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM PopSeqList( SEQ_LIST_TypeDef *listPtr, void *dataOut, uint32_t pos,\
		void (*pop_CallBack)( void *base, uint32_t pos, void *dataOut ) )
{
#if (OS_EN)
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
#endif	
	
	if (pos >= listPtr->index)
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif	
	
		return STATUS_DATA_STRUCT_FALSE;
	}

	pop_CallBack(listPtr->basePtr, pos, dataOut);
	
#if (OS_EN)
	CPU_CRITICAL_EXIT();
#endif

	return STATUS_DATA_STRUCT_TRUE;
}

/*
*********************************************************************************************************
* Function Name : InsertSeqList
* Description	: �������Ա���pos��ǰ�����룬pos��0��ʼ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM InsertSeqList( SEQ_LIST_TypeDef *listPtr, void *dataIn, uint32_t pos,\
		void (*push_CallBack)( void *base, uint32_t pos, void *dataIn ),\
		void (*copy_CallBack)( void *base, uint32_t targetPos, uint32_t sourcePos ) )
{
	
#if (OS_EN)
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
#endif
	
	if (SeqListIsFull(listPtr) == STATUS_DATA_STRUCT_TRUE)
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif
		
		return STATUS_DATA_STRUCT_FALSE;
	}
	
	if (pos >= listPtr->index)
	{
		pos = listPtr->index;
	}
	
	for (uint32_t i=listPtr->index; i>pos; --i)
	{
		copy_CallBack(listPtr->basePtr, i, i-1);
	}
	push_CallBack(listPtr->basePtr, pos, dataIn);
	listPtr->index++;
	
#if (OS_EN)
	CPU_CRITICAL_EXIT();
#endif
	
	return STATUS_DATA_STRUCT_TRUE;
}

/*
*********************************************************************************************************
* Function Name : DeleteSeqList
* Description	: ɾ�����Ա�
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM DeleteSeqList( SEQ_LIST_TypeDef *listPtr, uint32_t pos,\
		void (*copy_CallBack)( void *base, uint32_t targetPos, uint32_t sourcePos ) )
{
	
#if (OS_EN)
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
#endif
	
	if (pos >= listPtr->index)
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif
		
		return STATUS_DATA_STRUCT_FALSE;
	}

	listPtr->index--;
	for (uint32_t i=pos; i<listPtr->index; ++i)
	{
		copy_CallBack(listPtr->basePtr, i, i+1);
	}

#if (OS_EN)
	CPU_CRITICAL_EXIT();
#endif	
	
	return STATUS_DATA_STRUCT_TRUE;
}

/*
*********************************************************************************************************
* Function Name : TraverseSeqList
* Description	: ����˳�����Ա�
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void TraverseSeqList( SEQ_LIST_TypeDef *listPtr, void *dataOut,\
		void (*pop_CallBack)( void *base, uint32_t offset, void *data ),\
		void (*show_CallBack)( void *dataOut ) )
{
	SEQ_LIST_TypeDef list = *listPtr;
	uint32_t posIndex = 0;

#if (OS_EN)
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
#endif
	
	while (PopSeqList(&list, dataOut, posIndex++, pop_CallBack) == STATUS_DATA_STRUCT_TRUE)
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
