/*
*********************************************************************************************************
* @file    	SeqStack.c
* @author  	SY
* @version 	V1.1.0
* @date    	2017-1-9 08:50:46
* @IDE	 	Keil V5.22.0.0
* @Chip    	STM32F407VE
* @brief   	˳���ջԴ�ļ�
*********************************************************************************************************
* @attention
*	ջ��������һ�ν��в��������ɾ�����������Ա��׳ƶ�ջ����������в�����һ�˳�Ϊ��ջ������
*	��һ�̶��˳�Ϊ��ջ�ס�����ջ��û��Ԫ�س�Ϊ����ջ�����ص㣺�Ƚ������FILO����
* ---------------------------------------------------------
* �汾��V1.0.1 	�޸��ˣ�SY		�޸����ڣ�2016-9-12 13:30:37
* 
* 1������˳���ջ�ı�����
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
#include "SeqStack.h"


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
* Function Name : CreateSeqStack
* Description	: ����˳���ջ(�ڴ�ռ����ⲿ�ṩ����̬�ڴ�)
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void CreateSeqStack( SEQ_STACK_TypeDef *stackPtr, void *pBase, uint32_t lenth )
{
	stackPtr->basePtr = pBase;
	stackPtr->maxLenth = lenth;
	stackPtr->top = 0;
}

/*
*********************************************************************************************************
* Function Name : SeqStackIsFull
* Description	: ˳���ջ�Ƿ�Ϊ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM SeqStackIsFull( SEQ_STACK_TypeDef *stackPtr )
{
	if (stackPtr->top == stackPtr->maxLenth)
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
* Function Name : SeqStackIsEmpty
* Description	: ˳���ջ�Ƿ�Ϊ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM SeqStackIsEmpty( SEQ_STACK_TypeDef *stackPtr )
{
	if (stackPtr->top == 0)
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
* Function Name : GetSeqStackLenth
* Description	: ��ȡ˳���ջ����
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
uint32_t GetSeqStackLenth( SEQ_STACK_TypeDef *stackPtr )
{
	return stackPtr->maxLenth;
}

/*
*********************************************************************************************************
* Function Name : ClearSeqStack
* Description	: ��ն�ջ
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void ClearSeqStack( SEQ_STACK_TypeDef *stackPtr )
{
#if (OS_EN)
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
#endif
	
	stackPtr->top = 0;
	
#if (OS_EN)
	CPU_CRITICAL_EXIT();
#endif
}

/*
*********************************************************************************************************
* Function Name : PushSeqStack
* Description	: ��ջ
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM PushSeqStack( SEQ_STACK_TypeDef *stackPtr, void *dataIn,\
		void (*push_CallBack)( void *base, uint32_t offset, void *data ) )
{
#if (OS_EN)
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
#endif	

	if (SeqStackIsFull(stackPtr) == STATUS_DATA_STRUCT_TRUE)
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif	
		
		return STATUS_DATA_STRUCT_FALSE;
	}
	
	push_CallBack(stackPtr->basePtr, stackPtr->top, dataIn);
	stackPtr->top++;
	
#if (OS_EN)
	CPU_CRITICAL_EXIT();
#endif
	
	return STATUS_DATA_STRUCT_TRUE;
}

/*
*********************************************************************************************************
* Function Name : PopSeqStack
* Description	: ��ջ
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM PopSeqStack( SEQ_STACK_TypeDef *stackPtr, void *dataOut,\
		void (*pop_CallBack)( void *base, uint32_t offset, void *data ) )
{
#if (OS_EN)
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
#endif
	
	if (SeqStackIsEmpty(stackPtr) == STATUS_DATA_STRUCT_TRUE)
	{
	#if (OS_EN)
		CPU_CRITICAL_EXIT();
	#endif	
		
		return STATUS_DATA_STRUCT_FALSE;
	}
	
	stackPtr->top--;
	pop_CallBack(stackPtr->basePtr, stackPtr->top, dataOut);

#if (OS_EN)
	CPU_CRITICAL_EXIT();
#endif	
	
	return STATUS_DATA_STRUCT_TRUE;
}

/*
*********************************************************************************************************
* Function Name : GetSeqStackElement
* Description	: ��ȡ˳���ջԪ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM GetSeqStackElement( SEQ_STACK_TypeDef *stackPtr, void *dataOut,\
		void (*pop_CallBack)( void *base, uint32_t offset, void *data ) )
{
	SEQ_STACK_TypeDef stack = *stackPtr;
	
	return PopSeqStack(&stack,dataOut,pop_CallBack);
}

/*
*********************************************************************************************************
* Function Name : TraverseSeqStack
* Description	: ����˳���ջ
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void TraverseSeqStack( SEQ_STACK_TypeDef *stackPtr, void *dataOut,\
		void (*pop_CallBack)( void *base, uint32_t offset, void *data ),\
		void (*show_CallBack)( void *dataOut ) )
{
	SEQ_STACK_TypeDef stack = *stackPtr;

#if (OS_EN)
	CPU_SR_ALLOC();
	
	CPU_CRITICAL_ENTER();
#endif
	
	while ( PopSeqStack(&stack, dataOut, pop_CallBack) == STATUS_DATA_STRUCT_TRUE)
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
