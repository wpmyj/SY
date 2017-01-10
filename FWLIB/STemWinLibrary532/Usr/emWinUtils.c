/*
*********************************************************************************************************
* @file    	emWinUtils.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-10-31 15:48:55
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	emWin相关支持源文件
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
#include "bsp.h"
#include "emWinUtils.h"
#include "file.h"


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

#if (USE_XBF_FONT)
FIL 			FIL_FontSongTi_16X16;
GUI_FONT 		GUI_FontSongTi_16X16;
GUI_XBF_DATA 	GUI_XBF_DataSongTi_16X16;

FIL 			FIL_FontSongTi_24X24;
GUI_FONT 		GUI_FontSongTi_24X24;
GUI_XBF_DATA 	GUI_XBF_DataSongTi_24X24;

FIL 			FIL_FontSongTi_32X32;
GUI_FONT 		GUI_FontSongTi_32X32;
GUI_XBF_DATA 	GUI_XBF_DataSongTi_32X32;
#endif

const uint32_t ALLOC_PIC_MEM_SIZE 			= 4*1024;
static GUI_HMEM hPicMem;
static char *_acPicBuffer;
#define PIC_ALLOC_MEM()						do {\
												/* 申请一块内存空间 并且将其清零    */\
												hPicMem = GUI_ALLOC_AllocZero(ALLOC_PIC_MEM_SIZE);\
												/* 将申请到内存的句柄转换成指针类型 */\
												_acPicBuffer = (char *)GUI_ALLOC_h2p(hPicMem);\
											} while(0)
#define PIC_FREE_MEM()						do {\
												GUI_ALLOC_Free(hPicMem);\
											} while(0)


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
*	函 数 名: _GetData
*	功能说明: 被函数GUI_BMP_DrawEx()调用
*	形    参：p             FIL类型数据
*             NumBytesReq   请求读取的字节数
*             ppData        数据指针
*             Off           如果Off = 1，那么将重新从其实位置读取                 
*	返 回 值: 返回读取的字节数
*********************************************************************************************************
*/
static int _GetData(void * p, const U8 ** ppData, unsigned NumBytesReq, U32 Off) 
{
	static int FileAddress = 0;
	uint32_t NumBytesRead = 0;
	FIL *PicFile;
	uint32_t i;
	uint32_t bw;

	PicFile = (FIL *)p;

	/*
	* 检测缓存大小
	*/
	if (NumBytesReq > ALLOC_PIC_MEM_SIZE) {
		NumBytesReq = ALLOC_PIC_MEM_SIZE;
	}

	/*
	* 设置读取位置
	*/
	if(Off == 1) FileAddress = 0;
	else FileAddress = Off;
	FRESULT result =f_lseek(PicFile, FileAddress);
	
	/*
	* 读取数据到缓存, 由于FatFS+官方SD卡的方案存在DMA传输上的4字节对齐问题，这里以小于等于一个
	* SD卡扇区大小来操作，超过512字节会出错。
	*/
	for(i = 0; i < NumBytesReq / 512; i++)
	{
		result = f_read(PicFile, (_acPicBuffer+512*i), 512, &bw);
		NumBytesRead += bw;
	}
	
	result = f_read(PicFile, (_acPicBuffer+512*i), NumBytesReq % 512, &bw);
	NumBytesRead += bw;

	/*
	* 让指针ppData指向读取的函数
	*/
	*ppData = (const uint8_t *)_acPicBuffer;

	/*
	* 返回读取的字节数
	*/
	return NumBytesRead;
}

/*
*********************************************************************************************************
*                              				BMP
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*	函 数 名: ShowBMPEx
*	功能说明: 显示BMP图片
*	形    参: sFilename 要显示图片的名字
*	返 回 值: 无
*********************************************************************************************************
*/
void ShowBMPEx(uint16_t x, uint16_t y, const char * sFilename) 
{		
	FIL file;
	
    /* 打开文件 */		
	FRESULT result = f_open(&file, sFilename, FA_OPEN_EXISTING | FA_READ | FA_OPEN_ALWAYS);
	if (result != FR_OK)
	{
		return;
	}
	
	PIC_ALLOC_MEM();
	GUI_BMP_DrawEx(_GetData, &file, x, y);
	PIC_FREE_MEM();

	f_close(&file);
}

/*
*********************************************************************************************************
*	函 数 名: _WriteByte2File()
*	功能说明: 写文件到SD卡或者其他存储介质
*	形    参: Data 要写的数据， p 指向FIL类型变量      	
*	返 回 值: 无
*********************************************************************************************************
*/
static void _WriteByte2File(U8 Data, void * p) 
{
	uint32_t bw;
	
	FRESULT result = f_write (p, &Data, 1, &bw);
}

/*
*********************************************************************************************************
* Function Name : SaveBMPEx
* Description	: 保存BMP图片
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
bool SaveBMPEx( const char * filePath )
{
	FRESULT result = CreatFolder(filePath);
	if ((result != FR_OK) && (result != FR_EXIST))
	{
		return false;
	}
	
	FIL file;
	result = f_open(&file, filePath, FA_WRITE|FA_CREATE_ALWAYS);
	if (result != FR_OK)
	{
		return false;
	}
	
	PIC_ALLOC_MEM();
	/* 绘制BMP图片 */
	GUI_BMP_Serialize(_WriteByte2File, &file);
	PIC_FREE_MEM();
	
	result = f_close(&file);
	if (result != FR_OK)
	{
		return false;
	}
	
	return true;
}

/*
*********************************************************************************************************
*                              				JPG
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*	函 数 名: ShowJPEGEx
*	功能说明: 显示JPEG图片(需要给emWin分配80KByte内存，才能正常显示)
*	形    参：sFilename 要显示图片的名字
*	返 回 值: 无
*********************************************************************************************************
*/
void ShowJPEGEx(uint16_t x, uint16_t y, const char * sFilename) 
{	
	FIL file;
	GUI_JPEG_INFO JpegInfo;
	static int32_t ret = 0;

	/* 打开文件 */		
	FRESULT result = f_open(&file, sFilename, FA_OPEN_EXISTING | FA_READ | FA_OPEN_ALWAYS);
	if (result != FR_OK)
	{
		return;
	}
	
	PIC_ALLOC_MEM();
	ret = GUI_JPEG_GetInfoEx(_GetData, &file, &JpegInfo);
	if (ret)
	{
		PIC_FREE_MEM();
		f_close(&file);
		
		return;
	}
	GUI_JPEG_DrawScaledEx(_GetData, 
						  &file, 
						  x,
						  y,
						  100,
						  100);

	f_close(&file);
	
	PIC_FREE_MEM();
}

/*
*********************************************************************************************************
*                              				GIF
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*	函 数 名: ShowGIFEx
*	功能说明: GIF图片显示
*	形    参: sFilename 文件名
*	返 回 值: 无
*********************************************************************************************************
*/
void ShowGIFEx(uint16_t x, uint16_t y, const char * sFilename)
{
	uint16_t i = 0;
	uint32_t t0, t1;
	FIL file;
	FRESULT result;
	GUI_GIF_INFO InfoGif;
	GUI_GIF_IMAGE_INFO ImagInfoGif;

	/* 打开文件 */		
	result = f_open(&file, sFilename, FA_OPEN_EXISTING | FA_READ | FA_OPEN_ALWAYS);
	if (result != FR_OK)
	{
		return;
	}
   
	PIC_ALLOC_MEM();
	
	/* 获取GIF图片信息 */
   GUI_GIF_GetInfoEx(_GetData, &file,&InfoGif);
	
   while(1)
   {
	    /* 变量用来设置当前播放的帧数，InfoGif.NumImages是GIF图片总的帧数 */
		if(i < InfoGif.NumImages)
	    {
			/* 获取当前帧GIF图片信息，注意第4个参数是从0开始计数的 */
			GUI_GIF_GetImageInfoEx(_GetData, &file, &ImagInfoGif, i);
			
			/* 如果此帧延迟时间是0，默认是延迟100ms */
			if(ImagInfoGif.Delay == 0)
			{
				GUI_Delay(100);
			}
			else
			{
				t0 = GUI_GetTime();
				
				/* 解码并显示此帧GIF图片，注意第5个参数是从0开始计数的 */
				GUI_GIF_DrawSubEx(_GetData, 
								  &file, 
								  x,
								  y, 
								  i++);
				
				/* 获取本次解码和显示消耗的时间 */
				t1 = GUI_GetTime() - t0;
				
				/* 如果GIF的解码和显示的时间超时就不做延迟 */
				if (t1 < ImagInfoGif.Delay * 10) 
				{
					GUI_Delay(ImagInfoGif.Delay * 10 - t1);
				}
			}			  
	    }
	    else
	    {
	        i = 0;
	    }
	}

}

/*
*********************************************************************************************************
*                              				PNG
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*	函 数 名: _ShowPNG
*	功能说明: 显示PNG图片（emWin需要分配很大存储空间）
*	形    参: sFilename 要显示的图片名字
*             usPOSX    显示位置X
*             usPOSY    显示位置Y
*	返 回 值: 无
*********************************************************************************************************
*/
bool ShowPNGEx(uint16_t usPOSX, uint16_t usPOSY, const char * sFilename) 
{	
	FIL file;
	int32_t ret;
	
	/* 打开文件 */		
	FRESULT result = f_open(&file, sFilename, FA_OPEN_EXISTING | FA_READ | FA_OPEN_ALWAYS);
	if (result != FR_OK)
	{
		return false;
	}
	
	PIC_ALLOC_MEM();
	ret = GUI_PNG_DrawEx(_GetData, &file, usPOSX, usPOSY);
	PIC_FREE_MEM();
	
	f_close(&file);
	
	if (ret)
	{
		return false;
	}
	else
	{
		return true;
	}
}

/*
*********************************************************************************************************
*                              				Bitmap
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*	函 数 名: ShowBitmapEx
*	功能说明: 显示Bitmap图片
*	形    参: sFilename 要显示的图片名字
*             usPOSX    显示位置X
*             usPOSY    显示位置Y
*	返 回 值: 无
*********************************************************************************************************
*/
bool ShowBitmapEx(uint16_t usPOSX, uint16_t usPOSY, const char * sFilename) 
{	
	FIL file;
	int32_t ret;
	
	/* 打开文件 */		
	FRESULT result = f_open(&file, sFilename, FA_OPEN_EXISTING | FA_READ | FA_OPEN_ALWAYS);
	if (result != FR_OK)
	{
		return false;
	}
	
	PIC_ALLOC_MEM();
	ret = GUI_DrawStreamedBitmapExAuto(_GetData, &file, usPOSX, usPOSY);
	PIC_FREE_MEM();
	
	f_close(&file);
	
	if (ret)
	{
		return false;
	}
	else
	{
		return true;
	}
}

/*
*********************************************************************************************************
*                              				ICON
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*	函 数 名: ShowIconEx
*	功能说明: 显示ICON图片
*	形    参: sFilename 要显示的图片名字
*             usPOSX    显示位置X
*             usPOSY    显示位置Y
*	返 回 值: 无
*********************************************************************************************************
*/
bool ShowIconEx(ICONVIEW_Handle hIcon, const char * pText, const char * sFilename) 
{	
	FIL file;
	int32_t ret;
	
	/* 打开文件 */		
	FRESULT result = f_open(&file, sFilename, FA_OPEN_EXISTING | FA_READ | FA_OPEN_ALWAYS);
	if (result != FR_OK)
	{
		return false;
	}
	
	PIC_ALLOC_MEM();
	ret = ICONVIEW_AddBMPItemEx(hIcon, &file, _GetData, pText);
	PIC_FREE_MEM();
	
	f_close(&file);
	
	if (ret)
	{
		return false;
	}
	else
	{
		return true;
	}
}

/*
*********************************************************************************************************
*                              				FONTS
*********************************************************************************************************
*/
#if (USE_XBF_FONT)
/*
*********************************************************************************************************
*	函 数 名: XBF_GetFontData_CallBack
*	功能说明: 使用XBF字体
*	形    参: Off       要读取数据的起始位置
*             NumBytes  要读取的字节数
*             pVoid     应用定义指针，此处是文件句柄指针
*             pBuffer   用于存储字体数据的地址
*	返 回 值: 0 成功  1 失败
*********************************************************************************************************
*/
static int XBF_GetFontData_CallBack(U32 Off, U16 NumBytes, void * pVoid, void * pBuffer)
{
	FIL *FontFile;
	uint32_t bw;
	
	/* 文件句柄 */
	FontFile = (FIL *)pVoid;

	/*
	* 指向数据地址
	*/
	FRESULT result = f_lseek(FontFile, Off);
	if (result != FR_OK) 
	{
		return 1;
	}

	/*
	* 读取数据到buffer
	*/
	result = f_read(FontFile, pBuffer, NumBytes, &bw);
	if (result != FR_OK) 
	{
		return 1; 
	}
	
	return 0;

}

/*
*********************************************************************************************************
*	函 数 名: OpenXBF
*	功能说明: 打开XBF字体
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
bool Open_XBF(void) 
{
	/* 宋体 16X16 */
	{
		char path[50] = {0};
		const char *rootPath = bsp_GetSDRootPath();
		if (rootPath == NULL)
		{
			return false;
		}
		strcpy(path, rootPath);
		
		const char *FontPathPtr = "fonts/SongTi_16X16.xbf";
		strcat(path, FontPathPtr);
		
		FRESULT result = f_open(&FIL_FontSongTi_16X16, path, FA_OPEN_EXISTING | FA_READ | FA_OPEN_ALWAYS);
		if (result != FR_OK)
		{
			return false;
		}

		/* 创建XBF字体 */
		int ret = GUI_XBF_CreateFont(&GUI_FontSongTi_16X16,       
									 &GUI_XBF_DataSongTi_16X16,        
									 GUI_XBF_TYPE_PROP_AA4_EXT, 
									 XBF_GetFontData_CallBack,      
									 &FIL_FontSongTi_16X16);  
		if (ret)
		{
			return false;
		}
	}
	
	/* 宋体 24X24 */
	{
		char path[50] = {0};
		const char *rootPath = bsp_GetSDRootPath();
		if (rootPath == NULL)
		{
			return false;
		}
		strcpy(path, rootPath);
		
		const char *FontPathPtr = "fonts/SongTi_24X24.xbf";
		strcat(path, FontPathPtr);
		
		FRESULT result = f_open(&FIL_FontSongTi_24X24, path, FA_OPEN_EXISTING | FA_READ | FA_OPEN_ALWAYS);
		if (result != FR_OK)
		{
			return false;
		}

		/* 创建XBF字体 */
		int ret = GUI_XBF_CreateFont(&GUI_FontSongTi_24X24,       
									 &GUI_XBF_DataSongTi_24X24,        
									 GUI_XBF_TYPE_PROP_AA4_EXT, 
									 XBF_GetFontData_CallBack,      
									 &FIL_FontSongTi_24X24);  
		if (ret)
		{
			return false;
		}
	}
	
	/* 宋体 32X32 */
	{
		char path[50] = {0};
		const char *rootPath = bsp_GetSDRootPath();
		if (rootPath == NULL)
		{
			return false;
		}
		strcpy(path, rootPath);
		
		const char *FontPathPtr = "fonts/SongTi_32X32.xbf";
		strcat(path, FontPathPtr);
		
		FRESULT result = f_open(&FIL_FontSongTi_32X32, path, FA_OPEN_EXISTING | FA_READ | FA_OPEN_ALWAYS);
		if (result != FR_OK)
		{
			return false;
		}

		/* 创建XBF字体 */
		int ret = GUI_XBF_CreateFont(&GUI_FontSongTi_32X32,       
									 &GUI_XBF_DataSongTi_32X32,        
									 GUI_XBF_TYPE_PROP_AA4_EXT, 
									 XBF_GetFontData_CallBack,      
									 &FIL_FontSongTi_32X32);  
		if (ret)
		{
			return false;
		}
	}
	
	GUI_UC_SetEncodeUTF8();
	
	return true;
}

#endif

/*
*********************************************************************************************************
* Function Name : __UpdateFlashFont
* Description	: 更新FLASH字库
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static FRESULT __UpdateFlashFont(const char *const fontPathPtr, uint32_t wrStartAddr, uint16_t fontSize,\
								 void (*NotifyProgress_CallBack)( uint8_t fontSize, uint8_t progress ))
{
	FIL file_obj;  
	uint32_t br; 
	const uint16_t ONCE_READ_SIZE = 512;
	uint8_t fileBuff[ONCE_READ_SIZE];
	
	uint32_t writeAddr = wrStartAddr;
	uint32_t totalReadSize = 0;
	uint32_t totalWriteSize = 0;
	 	
	FRESULT fresult = f_open(&file_obj, fontPathPtr, FA_OPEN_EXISTING | FA_READ); 	
	if (fresult != FR_OK)
	{
		f_close(&file_obj);
		
		return fresult;
	}
	
	totalReadSize = f_size(&file_obj);	
	uint8_t progress = 0;
	
	do{
		fresult = f_read(&file_obj, fileBuff, ONCE_READ_SIZE, &br);  
		if (fresult != FR_OK)
		{
			break;
		}
		
		if (sf_WriteBuffer(fileBuff,writeAddr,br) == 0)
		{
			fresult = FR_DISK_ERR;
			break;
		}
		
		totalWriteSize += br;
		writeAddr += br;

		if (totalReadSize)
		{
			progress = ((float)totalWriteSize / totalReadSize) * 100;
		}		
		if (NotifyProgress_CallBack)
		{
			NotifyProgress_CallBack(fontSize, progress);
		}
	} while (br != 0);
	
	f_close(&file_obj);
	
	if (fresult != FR_OK)
	{
		return fresult;
	}
	
	if (totalReadSize != totalWriteSize)
	{
		return FR_DISK_ERR;
	}
	
	return FR_OK;
}

/*
*********************************************************************************************************
* Function Name : UpdateFlashFont
* Description	: 更新FLASH字库
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static bool UpdateFlashFont( void (*NotifyProgress_CallBack)( uint8_t fontSize, uint8_t progress ) )
{
	/* 16号字体 */
	{
		char path[50] = {0};
		const char *rootPath = bsp_GetSDRootPath();
		if (rootPath == NULL)
		{
			return false;
		}
		strcpy(path, rootPath);
		strcat(path, FONT_CH_SONGTI_16X16_PATH);	
		
		FRESULT result = __UpdateFlashFont(path, FONT_CH_SONGTI_16X16_ADDR, 16,\
											NotifyProgress_CallBack);
		if (result != FR_OK)
		{
			return false;
		}
	}
	
	/* 24号字体 */
	{
		char path[50] = {0};
		const char *rootPath = bsp_GetSDRootPath();
		if (rootPath == NULL)
		{
			return false;
		}
		strcpy(path, rootPath);
		strcat(path, FONT_CH_SONGTI_24X24_PATH);	
		
		FRESULT result = __UpdateFlashFont(path, FONT_CH_SONGTI_24X24_ADDR, 24,\
											NotifyProgress_CallBack);
		if (result != FR_OK)
		{
			return false;
		}
	}
	
	return true;
}

/*
*********************************************************************************************************
* Function Name : FlashFontIsExist
* Description	: 检测FLASH字库是否存在
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static bool FlashFontIsExist( void )
{
	uint32_t readFontPassword = 0;
	
	sf_ReadBuffer((uint8_t *)&readFontPassword, FONT_PASSWORD_ADDR, sizeof(FLASH_FONT_PASSWORD));
	
	if (readFontPassword == FLASH_FONT_PASSWORD)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*
*********************************************************************************************************
* Function Name : WriteFlashFontPassword
* Description	: 写入FLASH字库密码
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static bool WriteFlashFontPassword( void )
{
	uint32_t writeFontPassword = FLASH_FONT_PASSWORD;
	uint32_t readFontPassword = 0;
	
	if ( sf_WriteBuffer((uint8_t *)&writeFontPassword, FONT_PASSWORD_ADDR, sizeof(FLASH_FONT_PASSWORD)) == 1)
	{
		sf_ReadBuffer((uint8_t *)&readFontPassword, FONT_PASSWORD_ADDR, sizeof(FLASH_FONT_PASSWORD));
		if (readFontPassword == writeFontPassword)
		{
			return true;
		}
	}
	
	return false;
}

/*
*********************************************************************************************************
* Function Name : UpdateFlashFontTask
* Description	: 更新FLASH任务
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void UpdateFlashFontTask( void (*NotifyProgress_CallBack)( uint8_t fontSize, uint8_t progress ) )
{
	if (FlashFontIsExist() == true)
	{
		return;
	}
	
	if (UpdateFlashFont(NotifyProgress_CallBack) == false)
	{
		goto FileException;
	}
	
	if (WriteFlashFontPassword() == false)
	{
		goto FileException;
	}
	
	return;

FileException:	
{
	while (1);		
}

}


/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/
