/*
*********************************************************************************************************
* @file    	SeqListSort.c
* @author  	SY
* @version 	V1.1.0
* @date    	2017-1-9 08:50:46
* @IDE	 	Keil V5.22.0.0
* @Chip    	STM32F407VE
* @brief   	˳�����Ա�����Դ�ļ�
*********************************************************************************************************
* @attention
*	�������������Ա������ʵ�ֲ��������㷨��ǰ�������Ա��������
*		1��ֱ�Ӳ�������
*			��˳�����Ա��У���������Ԫ�صļ�ֵ������еļ�ֵһһ���бȽϣ��ҵ�������λ�ú󣬲���Ԫ�ء�
*		2���۰��������
*			��˳�����Ա��У�ͨ�����ַ������ҵ�������Ԫ�������Ա��е�λ�ã�Ȼ�����Ԫ�ء�
*			���ַ�ͨ���������Ա������ֵ���ȶ����Ա���Ԫ�صļ�ֵ���õ�������Ԫ�ص�λ�á�
*	�������������Ա������ʵ�ֽ��������㷨�����Ա����������
*		1��ð�ݷ�����
*			��˳�����Ա��У�ÿ�����򶼽���ֵ(��/С)�ŵ�����ظ����У�ֱ��������ϡ�
*		2����������ʹ�á����η���˼�룬
*			��1��ѡȡһ����׼ֵ������һ�����򣬽��������Ա��Ϊ�����֣���벿�ֱȻ�׼ֵС���Ұ벿��
*				�Ȼ�׼ֵ��
*			��2��ʹ�õݹ���÷�ʽ�����������������Ա��ظ�ִ�в���(1)��ֱ���������Ա�����
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
#include "SeqListSort.h"


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
* Function Name : InsertSeqListSequenceSorting
* Description	: ֱ�Ӳ���˳�����Ա�
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM InsertSeqListSequenceSorting( SEQ_LIST_TypeDef *listPtr, void *dataIn,\
		bool (*comp_CallBack)( void *base, uint32_t index, void *dataIn ),\
		void (*push_CallBack)( void *base, uint32_t index, void *dataIn ),\
		void (*copy_CallBack)( void *base, uint32_t targetIndex, uint32_t sourceIndex ) )
{
#if (OS_EN)
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
#endif


	if ((comp_CallBack == NULL) || (push_CallBack == NULL) || (copy_CallBack == NULL))
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif	
		
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	if (SeqListIsFull(listPtr) == STATUS_DATA_STRUCT_TRUE)
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif	
		
		return STATUS_DATA_STRUCT_FALSE;
	}
	
	/* ��λ������λ�� */
	uint32_t insertIndex = 0;
	for (insertIndex=0; insertIndex<listPtr->index; ++insertIndex)
	{
		if (comp_CallBack(listPtr->basePtr, insertIndex, dataIn) == true)
		{
			break;
		}
	}
	
	/* ��˳�����Ԫ�����κ��� */
	for (uint32_t i=listPtr->index; i>insertIndex; --i)
	{
		copy_CallBack(listPtr->basePtr, i, i-1);
	}
	
	/* ������Ԫ�� */
	push_CallBack(listPtr->basePtr, insertIndex, dataIn);
	listPtr->index++;
	
#if (OS_EN)
	CPU_CRITICAL_EXIT();
#endif
	
	return STATUS_DATA_STRUCT_TRUE;
}

/*
*********************************************************************************************************
* Function Name : InsertSeqListBinSorting
* Description	: �۰���ҷ�ʽ����˳�����Ա�
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM InsertSeqListBinSorting( SEQ_LIST_TypeDef *listPtr, void *dataIn,\
		int8_t (*equalKey_CallBack)( void *base, uint32_t nodeIndex, void *dataIn ),\
		void (*push_CallBack)( void *base, uint32_t index, void *dataIn ),\
		void (*copy_CallBack)( void *base, uint32_t targetIndex, uint32_t sourceIndex ) )
{
#if (OS_EN)
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
#endif	
	
	if ((equalKey_CallBack == NULL) || (push_CallBack == NULL) || (copy_CallBack == NULL))
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif	
		
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	if (SeqListIsFull(listPtr) == STATUS_DATA_STRUCT_TRUE)
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif	
		
		return STATUS_DATA_STRUCT_FALSE;
	}
	
	/* ��λ������λ�� */	
	uint32_t low = 1;
	uint32_t high = listPtr->index;

	while (low <= high)
	{
		uint32_t middle = (low + high) / 2;	
		int8_t result = equalKey_CallBack(listPtr->basePtr, middle-1, dataIn);
		if (result >= 0)
		{
			low = middle + 1;
		}
		else
		{
			high = middle - 1;
		}
	}
	uint32_t insertIndex = low - 1;
	
	/* ��˳�����Ԫ�����κ��� */
	for (uint32_t i=listPtr->index; i>insertIndex; --i)
	{
		copy_CallBack(listPtr->basePtr, i, i-1);
	}
	
	/* ������Ԫ�� */
	push_CallBack(listPtr->basePtr, insertIndex, dataIn);
	listPtr->index++;
	
#if (OS_EN)
	CPU_CRITICAL_EXIT();
#endif
	
	return STATUS_DATA_STRUCT_TRUE;
}

/*
*********************************************************************************************************
* Function Name : SeqListBubSorting
* Description	: ˳�����Ա�ð������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM SeqListBubSorting( SEQ_LIST_TypeDef *listPtr,\
		int8_t (*equalKey_CallBack)( void *base, uint32_t targetIndex, uint32_t sourceIndex ),\
		void (*swap_CallBack)( void *base, uint32_t targetIndex, uint32_t sourceIndex ) )
{
#if (OS_EN)
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
#endif	
	
	if ((equalKey_CallBack == NULL) || (swap_CallBack == NULL))
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif	
		
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	if (SeqListIsEmpty(listPtr) == STATUS_DATA_STRUCT_TRUE)
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif	
		
		return STATUS_DATA_STRUCT_FALSE;
	}
	
	/* ���ִ�� (n-1) ������ */
	for (uint32_t i=0; i<listPtr->index-1; ++i)
	{
		for (uint32_t j=0; j<listPtr->index-i-1; ++j)
		{
			int8_t result = equalKey_CallBack(listPtr->basePtr, j+1, j);
			if (result > 0)
			{
				swap_CallBack(listPtr->basePtr, j+1, j);
			}
		}
	}
	
#if (OS_EN)
	CPU_CRITICAL_EXIT();
#endif	
	
	return STATUS_DATA_STRUCT_TRUE;
}

/*
*********************************************************************************************************
* Function Name : SeqListQuickSortingPartition
* Description	: ˳�����Ա���������Ϊ������
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void SeqListQuickSortingPartition( SEQ_LIST_TypeDef *listPtr, uint32_t low, uint32_t high, uint32_t *middle,\
		int8_t (*equalKey_CallBack)( void *base, uint32_t nodeIndex, void *dataIn ),\
		void (*copy_CallBack)( void *base, uint32_t targetIndex, uint32_t sourceIndex ),\
		void (*push_CallBack)( void *base, uint32_t index, void *data ),\
		void (*pop_CallBack)( void *base, uint32_t index, void *data ))
{
#if (OS_EN)
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
#endif	
	
	SEQ_LIST_SORT_NODE_TypeDef referenceNode;
	PopSeqList(listPtr, &referenceNode, low, pop_CallBack);
	
	/**
	 *	ԭ������->��Ѱ�ұȻ�׼ֵС��ֵ Value1������->��Ѱ�ұȻ�׼ֵ���ֵ Value2��
	 *	 ���� Value1 �� Value2��ֱ����׼ֵ��ߵ�ֵȫ��С�ڻ�׼ֵ�ұߵ�ֵ��
	 */
	while (low < high)
	{
		while ((low < high) && (equalKey_CallBack(listPtr->basePtr, high, &referenceNode) >= 0))
		{
			high--;
		}
		if (low < high)
		{
			copy_CallBack(listPtr->basePtr, low, high);
		}
		
		while ((low < high) && (equalKey_CallBack(listPtr->basePtr, low, &referenceNode) <= 0))
		{
			low++;
		}
		if (low < high)
		{
			copy_CallBack(listPtr->basePtr, high, low);
		}
	}
	
	PushSeqList(listPtr, &referenceNode, low, push_CallBack);	
	*middle = low;
	
#if (OS_EN)
	CPU_CRITICAL_EXIT();
#endif
}

/*
*********************************************************************************************************
* Function Name : __SeqListQuickSorting
* Description	: ˳�����Ա����������ģ�low/high��0��ʼ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void __SeqListQuickSorting( SEQ_LIST_TypeDef *listPtr, uint32_t low, uint32_t high,\
		int8_t (*equalKey_CallBack)( void *base, uint32_t nodeIndex, void *dataIn ),\
		void (*copy_CallBack)( void *base, uint32_t targetIndex, uint32_t sourceIndex ),\
		void (*push_CallBack)( void *base, uint32_t index, void *data ),\
		void (*pop_CallBack)( void *base, uint32_t index, void *data ))
{
	uint32_t middle = 0;
	
#if (OS_EN)
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
#endif
	
	if (low < high)
	{
		SeqListQuickSortingPartition(listPtr, low, high, &middle,\
								equalKey_CallBack,\
								copy_CallBack,\
								push_CallBack,\
								pop_CallBack);
		if (middle > 0)
		{
			SeqListQuickSorting(listPtr, low, middle-1,\
								equalKey_CallBack,\
								copy_CallBack,\
								push_CallBack,\
								pop_CallBack);
		}
		
		SeqListQuickSorting(listPtr, middle+1, high,\
								equalKey_CallBack,\
								copy_CallBack,\
								push_CallBack,\
								pop_CallBack);
	}
	
#if (OS_EN)
	CPU_CRITICAL_EXIT();
#endif
}

/*
*********************************************************************************************************
* Function Name : SeqListCoreQuickSorting
* Description	: ˳�����Ա����������ģ�low/high��0��ʼ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM SeqListQuickSorting( SEQ_LIST_TypeDef *listPtr, uint32_t low, uint32_t high,\
		int8_t (*equalKey_CallBack)( void *base, uint32_t nodeIndex, void *dataIn ),\
		void (*copy_CallBack)( void *base, uint32_t targetIndex, uint32_t sourceIndex ),\
		void (*push_CallBack)( void *base, uint32_t index, void *data ),\
		void (*pop_CallBack)( void *base, uint32_t index, void *data ))
{
#if (OS_EN)
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
#endif	
	
	if ((equalKey_CallBack == NULL) || (copy_CallBack == NULL) ||\
		(push_CallBack == NULL) || (pop_CallBack == NULL))
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif	
		
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	if (SeqListIsEmpty(listPtr) == STATUS_DATA_STRUCT_TRUE)
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif	
		
		return STATUS_DATA_STRUCT_FALSE;
	}
	
	__SeqListQuickSorting(listPtr, low, high,\
								equalKey_CallBack,\
								copy_CallBack,\
								push_CallBack,\
								pop_CallBack);
	
#if (OS_EN)
	CPU_CRITICAL_EXIT();
#endif
	
	return STATUS_DATA_STRUCT_TRUE;
}

/*
*********************************************************************************************************
* Function Name : TraverseSeqListSorting
* Description	: ����˳�����Ա�
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void TraverseSeqListSorting( SEQ_LIST_TypeDef *listPtr, void *dataOut,\
		void (*pop_CallBack)( void *base, uint32_t index, void *data ),\
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
