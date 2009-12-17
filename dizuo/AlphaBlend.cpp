/*---------------------------------------
Gradient效果
11/18/2009 RYF
---------------------------------------*/

#include <windows.h>
#include <WindowsX.h>
#include <math.h>
#include "bmp.h"

//////////////////////////////////////////////////////////////////////////

/*
* alpha为混合因子 0.0 <= alpha <= 1.0
* srcColor  destColor可以为Red，Green，Blue分量 
*/
BYTE AlphaBlend(const BYTE srcColor, const BYTE destColor, BYTE alpha = 100)
{
	BYTE retValue= srcColor * alpha/255 + destColor * (0xff-alpha)/255;

	return retValue;
}

struct COLOR
{
	COLOR(BYTE r, BYTE g, BYTE b)
		: red(r)
		, green(g)
		, blue(b)
	{}
	BYTE red;
	BYTE green;
	BYTE blue;
};
class CBMP
{
public:
	CBMP(const char* bmpName)
	{
		LoadFileBMP( bmpName, &byte, &m_iWid, &m_iHgh, true);
		m_iSize = m_iWid*m_iHgh*4;
	}

	~CBMP()
	{
		if (byte)
		{
			delete byte; 
			byte = 0;
		}
	}

	//根据序号获得颜色
	COLOR operator()(int index) const
	{
		if (index > m_iSize || index < 0)
		{
			return COLOR(0xff, 0xff, 0xff);
		}

		BYTE* ptr = &(byte[index]);
		return COLOR(ptr[0], ptr[1], ptr[2]);
	}

	// 根据二维坐标获得颜色
	COLOR operator()(int x, int y) const
	{
		if (x < 0 || x > m_iWid || y < 0 || y > m_iHgh)
		{
			return COLOR(0xff, 0xff, 0xff); 
		}
		int index = (x + y*m_iWid) * 4;
		BYTE* ptr = &(byte[index]);
		return COLOR(ptr[0], ptr[1], ptr[2]);
	}

	void ShowBMP(HDC myDc) const
	{
		HBITMAP hbmp;
		HDC		hBfr;

		hbmp = CreateCompatibleBitmap( myDc, m_iWid, m_iHgh );
		hBfr = CreateCompatibleDC( myDc );
		SelectObject( hBfr,  hbmp );

		for (int i=0; i<m_iSize; i+=4)
		{
			int pos = i / 4;
			int x = pos % m_iWid;
			int y = pos / m_iWid;
			BYTE* ptr = &(byte[i]);

			//BYTE white = 255;
			//BYTE Red	= AlphaBlend(ptr[0], white, alpha);
			//BYTE Green	= AlphaBlend(ptr[1], white, alpha);
			//BYTE Blue	= AlphaBlend(ptr[2], 0, alpha);
			//SetPixelV( hBfr, x, y, RGB( Red, Green, Blue) );
			SetPixelV( hBfr, x, y, RGB( ptr[0], ptr[1], ptr[2]) );
		}

		//显示 也可以缩放
		StretchBlt( myDc, 0, 0, m_iWid, m_iHgh, 
			hBfr, 0, 0, m_iWid, m_iHgh, SRCCOPY );
	}

	int m_iSize;
	int m_iWid;
	int m_iHgh;
	BYTE* byte;
private:
	CBMP();
};

CBMP g_srcBmp("bmp24.bmp");
CBMP g_destBmp("background.bmp");

//////////////////////////////////////////////////////////////////////////
// Util class:
class AlphaBlendBMP
{
public:
	AlphaBlendBMP(BYTE a)
		:alpha(a)
	{}

	//alpha融合图像，显示
	//src的长宽均小于dest位图的长宽
	void operator()(HDC myDc, const CBMP& srcBmp, const CBMP& destBmp)
	{
		int minWid = min(srcBmp.m_iWid, destBmp.m_iWid);
		int minHgh = min(srcBmp.m_iHgh, destBmp.m_iHgh);

		int maxWid = max(srcBmp.m_iWid, destBmp.m_iWid);
		int maxHgh = max(srcBmp.m_iHgh, destBmp.m_iHgh);

		int src = 0;
		int dst = 0;

		HBITMAP hbmp;
		HDC		hBfr;

		hbmp = CreateCompatibleBitmap( myDc, srcBmp.m_iWid, srcBmp.m_iHgh );
		hBfr = CreateCompatibleDC( myDc );
		SelectObject( hBfr,  hbmp );

		// 只是在src的区域上面做融合
		for (; src<srcBmp.m_iSize; src+=4, dst+=4)
		{
			int pos = src/4;
			int x = pos % srcBmp.m_iWid;
			int y = pos / srcBmp.m_iWid;

			COLOR colorSrc = srcBmp(src);
			COLOR colorDest = destBmp(x, y);
			
			BYTE red	= AlphaBlend(colorSrc.red, colorDest.red, alpha);
			BYTE green  = AlphaBlend(colorSrc.green, colorDest.green, alpha);
			BYTE blue	= AlphaBlend(colorSrc.blue, colorDest.blue, alpha);
			
			SetPixelV( hBfr, x, y, RGB( red, green, blue) );
			//SetPixelV( hBfr, x, y, RGB(colorDest.red, colorDest.green, colorDest.blue) );
			//SetPixelV( hBfr, x, y, RGB(srcBmp.byte[src], srcBmp.byte[src+1], srcBmp.byte[src+2]) );
		}

		//显示 也可以缩放
		StretchBlt( myDc, 0, 0, srcBmp.m_iWid, srcBmp.m_iHgh, 
			hBfr, 0, 0, srcBmp.m_iWid, srcBmp.m_iHgh, SRCCOPY );
	}

	void IncreAlpha()
	{
		alpha = (alpha + 10)%256;
	}
private:
	AlphaBlendBMP(){};
	BYTE alpha;
};
//////////////////////////////////////////////////////////////////////////
AlphaBlendBMP alphaBlendTest(100);
//////////////////////////////////////////////////////////////////////////
// MSG Functions
BOOL OnCreate( HWND hwnd, LPCREATESTRUCT lpct )
{
	HINSTANCE hInstance = GetWindowInstance(hwnd);
	//load res
	//...

	return TRUE;
}

VOID OnDestroy( HWND hwnd )
{
	//delete obj;
	PostQuitMessage(0);
}

BOOL OnPaint( HWND hwnd )
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd,&ps);
	{
		// 显示bmp
		//g_srcBmp.ShowBMP(hdc);
		//g_destBmp.ShowBMP(hdc);
		alphaBlendTest(hdc, g_srcBmp, g_destBmp);
	} EndPaint(hwnd,&ps);

	return TRUE;
}

void OnChar(HWND hwnd, TCHAR ch, int cRepeat)
{
	switch(ch)
	{
	case ' ':
		alphaBlendTest.IncreAlpha();
		::InvalidateRect(hwnd, NULL, TRUE);
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK	WndProc (HWND, UINT, WPARAM, LPARAM) ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
					PSTR szCmdLine, int iCmdShow)
{
	static wchar_t szAppName[] = L"Alpha Blend" ;
	HWND        hwnd ;
	MSG         msg ;
	WNDCLASSEX  wndclass ;

	wndclass.cbSize        = sizeof (wndclass) ;
	wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
	wndclass.lpfnWndProc   = WndProc ;
	wndclass.cbClsExtra    = 0 ;
	wndclass.cbWndExtra    = 0 ;
	wndclass.hInstance     = hInstance ;
	wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
	wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
	wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
	wndclass.lpszMenuName  = NULL ;
	wndclass.lpszClassName = szAppName ;
	wndclass.hIconSm       = LoadIcon (NULL, IDI_APPLICATION) ;

	RegisterClassEx (&wndclass) ;

	hwnd = CreateWindow (szAppName, szAppName,
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT, CW_USEDEFAULT,
						CW_USEDEFAULT, CW_USEDEFAULT,
						NULL, NULL, hInstance, NULL) ;

	ShowWindow (hwnd, iCmdShow) ;
	UpdateWindow (hwnd) ;

	while (GetMessage (&msg, NULL, 0, 0))
	{
		TranslateMessage (&msg) ;
		DispatchMessage (&msg) ;
	}
	return msg.wParam ;
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
		HANDLE_MSG(hwnd, WM_CREATE,	OnCreate);
		HANDLE_MSG(hwnd, WM_DESTROY,OnDestroy);
		HANDLE_MSG(hwnd, WM_PAINT,  OnPaint);
		HANDLE_MSG(hwnd, WM_CHAR,	OnChar);
	}

	return DefWindowProc (hwnd, iMsg, wParam, lParam);
}


/*
unsigned int AlphaBlend(const unsigned int bg, const unsigned int src)
{
	unsigned int a = src >> 24;    
	/ * alpha * /    / * If source pixel is transparent, just return the background * /   
	if (0 == a)       
		return bg;    

	/ * alpha blending the source and background colors * /   
	unsigned int rb = (((src & 0x00ff00ff) * a) + ((bg & 0x00ff00ff) * (0xff - a))) & 0xff00ff00;   
	unsigned int g = (((src & 0x0000ff00) * a) +  ((bg & 0x0000ff00) * (0xff - a))) & 0x00ff0000;     

	return (src & 0xff000000) | ((rb | g) >> 8);
}*/
