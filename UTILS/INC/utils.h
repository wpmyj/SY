/*
*********************************************************************************************************
* @file    	utils.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-10-11 15:10:28
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	���߰�ͷ�ļ�
*********************************************************************************************************
* @attention
*
* 
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Define to prevent recursive inclusion
*********************************************************************************************************
*/
#ifndef __UTILS_H
#define __UTILS_H

/*
*********************************************************************************************************
*                              				Exported Includes
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*                              				LIB
*********************************************************************************************************
*/
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

/*
*********************************************************************************************************
*                              				UTILS
*********************************************************************************************************
*/
#include "stm32f4xx_hal.h"
#include "ustdlib.h"
#include "ustring.h"
#include "isqrt.h" 
#include "fonts.h"
#include "list.h"

#include "config.h"
#include "MY_DEBUG.H"
#include "MY_ASSERT.H"

/*
*********************************************************************************************************
*                                             OS
*********************************************************************************************************
*/
#include "os_inc.h"

#define OS_EN								(0x01U)


/*
*********************************************************************************************************
*                              				Exported define
*********************************************************************************************************
*/
#define MAX_STRING_NUM						(32)
#define EPSINON								(0.00001f)				//�����������ھ��Ե�0��ֻ��ȡ����ֵ


/*
*********************************************************************************************************
*                              				Exported types
*********************************************************************************************************
*/
/* ���ݽṹ���Ͷ��� */
typedef enum
{
	STATUS_DATA_STRUCT_FALSE = 0,
	STATUS_DATA_STRUCT_TRUE,
	STATUS_DATA_STRUCT_UNDEFINED,
}DATA_STRUCT_STATUS_ENUM;

/* ��ϣ��ṹ���Ͷ��� */
typedef enum
{
	STATUS_HASH_FALSE = 0,
	STATUS_HASH_TRUE = 1,
	STATUS_HASH_UNDEFINED,
}STATUS_HASH_ENUM;

/*
*********************************************************************************************************
*                              				ѭ������
*********************************************************************************************************
*/
typedef enum
{
	STATUS_CYCLE_TASK_IDLE = 0,
	STATUS_CYCLE_TASK_ON,
	STATUS_CYCLE_TASK_OFF,
	STATUS_CYCLE_TASK_DONE,
}CYCLE_TASK_STATUS_TypeDef;

typedef struct
{
	/* ����������ʵ�� */
	CYCLE_TASK_STATUS_TypeDef status;					//״̬
	uint32_t runPeriod;									//������������(��λ��ms)
	void *devicePtr;									//�豸ָ��
	
	/* �ص����� */
	void (*taskStartNotify_CallBack)( void *devicePrt );//��ʼ֪ͨ
	void (*onDevice_CallBack)( void *devicePrt );		//���豸
	void (*offDevice_CallBack)( void *devicePrt );		//�ر��豸
	void (*taskDoneNotify_CallBack)( void *devicePrt );	//���֪ͨ
	
	/* �ɱ�������ʵ�� */
	bool enableFlag;							//ʹ�ܿ���
	bool disableFlag;							//ʧ�ܿ���
	uint32_t executeNums;						//ִ�д���
	uint32_t onTime;							//��ʱ��
	uint32_t offTime;							//�ر�ʱ��
	uint32_t executeCount;						//ִ�ж��������
	uint32_t onCount;							//�򿪼�����
	uint32_t offCount;							//�رռ�����
}CYCLE_TASK_TypeDef;	

/*
*********************************************************************************************************
*                              				��ʱֹͣ����
*********************************************************************************************************
*/
typedef enum
{
	STATUS_DELAY_STOP_TASK_IDLE = 0,
	STATUS_DELAY_STOP_TASK_ON,
	STATUS_DELAY_STOP_TASK_DONE,
}DELAY_STOP_STATUS_TypeDef;

typedef struct
{
	/* ����������ʵ�� */
	DELAY_STOP_STATUS_TypeDef status;						//״̬
	uint32_t runPeriod;										//������������(��λ��ms)
	void *devicePrt;										//�豸ָ��
	
	/* �ص����� */
	void (*taskStartNotify_CallBack)( void *devicePrt );	//��ʼ֪ͨ
	void (*onDevice_CallBack)( void *devicePrt );			//���豸
	void (*offDevice_CallBack)( void *devicePrt );			//�ر��豸
	void (*taskDoneNotify_CallBack)( void *devicePrt );		//���֪ͨ
	
	/* �ɱ�������ʵ�� */
	bool enableFlag;							//ʹ�ܿ���
	bool disableFlag;							//ʧ�ܿ���
	uint32_t delayTime;							//��ʱʱ��
	uint32_t delayCount;						//��ʱ������
}DELAY_STOP_TASK_TypeDef;

/*
*********************************************************************************************************
*                              				IOģ��PWM����
*********************************************************************************************************
*/
typedef enum
{
	PWM_IO_STATUS_IDLE = 0,
	PWM_IO_STATUS_OPEN,
	PWM_IO_STATUS_CLOSE,
	PWM_IO_STATUS_DONE,
}PWM_IO_STATUS_TypeDef;

typedef struct
{
	/* ����������ʵ�� */
	PWM_IO_STATUS_TypeDef status;					//״̬
	float runPeriod;								//��������
	float outputPeriod;								//�������
	uint16_t maxControlDegree;						//��������
	void *devicePrt;								//�豸ָ��
	
	/* �ص����� */
	void (*taskStartNotify_CallBack)( void *devicePrt );//��ʼ֪ͨ
	void (*onDevice_CallBack)( void *devicePrt );		//���豸
	void (*offDevice_CallBack)( void *devicePrt );		//�ر��豸
	void (*taskDoneNotify_CallBack)( void *devicePrt );	//���֪ͨ
	
	/* �ɱ�������ʵ�� */
	bool enableFlag;								//ʹ�ܿ���
	bool disableFlag;								//ʧ�ܿ���
	bool continueRunFlag;							//�������п���
	uint16_t dutyCycle;								//ռ�ձ�
	uint32_t PWM_Count;								//PWM������
	uint32_t sumCount;								//��ִ�д���
	uint32_t Reg_CCR;								//�Ƚϼ�����
}PWM_IO_TypeDef;

/*
*********************************************************************************************************
*                              				��������
*********************************************************************************************************
*/
typedef enum
{
	SWITCH_TASK_IDLE = 0,
	SWITCH_TASK_OPEN,
	SWITCH_TASK_OPEN_NOTIFY,
	SWITCH_TASK_CLOSE,
	SWITCH_TASK_CLOSE_NOTIFY,
}SWITCH_TASK_STATUS_TypeDef;

typedef struct
{
	/* ����������ʵ�� */
	SWITCH_TASK_STATUS_TypeDef status;					//״̬
	void *devicePrt;									//�豸ָ��
	
	/* �ص����� */
	bool (*isSwitchOn_CallBack)( void *devicePrt );		//�����Ƿ��
	void (*onDevice_CallBack)( void *devicePrt );		//�򿪿���֪ͨ
	void (*offDevice_CallBack)( void *devicePrt );		//�رտ���֪ͨ
	
	/* �ɱ�������ʵ�� */	
}SWITCH_TASK_TypeDef;

/*
*********************************************************************************************************
*                              				���Ź�����
*********************************************************************************************************
*/
typedef enum
{
	WATCH_DOG_TASK_IDLE = 0,
	WATCH_DOG_TASK_WORKING,
	WATCH_DOG_TASK_DONE,
}WATCH_DOG_TASK_STATUS_TypeDef;

typedef struct
{
	/* ����������ʵ�� */
	WATCH_DOG_TASK_STATUS_TypeDef status;			//״̬
	uint32_t runPeriod;								//��������
	void *devicePrt;								//�豸ָ��
	
	/* �ص����� */
	void (*timeUpNotify_CallBack)( void *devicePrt );//ʱ�䵽��֪ͨ
	
	/* �ɱ�������ʵ�� */
	bool enableFlag;								//ʹ�ܿ���
	bool disableFlag;								//ʧ�ܿ���
	uint32_t decreaseCount;							//�ݼ�������
	uint32_t countTimeMS;							//����ʱ��
}WATCH_DOG_TASK_TypeDef;

/*
*********************************************************************************************************
*                              				����IO����
*********************************************************************************************************
*/
typedef struct tagIO_OPERATE_TypeDef {
	uint32_t shadowReg;										//Ӱ�ӼĴ���
	uint32_t dataReg;										//���ݼĴ���
	
	//��ʼ��
	void (*init)(struct tagIO_OPERATE_TypeDef *this);		//��ʼ��
	
	//������
	uint32_t (*read)(struct tagIO_OPERATE_TypeDef *this);	//��ȡ����
	void (*readSync)(struct tagIO_OPERATE_TypeDef *this);	//��ͬ��
	
	//д����
	void (*setOutputBit)(struct tagIO_OPERATE_TypeDef *this, uint32_t bit);	//�������λ
	void (*clrOutputBit)(struct tagIO_OPERATE_TypeDef *this, uint32_t bit);	//ȡ�����λ
	void (*revOutputBit)(struct tagIO_OPERATE_TypeDef *this, uint32_t bit);	//ȡ�����λ
	void (*setOutput)(struct tagIO_OPERATE_TypeDef *this, uint32_t value);	//�������
	void (*writeSync)(struct tagIO_OPERATE_TypeDef *this);	//дͬ��
}IO_OPERATE_TypeDef;

/*
*********************************************************************************************************
*                              				Exported constants
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Exported macro
*********************************************************************************************************
*/
#define ARRAY_SIZE(x) 						(sizeof(x) / sizeof((x)[0])) 


__STATIC_INLINE void ExchangeData_UINT16( void *data1, void *data2 )
{
	uint16_t *data1Ptr = (uint16_t *)data1;
	uint16_t *data2Ptr = (uint16_t *)data2;
	uint16_t temp = *data1Ptr;
	
	*data1Ptr = *data2Ptr;
	*data2Ptr = temp;
}

__STATIC_INLINE void *new(uint32_t size) {	
	void *object = calloc(1, size);
	ECHO_ASSERT(object != NULL);
	
	return object;
}

__STATIC_INLINE void delete(void *object) {
	ECHO_ASSERT(object != NULL);
	
	free(object);
}				


/*
*********************************************************************************************************
*                              				Exported variables
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Exported functions
*********************************************************************************************************
*/
uint16_t hton16(uint16_t n);
uint32_t hton32(uint32_t n);
void float2char( uint8_t sumBit, uint8_t pointBit, float source, char * const str );
void num2char( uint8_t sumBit, int32_t source, char * const str );
uint8_t RemoveStrChar( char *str, const char c );
bool FloatIsEqual(float fp1, float fp2);
uint8_t GetMonthDays( uint16_t year, uint8_t mouth );
TestStatus BuffCmpWithChar( const uint8_t *pSorce, uint8_t target, uint32_t lenth );
void ExchangeData( void *data1, void *data2, void (*exchangeData_CallBack)( void *data1, void *data2 ) );

/*
*********************************************************************************************************
*                              				ѭ������
*********************************************************************************************************
*/
void RepeatExecuteTaskInit( CYCLE_TASK_TypeDef *cycleTaskPtr, uint32_t runPeriod,
							void *devicePtr );
void RepeatExecuteTaskDeInit( CYCLE_TASK_TypeDef *cycleTaskPtr );
void SetRepeatExecuteTime( CYCLE_TASK_TypeDef *cycleTaskPtr,\
		uint32_t executeNums, uint32_t onTimeMS, uint32_t offTimeMS );
void CloseRepeatExecuteTask( CYCLE_TASK_TypeDef *cycleTaskPtr );	
void RepeatExecuteTaskCycle( CYCLE_TASK_TypeDef *cycleTaskPtr );
void RegisterRepeatExecuteTaskStartNotify_CallBack( CYCLE_TASK_TypeDef *cycleTaskPtr,\
		void (*taskStartNotify_CallBack)( void *devicePrt ) );
void RegisterRepeatExecuteTaskOnDevice_CallBack( CYCLE_TASK_TypeDef *cycleTaskPtr,\
		void (*onDevice_CallBack)( void *devicePrt ) );
void RegisterRepeatExecuteTaskOffDevice_CallBack( CYCLE_TASK_TypeDef *cycleTaskPtr,\
		void (*offDevice_CallBack)( void *devicePrt ) );
void RegisterRepeatExecuteTaskDoneNotify_CallBack( CYCLE_TASK_TypeDef *cycleTaskPtr,\
		void (*taskDoneNotify_CallBack)( void *devicePrt ) );

/*
*********************************************************************************************************
*                              				��ʱֹͣ����
*********************************************************************************************************
*/
void DelayStopTaskInit( DELAY_STOP_TASK_TypeDef *delayStopPtr, uint32_t runPeriod,
						void *devicePrt );
void DelayStopTaskDeInit( DELAY_STOP_TASK_TypeDef *delayStopPtr );
void SetDelayStopTime( DELAY_STOP_TASK_TypeDef *delayStopPtr,\
		uint32_t delayTimeMS );
void CloseDelayStopTask( DELAY_STOP_TASK_TypeDef *delayStopPtr );
bool IsDelayStopTaskRuning( DELAY_STOP_TASK_TypeDef *delayStopPtr );
void DelayStopTaskCycle( DELAY_STOP_TASK_TypeDef *delayStopPtr );
void RegisterDelayStopTaskStartNotify_CallBack( DELAY_STOP_TASK_TypeDef *delayStopPtr,\
		void (*taskStartNotify_CallBack)( void *devicePrt ) );
void RegisterDelayStopTaskOnDevice_CallBack( DELAY_STOP_TASK_TypeDef *delayStopPtr,\
		void (*onDevice_CallBack)( void *devicePrt ) );
void RegisterDelayStopTaskOffDevice_CallBack( DELAY_STOP_TASK_TypeDef *delayStopPtr,\
		void (*offDevice_CallBack)( void *devicePrt ) );
void RegisterDelayStopTaskDoneNotify_CallBack( DELAY_STOP_TASK_TypeDef *delayStopPtr,\
		void (*taskDoneNotify_CallBack)( void *devicePrt ) );

/*
*********************************************************************************************************
*                              				IOģ��PWM����
*********************************************************************************************************
*/
void PWM_IO_OutputInit( PWM_IO_TypeDef *PWM, float runPeriod, float outputPeriod, 
						uint16_t maxControlDegree, void *devicePrt );
void PWM_IO_OutputDeInit( PWM_IO_TypeDef *PWM );
void PWM_IO_SetOutput( PWM_IO_TypeDef *PWM, uint16_t dutyCycle );
void PWM_IO_CloseOutput( PWM_IO_TypeDef *PWM );
void PWM_IO_OutputCycle( PWM_IO_TypeDef *PWM_ObjPtr );
void RegisterPWM_IO_TaskStartNotify_CallBack( PWM_IO_TypeDef *PWM_IO_Ptr,\
		void (*taskStartNotify_CallBack)( void *devicePrt ) );
void RegisterPWM_IO_TaskOnDevice_CallBack( PWM_IO_TypeDef *PWM_IO_Ptr,\
		void (*onDevice_CallBack)( void *devicePrt ) );
void RegisterPWM_IO_TaskOffDevice_CallBack( PWM_IO_TypeDef *PWM_IO_Ptr,\
		void (*offDevice_CallBack)( void *devicePrt ) );
void RegisterPWM_IO_TaskDoneNotify_CallBack( PWM_IO_TypeDef *PWM_IO_Ptr,\
		void (*taskDoneNotify_CallBack)( void *devicePrt ) );

/*
*********************************************************************************************************
*                              				��������
*********************************************************************************************************
*/
void SwitchTaskInit( SWITCH_TASK_TypeDef *switchPtr, void *devicePrt );
void SwitchTaskDeInit( SWITCH_TASK_TypeDef *switchPtr );
void SwitchTask( SWITCH_TASK_TypeDef *switchPtr );
void RegisterSwitchTaskIsSwitchOn_CallBack( SWITCH_TASK_TypeDef *switchPtr,\
		bool (*isSwitchOn_CallBack)( void *devicePrt ) );
void RegisterSwitchTaskOnDevice_CallBack( SWITCH_TASK_TypeDef *switchPtr,\
		void (*onDevice_CallBack)( void *devicePrt ) );
void RegisterSwitchTaskOffDevice_CallBack( SWITCH_TASK_TypeDef *switchPtr,\
		void (*offDevice_CallBack)( void *devicePrt ) );
		
/*
*********************************************************************************************************
*                              				���Ź�����
*********************************************************************************************************
*/
void WatchDogTaskInit( WATCH_DOG_TASK_TypeDef *wwdgPtr, uint32_t runPeriod, 
						void *devicePrt);
void WatchDogTaskDeInit( WATCH_DOG_TASK_TypeDef *wwdgPtr );
void SetWatchDogCounter( WATCH_DOG_TASK_TypeDef *wwdgPtr,\
		uint32_t countTimeMS );
void CloseWatchDogTask( WATCH_DOG_TASK_TypeDef *wwdgPtr );
void RegisterWatchDogTaskTimeUpNotify_CallBack( WATCH_DOG_TASK_TypeDef *wwdgPtr,\
		void (*timeUpNotify_CallBack)( void *devicePrt ) );
void WatchDogTaskCycle( WATCH_DOG_TASK_TypeDef *wwdgPtr );



#endif
/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
