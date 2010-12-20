#include "ImgRW.h"
#include <stdio.h>
#include <memory.h>

BOOL BMPWrapper::BW_BMP_Read(const char* BMP_filename, unsigned char * BMP_Img, int width, int height)
{
	int i;
	alvBITMAPFILEHEADER bmfh;
	alvBITMAPINFOHEADER bmih;
	alvRGBQUAD rgbQuad[256];

    FILE *fp_BMP;
	BYTE *pImgtmp;

	if((fp_BMP=fopen(BMP_filename,"rb"))==NULL)
	{
		printf("can't opened the file '%s' !\n",BMP_filename);
		printf("Press any key to continue! \n");
		getchar();
		return FALSE;
	}

	fread(&bmfh.bfType,sizeof(unsigned short),1,fp_BMP);
	if(bmfh.bfType != (unsigned short)0x4D42)
	{
		printf("the file '%s' is not BMP file!\n",BMP_filename);
		printf("Press any key to continue! \n");
		getchar();
		return FALSE;
	}
	fread(&bmfh.bfSize,sizeof(unsigned long),1,fp_BMP);
	fread(&bmfh.bfReserved1,sizeof(unsigned short),1,fp_BMP);
	fread(&bmfh.bfReserved2,sizeof(unsigned short),1,fp_BMP);
	fread(&bmfh.bfOffBits,sizeof(unsigned long),1,fp_BMP);
	
	fread((char*)&bmih,sizeof(alvBITMAPINFOHEADER),1,fp_BMP);
	if(bmih.biWidth != (UINT)width)
	{
		printf("\nERROR:\n   image's width: %ld doesn't consistent with user definition: %d!\n",bmih.biWidth,width);
		printf("Press any key to continue! \n");
		getchar();
		return FALSE;
	}
	if(bmih.biHeight != (UINT)height)
	{
		printf("Press any key to continue! \n");
		getchar();
		return FALSE;
	}
	if(bmih.biBitCount != 8)
	{
		printf("\nERROR:\n    the image isn't 8 bits!\n");
		printf("Press any key to continue! \n");
		getchar();
		return FALSE;
	}
	fread((char*)rgbQuad,sizeof(alvRGBQUAD),256,fp_BMP);

	//逆向读取图片数据
	pImgtmp = BMP_Img+width*(height-1);
	for(i=height-1; i>=0; i--)
	{
		fread(pImgtmp,sizeof(unsigned char),width,fp_BMP);
		pImgtmp -= width;
	}

	fclose(fp_BMP);	
	return TRUE;
}

BOOL BMPWrapper::BW_BMP_Write(const char* BMP_filename, BYTE* BMP_Img, int width, int height)
{
	alvBITMAPFILEHEADER bmfh;
	alvBITMAPINFOHEADER bmih;
	alvRGBQUAD rgbQuad[256];
	int biWidth = (int)((width+3)/4*4);
	int img_size = biWidth * height; 

	FILE *fp_BMP;
	int i;
	BYTE *pImgtmp;

    bmfh.bfType = (unsigned short)0x4D42;
	bmfh.bfSize = (unsigned long)(14+40+4*256+img_size);
	bmfh.bfReserved1 = (unsigned short)0;
	bmfh.bfReserved2 = (unsigned short)0;
	bmfh.bfOffBits = (unsigned long)(14+40+4*256);
    
	bmih.biSize = (unsigned long)40;
	bmih.biWidth  = (long)width;
	bmih.biHeight = (long)height;
	bmih.biPlanes = (unsigned short)1;
	bmih.biBitCount = (unsigned short)8;
	bmih.biCompression = (unsigned long)0;
	bmih.biSizeImage = (unsigned long)img_size;
	bmih.biXPelsPerMeter = (long)0;
	bmih.biYPelsPerMeter = (long)0;
	bmih.biClrUsed = (unsigned long)0;
	bmih.biClrImportant = (unsigned long)0;

    for(i=0;i<256;i++)
	{
		rgbQuad[i].rgbBlue     = (BYTE)i;
		rgbQuad[i].rgbRed      = (BYTE)i;
		rgbQuad[i].rgbGreen    = (BYTE)i;
		rgbQuad[i].rgbReserved = (BYTE)0;
	}

    if((fp_BMP=fopen(BMP_filename,"wb"))==NULL)
	{
		printf("the %s cannot opened!\n",BMP_filename);
		return FALSE;
	}

    fwrite(&bmfh.bfType,sizeof(unsigned short),1,fp_BMP);
	fwrite(&bmfh.bfSize,sizeof(unsigned long),1,fp_BMP);
	fwrite(&bmfh.bfReserved1,sizeof(unsigned short),1,fp_BMP);
	fwrite(&bmfh.bfReserved2,sizeof(unsigned short),1,fp_BMP);
	fwrite(&bmfh.bfOffBits,sizeof(unsigned long),1,fp_BMP);

	fwrite((char*)&bmih,sizeof(alvBITMAPINFOHEADER),1,fp_BMP);
	fwrite((char*)rgbQuad,sizeof(alvRGBQUAD),256,fp_BMP);

	pImgtmp = BMP_Img+width*(height-1); 
	for(i=height-1; i>=0; i--)
	{
		fwrite(pImgtmp,sizeof(BYTE),width,fp_BMP);
		pImgtmp -= width;
	}
	
	fclose(fp_BMP);
	return TRUE;
}

void BMPWrapper::GetDefaultBitInfo(BITMAPINFO** bitmapinfo)
{
	BITMAPINFOHEADER *bmih;	
	int i;
	*bitmapinfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO)+(256-1)*sizeof(RGBQUAD));
	bmih=&(*bitmapinfo)->bmiHeader;
	for(i=0; i<256; i++){
		(*bitmapinfo)->bmiColors[i].rgbBlue=i;
		(*bitmapinfo)->bmiColors[i].rgbGreen=i;
		(*bitmapinfo)->bmiColors[i].rgbRed=i;
		(*bitmapinfo)->bmiColors[i].rgbReserved=0;
	}
	bmih->biSize = (unsigned long)(40);
	bmih->biWidth  = (long)640;
	bmih->biHeight = (long)480;
	bmih->biPlanes = (unsigned short)1;
	bmih->biBitCount = (unsigned short)8;
	bmih->biCompression = (unsigned long)0;
	bmih->biSizeImage = (unsigned long)(640*480);
	bmih->biXPelsPerMeter = (long)3780;
	bmih->biYPelsPerMeter = (long)3780;
	bmih->biClrUsed = (unsigned long)0;
	bmih->biClrImportant = (unsigned long)0;

}