/*
*********************************************************************************************************
* @file    	LinkDoubleCycleList.c
* @author  	SY
* @version 	V1.1.0
* @date    	2017-1-9 08:50:46
* @IDE	 	Keil V5.22.0.0
* @Chip    	STM32F407VE
* @brief   	双向循环链表源文件
*********************************************************************************************************
* @attention
*		双向链表也叫双链表，是链表的一种，它的每个数据结点中都有两个指针，分别指向直接后继和直接前驱。
*	所以，从双向链表中的任意一个结点开始，都可以很方便地访问它的前驱结点和后继结点。一般我们都构造
*	双向循环链表。
* 
* ---------------------------------------------------------
* 版本：V1.1.0 	修改人：SY		修改日期：2017-1-9 08:50:46
* 
* 1、增加线程安全操作。
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
* Description	: 创建双向循环链表(内存空间由内部提供、动态内存)
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
LINK_DOUBLE_CYCLE_LIST_TypeDef *CreateLinkDoubleCycleList( void )
{
	/* 创建头节点，头指针由外部提供 */
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
* Description	: 双向循环链表是否为空
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
* Description	: 获取双向循环链表长度
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
* Description	: 清空双向循环链表
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
* Description	: 销毁双向循环链表
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
* Description	: 双向循环链表插入
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
	
	/* 增加新节点 */
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
	
	/* 存储用户数据 */
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
* Description	: 双向循环链表删除
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
* Description	: 压入双向循环链表
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
* Description	: 弹出双向循环链表
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
* Description	: 向后遍历双向循环链表
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
* Description	: 向前遍历双向循环链表
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
* Description	: 连接双向循环链表
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
	
	/* 新结点并未释放头结点内存，需要外部 free() */
	pHeadNew->next = pHeadNew;
	pHeadNew->prev = pHeadNew;
	
#if (OS_EN)
	CPU_CRITICAL_EXIT();
#endif
	
	return STATUS_DATA_STRUCT_TRUE;
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
