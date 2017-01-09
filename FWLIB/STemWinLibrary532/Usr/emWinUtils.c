/*
*********************************************************************************************************
* @file    	emWinUtils.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-10-31 15:48:55
* @IDE	 	Keil V5.18.0.0
* @Chip    	STM32F407VE
* @brief   	emWin���֧��Դ�ļ�
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
extern uint8_t 	g_PicBuff[PIC_BUFF_SIZE];

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
*                              				BMP
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*	�� �� ��: BMP_GetData
*	����˵��: ������GUI_BMP_DrawEx()����
*	��    �Σ�p             FIL��������
*             NumBytesReq   �����ȡ���ֽ���
*             ppData        ����ָ��
*             Off           ���Off = 1����ô�����´���ʵλ�ö�ȡ                 
*	�� �� ֵ: ���ض�ȡ���ֽ���
*********************************************************************************************************
*/
static int BMP_GetData(void * p, const U8 ** ppData, unsigned NumBytesReq, U32 Off) 
{
	static int FileAddress = 0;
	uint32_t NumBytesRead = 0;
	FIL *PicFile;
	uint32_t i;
	uint32_t bw;

	PicFile = (FIL *)p;

	/*
	* ��⻺���С
	*/
	if (NumBytesReq > sizeof(g_PicBuff)) {
	NumBytesReq = sizeof(g_PicBuff);
	}

	/*
	* ���ö�ȡλ��
	*/
	if(Off == 1) FileAddress = 0;
	else FileAddress = Off;
	FRESULT result =f_lseek(PicFile, FileAddress);
	
	/*
	* ��ȡ���ݵ�����, ����FatFS+�ٷ�SD���ķ�������DMA�����ϵ�4�ֽڶ������⣬������С�ڵ���һ��
	* SD��������С������������512�ֽڻ����
	*/
	for(i = 0; i < NumBytesReq / 512; i++)
	{
		result = f_read(PicFile, &g_PicBuff[512*i], 512, &bw);
		NumBytesRead += bw;
	}
	
	result = f_read(PicFile, &g_PicBuff[512*i], NumBytesReq % 512, &bw);
	NumBytesRead += bw;

	/*
	* ��ָ��ppDataָ���ȡ�ĺ���
	*/
	*ppData = (const uint8_t *)&g_PicBuff[0];

	/*
	* ���ض�ȡ���ֽ���
	*/
	return NumBytesRead;
}

/*
*********************************************************************************************************
*	�� �� ��: emWinShowBMPEx
*	����˵��: ��ʾBMPͼƬ
*	��    ��: sFilename Ҫ��ʾͼƬ������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void emWinShowBMPEx(uint16_t x, uint16_t y, const char * sFilename) 
{		
	FIL file;
	
    /* ���ļ� */		
	FRESULT result = f_open(&file, sFilename, FA_OPEN_EXISTING | FA_READ | FA_OPEN_ALWAYS);
	if (result != FR_OK)
	{
		return;
	}
	
	GUI_BMP_DrawEx(BMP_GetData, &file, x, y);

	f_close(&file);
}

/*
*********************************************************************************************************
*	�� �� ��: _WriteByte2File()
*	����˵��: д�ļ���SD�����������洢����
*	��    ��: Data Ҫд�����ݣ� p ָ��FIL���ͱ���      	
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void _WriteByte2File(U8 Data, void * p) 
{
	uint32_t bw;
	
	FRESULT result = f_write (p, &Data, 1, &bw);
}

/*
*********************************************************************************************************
* Function Name : emWinSaveBMPEx
* Description	: ����BMPͼƬ
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
bool emWinSaveBMPEx( const char * filePath )
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
	
	/* ����BMPͼƬ */
	GUI_BMP_Serialize(_WriteByte2File, &file);
	
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
*	�� �� ��: JPG_GetData
*	����˵��: ������GUI_JPEG_DrawEx()����
*	��    �Σ�p             FIL��������
*             NumBytesReq   �����ȡ���ֽ���
*             ppData        ����ָ��
*             Off           ���Off = 1����ô�����´���ʵλ�ö�ȡ                 
*	�� �� ֵ: ���ض�ȡ���ֽ���
*********************************************************************************************************
*/
static int JPG_GetData(void * p, const U8 ** ppData, unsigned NumBytesReq, U32 Off) 
{
	static int FileAddress = 0;
	UINT NumBytesRead;
	FIL *PicFile;

	PicFile = (FIL *)p;

	/*
	* ��⻺���С
	*/
	if (NumBytesReq > sizeof(g_PicBuff)) {
	NumBytesReq = sizeof(g_PicBuff);
	}

	/*
	* ���ö�ȡλ��
	*/
	if(Off == 1) FileAddress = 0;
	else FileAddress = Off;
	FRESULT result =f_lseek(PicFile, FileAddress);

	/*
	* ��ȡ���ݵ�����
	*/
	result = f_read(PicFile, g_PicBuff, NumBytesReq, &NumBytesRead);

	/*
	* ��ָ��ppDataָ���ȡ�ĺ���
	*/
	*ppData = (const U8 *)g_PicBuff;

	/*
	* ���ض�ȡ���ֽ���
	*/
	return NumBytesRead;
}

/*
*********************************************************************************************************
*	�� �� ��: emWinShowJPEGEx
*	����˵��: ��ʾJPEGͼƬ(��Ҫ��emWin����80KByte�ڴ棬����������ʾ)
*	��    �Σ�sFilename Ҫ��ʾͼƬ������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void emWinShowJPEGEx(uint16_t x, uint16_t y, const char * sFilename) 
{	
	FIL file;
	GUI_JPEG_INFO JpegInfo;
	static int32_t ret = 0;

	/* ���ļ� */		
	FRESULT result = f_open(&file, sFilename, FA_OPEN_EXISTING | FA_READ | FA_OPEN_ALWAYS);
	if (result != FR_OK)
	{
		return;
	}
	
	ret = GUI_JPEG_GetInfoEx(JPG_GetData, &file, &JpegInfo);
	if (ret)
	{
		f_close(&file);
		
		return;
	}
	GUI_JPEG_DrawScaledEx(JPG_GetData, 
						  &file, 
						  x,
						  y,
						  100,
						  100);

	f_close(&file);
}

/*
*********************************************************************************************************
*                              				GIF
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*	�� �� ��: GIF_GetData
*	����˵��: ������GUI_JPEG_DrawEx()����
*	��    ��: p             FIL��������
*             NumBytesReq   �����ȡ���ֽ���
*             ppData        ����ָ��
*             Off           ���Off = 1����ô�����´���ʼλ�ö�ȡ                 
*	�� �� ֵ: ���ض�ȡ���ֽ���
*********************************************************************************************************
*/
static int GIF_GetData(void * p, const U8 ** ppData, unsigned NumBytesReq, U32 Off) 
{
	static int FileAddress = 0;
	UINT NumBytesRead;
	FIL *PicFile;

	PicFile = (FIL *)p;

	/*
	* ��⻺���С
	*/
	if (NumBytesReq > sizeof(g_PicBuff)) {
	NumBytesReq = sizeof(g_PicBuff);
	}

	/*
	* ���ö�ȡλ��
	*/
	if(Off == 1) FileAddress = 0;
	else FileAddress = Off;
	FRESULT result =f_lseek(PicFile, FileAddress);

	/*
	* ��ȡ���ݵ�����
	*/
	result = f_read(PicFile, g_PicBuff, NumBytesReq, &NumBytesRead);

	/*
	* ��ָ��ppDataָ���ȡ�ĺ���
	*/
	*ppData = (const U8 *)g_PicBuff;

	/*
	* ���ض�ȡ���ֽ���
	*/
	return NumBytesRead;
}

/*
*********************************************************************************************************
*	�� �� ��: ShowGIFEx
*	����˵��: GIFͼƬ��ʾ
*	��    ��: sFilename �ļ���
*	�� �� ֵ: ��
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

	/* ���ļ� */		
	result = f_open(&file, sFilename, FA_OPEN_EXISTING | FA_READ | FA_OPEN_ALWAYS);
	if (result != FR_OK)
	{
		return;
	}
   
	/* ��ȡGIFͼƬ��Ϣ */
   GUI_GIF_GetInfoEx(GIF_GetData, &file,&InfoGif);
	
   while(1)
   {
	    /* �����������õ�ǰ���ŵ�֡����InfoGif.NumImages��GIFͼƬ�ܵ�֡�� */
		if(i < InfoGif.NumImages)
	    {
			/* ��ȡ��ǰ֡GIFͼƬ��Ϣ��ע���4�������Ǵ�0��ʼ������ */
			GUI_GIF_GetImageInfoEx(GIF_GetData, &file, &ImagInfoGif, i);
			
			/* �����֡�ӳ�ʱ����0��Ĭ�����ӳ�100ms */
			if(ImagInfoGif.Delay == 0)
			{
				GUI_Delay(100);
			}
			else
			{
				t0 = GUI_GetTime();
				
				/* ���벢��ʾ��֡GIFͼƬ��ע���5�������Ǵ�0��ʼ������ */
				GUI_GIF_DrawSubEx(GIF_GetData, 
								  &file, 
								  x,
								  y, 
								  i++);
				
				/* ��ȡ���ν������ʾ���ĵ�ʱ�� */
				t1 = GUI_GetTime() - t0;
				
				/* ���GIF�Ľ������ʾ��ʱ�䳬ʱ�Ͳ����ӳ� */
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
*	�� �� ��: _GetData
*	����˵��: ������GUI_PNG_DrawEx����
*	��    ��: p             FIL��������
*             NumBytesReq   �����ȡ���ֽ���
*             ppData        ����ָ��
*             Off           ���Off = 1����ô�����´���ʵλ�ö�ȡ                 
*	�� �� ֵ: ���ض�ȡ���ֽ���
*********************************************************************************************************
*/
static int PNG_GetData(void * p, const U8 ** ppData, unsigned NumBytesReq, U32 Off) 
{
	static int FileAddress = 0;
	FIL *file;
	UINT    NumBytesRead;
	U8     * pData;

	pData  = (U8 *)*ppData;
	file = (FIL *)p;
	
	//
	// �������ݶ�ȡλ��
	//
	if(Off == 1) FileAddress = 0;
	else FileAddress = Off;
	FRESULT result =f_lseek(file, FileAddress);
	//
	// ��ȡ���ݵ�����
	//
	result = f_read(file, pData, NumBytesReq, &NumBytesRead);
	
	//
	// ���ض�ȡ��С
	//
	return NumBytesRead;

}
/*
*********************************************************************************************************
*	�� �� ��: _ShowPNG
*	����˵��: ��ʾPNGͼƬ��emWin��Ҫ����ܴ�洢�ռ䣩
*	��    ��: sFilename Ҫ��ʾ��ͼƬ����
*             usPOSX    ��ʾλ��X
*             usPOSY    ��ʾλ��Y
*	�� �� ֵ: ��
*********************************************************************************************************
*/
bool ShowPNGEx(uint16_t usPOSX, uint16_t usPOSY, const char * sFilename) 
{	
	FIL file;
	int32_t ret;
	
	/* ���ļ� */		
	FRESULT result = f_open(&file, sFilename, FA_OPEN_EXISTING | FA_READ);
	if (result != FR_OK)
	{
		return false;
	}
	
	ret = GUI_PNG_DrawEx(PNG_GetData, &file, usPOSX, usPOSY);

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
*	�� �� ��: XBF_GetFontData_CallBack
*	����˵��: ʹ��XBF����
*	��    ��: Off       Ҫ��ȡ���ݵ���ʼλ��
*             NumBytes  Ҫ��ȡ���ֽ���
*             pVoid     Ӧ�ö���ָ�룬�˴����ļ����ָ��
*             pBuffer   ���ڴ洢�������ݵĵ�ַ
*	�� �� ֵ: 0 �ɹ�  1 ʧ��
*********************************************************************************************************
*/
static int XBF_GetFontData_CallBack(U32 Off, U16 NumBytes, void * pVoid, void * pBuffer)
{
	FIL *FontFile;
	uint32_t bw;
	
	/* �ļ���� */
	FontFile = (FIL *)pVoid;

	/*
	* ָ�����ݵ�ַ
	*/
	FRESULT result = f_lseek(FontFile, Off);
	if (result != FR_OK) 
	{
		return 1;
	}

	/*
	* ��ȡ���ݵ�buffer
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
*	�� �� ��: OpenXBF
*	����˵��: ��XBF����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
bool Open_XBF(void) 
{
	/* ���� 16X16 */
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

		/* ����XBF���� */
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
	
	/* ���� 24X24 */
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

		/* ����XBF���� */
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
	
	/* ���� 32X32 */
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

		/* ����XBF���� */
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
* Description	: ����FLASH�ֿ�
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
* Description	: ����FLASH�ֿ�
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
static bool UpdateFlashFont( void (*NotifyProgress_CallBack)( uint8_t fontSize, uint8_t progress ) )
{
	/* 16������ */
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
	
	/* 24������ */
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
* Description	: ���FLASH�ֿ��Ƿ����
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
* Description	: д��FLASH�ֿ�����
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
* Description	: ����FLASH����
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
