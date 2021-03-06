/*
*********************************************************************************************************
* @file    	shell.c
* @author  	SY
* @version 	V1.0.0
* @date    	2017-1-13 10:47:04
* @IDE	 	Keil V5.22.0.0
* @Chip    	STM32F407VE
* @brief   	SHELL命令解析引擎源文件
*********************************************************************************************************
* @attention
*
* 
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Private Includes
*********************************************************************************************************
*/
#include "kernel.h"


/*
*********************************************************************************************************
*                              				Private define
*********************************************************************************************************
*/
static void CmdList_handler(const char *para);
static void CmdFormat_handler(const char *para);

static void HelpCmd_handler(const char *para);
static void HelpCmdFormat_handler(const char *para);

static void PowerOnTime_handler(const char *para);
static void TimeCmdFormat_handler(const char *para);

static void Uname_handler(const char *para);
static void UnameCmdFormat_handler(const char *para);

static void Status_handler(const char *para);
static void StatusCmdFormat_handler(const char *para);

static void ProcessSnapshot_handler(const char *para);
static void ProcessSnapshotCmdFormat_handler(const char *para);

static void Reboot_handler(const char *para);
static void RebootCmdFormat_handler(const char *para);


/*
*********************************************************************************************************
*                              				Private typedef
*********************************************************************************************************
*/
typedef struct {
	const char *cmd;
	uint8_t paraNums;
	void (*handler_CallBack)(const char *para);
	void (*cmdFormat_CallBack)(const char *para);
}SHELL_TypeDef;


/*
*********************************************************************************************************
*                              				Private function prototypes
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                              				Private constants
*********************************************************************************************************
*/
static const SHELL_TypeDef g_Shell[] = {
	{
		"help",
		1,
		HelpCmd_handler,
		HelpCmdFormat_handler,
	},
	{
		"cmd",
		0,
		CmdList_handler,
		CmdFormat_handler,
	},	
	{
		"time",
		0,
		PowerOnTime_handler,
		TimeCmdFormat_handler,
	},
	{
		"uname",
		0,
		Uname_handler,
		UnameCmdFormat_handler,
	},
	{
		"status",
		0,
		Status_handler,
		StatusCmdFormat_handler,
	},
	{
		"ps",
		0,
		ProcessSnapshot_handler,
		ProcessSnapshotCmdFormat_handler,
	},
	{
		"reboot",
		0,
		Reboot_handler,
		RebootCmdFormat_handler,
	},
};


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
*                              				Private functions
*********************************************************************************************************
*/
/*
*********************************************************************************************************
* Function Name : isCmdExist
* Description	: 命令是否存在
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static bool isCmdExist(const char *cmd)
{
	for (uint32_t i=0; i<ARRAY_SIZE(g_Shell); ++i)
	{
		if (strcmp(g_Shell[i].cmd, cmd) == 0)
		{
			return true;
		}
	}
	
	return false;
}

/*
*********************************************************************************************************
* Function Name : GetCmdIndex
* Description	: 获取命令索引值
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static bool GetCmdIndex(const char *cmd, uint32_t *index)
{
	for (uint32_t i=0; i<ARRAY_SIZE(g_Shell); ++i)
	{
		if (strcmp(g_Shell[i].cmd, cmd) == 0)
		{
			*index = i;
			
			return true;
		}
	}
	
	return false;
}

/*
*********************************************************************************************************
* Function Name : AppTaskShell
* Description	: Shell任务
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void AppTaskShell(void *p_arg)
{
	(void)p_arg;
	
	while (1)
	{		
		char stringBuffer[128] = {0};
		
		scanf("%s", stringBuffer);
		if (isCmdExist(stringBuffer) == false)
		{
			ECHO(DEBUG_KERNEL_SHELL, "未找到命令！");
			ECHO(DEBUG_KERNEL_SHELL, "输入 <cmd> 查看支持的命令！");
			continue;
		}
		
		uint32_t cmdIndex = 0;
		if (GetCmdIndex(stringBuffer, &cmdIndex) == false)
		{
			continue;
		}
		
		const SHELL_TypeDef *shell = &g_Shell[cmdIndex];
		uint32_t rxParaNums = 0;
		while (1)
		{
			if (rxParaNums >= shell->paraNums)
			{
				break;
			}
			
			char paraBuffer[32] = {0};
			if (scanf("%s", paraBuffer) != 1)
			{
				break;
			}
			
			strcat(stringBuffer, " ");
			strcat(stringBuffer, paraBuffer);	
			rxParaNums++;
		}
		
		if (g_Shell[cmdIndex].handler_CallBack)
		{
			g_Shell[cmdIndex].handler_CallBack(stringBuffer);
		}
	}
}

/*
*********************************************************************************************************
* Function Name : CmdList_handler
* Description	: 处理命令列表
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void CmdList_handler(const char *para)
{
	ECHO(DEBUG_KERNEL_SHELL, "命令列表：");	
	for (uint32_t i=0; i<ARRAY_SIZE(g_Shell); ++i)
	{
		ECHO(DEBUG_KERNEL_SHELL, "%s", g_Shell[i].cmd);		
	}
}

/*
*********************************************************************************************************
* Function Name : CmdFormat_handler
* Description	: 处理命令格式
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void CmdFormat_handler(const char *para)
{
	ECHO(DEBUG_KERNEL_SHELL, "命令格式：cmd");	
}

/*
*********************************************************************************************************
* Function Name : HelpCmd_handler
* Description	: 处理帮助命令
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void HelpCmd_handler(const char *para)
{
	char cmdBuff1[32] = {0};
	char cmdBuff2[32] = {0}; 
	int argc = 0;
	argc = sscanf(para, "%s %s", cmdBuff1, cmdBuff2);
	
	if (argc == 2)
	{
		bool isFindCmd = false;
		for (uint32_t i=0; i<ARRAY_SIZE(g_Shell); ++i)
		{
			if (strcmp(g_Shell[i].cmd, cmdBuff2) == 0)
			{
				isFindCmd = true;
				if (g_Shell[i].cmdFormat_CallBack)
				{
					g_Shell[i].cmdFormat_CallBack(cmdBuff2);
				}
			}
		}
		
		if (isFindCmd == false)
		{
			ECHO(DEBUG_KERNEL_SHELL, "未找到命令！");
		}
	}
	else
	{
		ECHO(DEBUG_KERNEL_SHELL, "输入格式错误！");
	}
}

/*
*********************************************************************************************************
* Function Name : HelpCmdFormat_handler
* Description	: 处理帮助命令格式
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void HelpCmdFormat_handler(const char *para)
{
	ECHO(DEBUG_KERNEL_SHELL, "命令格式：help + [cmd]");	
}

/*
*********************************************************************************************************
* Function Name : PowerOnTime_handler
* Description	: 处理开机时间
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void PowerOnTime_handler(const char *para)
{
	char timeBuff[32] = {0};
	tTime time;
	GetSystemTime(&time);
	
	sprintf(timeBuff, "开机时间：%04d-%02d-%02d %02d:%02d:%02d", \
			time.usYear, time.ucMon, time.ucMday, time.ucHour, time.ucMin, time.ucSec);
	ECHO(DEBUG_KERNEL_SHELL, timeBuff);
}

/*
*********************************************************************************************************
* Function Name : TimeCmdFormat_handler
* Description	: 处理时间命令格式
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void TimeCmdFormat_handler(const char *para)
{
	ECHO(DEBUG_KERNEL_SHELL, "命令格式：time");	
}

/*
*********************************************************************************************************
* Function Name : Uname_handler
* Description	: 处理系统信息
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void Uname_handler(const char *para)
{
	void PrintControllerInfo(void);
	
	PrintControllerInfo();
}

/*
*********************************************************************************************************
* Function Name : UnameCmdFormat_handler
* Description	: 处理系统信息命令格式
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void UnameCmdFormat_handler(const char *para)
{
	ECHO(DEBUG_KERNEL_SHELL, "命令格式：uname");	
}

/*
*********************************************************************************************************
* Function Name : Status_handler
* Description	: 处理状态信息
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void Status_handler(const char *para)
{
	ECHO(DEBUG_KERNEL_SHELL, "Run...");	
}

/*
*********************************************************************************************************
* Function Name : StatusCmdFormat_handler
* Description	: 处理状态命令格式
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void StatusCmdFormat_handler(const char *para)
{
	ECHO(DEBUG_KERNEL_SHELL, "命令格式：status");	
}

/*
*********************************************************************************************************
* Function Name : ProcessSnapshot_handler
* Description	: 处理进程快照
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void ProcessSnapshot_handler(const char *para)
{
	PrintProcessSnapshot();
}

/*
*********************************************************************************************************
* Function Name : ProcessSnapshotCmdFormat_handler
* Description	: 处理进程快照命令格式
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void ProcessSnapshotCmdFormat_handler(const char *para)
{
	ECHO(DEBUG_KERNEL_SHELL, "命令格式：ps");	
}

/*
*********************************************************************************************************
* Function Name : Reboot_handler
* Description	: 处理重启控制器
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void Reboot_handler(const char *para)
{
	SystemSoftwareReset();
}

/*
*********************************************************************************************************
* Function Name : RebootCmdFormat_handler
* Description	: 处理重启命令格式
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static void RebootCmdFormat_handler(const char *para)
{
	ECHO(DEBUG_KERNEL_SHELL, "命令格式：reboot");	
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
