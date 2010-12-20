#ifndef IMGRW_H
#define IMGRW_H
#include "windows.h"
typedef int BOOL;
typedef unsigned char BYTE;
typedef unsigned int  UINT;

#define bool int

#ifndef FALSE
	#define FALSE 0
#endif

#ifndef false
	#define false 0
#endif

#ifndef TRUE
	#define TRUE (!0)
#endif

#ifndef true
	#define true !(0)
#endif


#ifndef ALV_BMPHEADER
	#define ALV_BMPHEADER

	typedef struct
	{
		unsigned short  bfType;
		unsigned long	bfSize;
		unsigned short	bfReserved1;
		unsigned short	bfReserved2;
		unsigned long	bfOffBits;
	}alvBITMAPFILEHEADER;

	typedef struct
	{
		unsigned long	biSize;
		unsigned long	biWidth;
		unsigned long	biHeight;
		unsigned short	biPlanes;
		unsigned short	biBitCount;

		unsigned long	biCompression;

		unsigned long	biSizeImage;
		long			biXPelsPerMeter;
		long			biYPelsPerMeter;
		unsigned long	biClrUsed;
		unsigned long	biClrImportant;
	}alvBITMAPINFOHEADER;

	typedef struct
	{
		unsigned char	rgbBlue;
		unsigned char	rgbGreen;
		unsigned char	rgbRed;
		unsigned char	rgbReserved;
	}alvRGBQUAD;

	typedef struct 
	{
		alvBITMAPINFOHEADER bmiHeader; 
		alvRGBQUAD bmiColors[1];
	}alvBITMAPINFO;

	typedef struct
	{
		alvBITMAPINFOHEADER bBitHeader;
		void* lpvBits;
	}aluBITMAP;

#endif
class BMPWrapper
{
public:
	static BOOL BW_BMP_Read(const char* BMP_filename, BYTE* BMP_Img, int width, int height);
	static BOOL BW_BMP_Write(const char* BMP_filename, BYTE* BMP_Img, int width, int height);
	static void GetDefaultBitInfo(BITMAPINFO** bitmapinfo);
};

// 位图必须为8为深度的灰度图片

#endif