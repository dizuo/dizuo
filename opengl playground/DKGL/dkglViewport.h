#ifndef _DKGL_VIEWPORT_H_
#define _DKGL_VIEWPORT_H_

#include "dkglPreRequest.h"
#include "dkglSingleton.h"
#include "dkglRect2D.h"

namespace DKGL
{

class ViewportSizerAdaptor
{
public:
	virtual intRect2D operator () (const intRect2D&)= 0;
};

class DefaultVpSizer : public ViewportSizerAdaptor
{
public:
	inline intRect2D operator () (const intRect2D& rs) 
	{
		intRect2D _rs = rs;
		return _rs;
	};
};

class Viewport
{
public:
	Viewport();
	Viewport(int _x, int _y, int _w, int _h);
	~Viewport(){};

	//inline void assignSizer(RectSize (*sizerFunc)(int x, int y, int width, int height))
	//{
	//	this->sizerFunc = sizerFunc;
	//};

	void reset(int _x, int _y, int _w, int _h)
	{ x = _x; y = _y; width = _w; height = _h; }

	void drawBoarder() const;

	void onSize(const intRect2D&, ViewportSizerAdaptor& sizer);

	//inline void setSize (int _x, int _y, int _w, int _h)
	//{
	//	x = _x; y = _y; width = _w, height = _h;
	//	mAspectRatio = (double)width/ height;
	//}

	inline Viewport& operator = ( const Viewport& vp )
    {
        x = vp.x;
        y = vp.y;
		width = vp.width;
		height = vp.height;
		mBeActive = vp.mBeActive;
		mBeExist = vp.mBeExist;
        return *this;
    }
	
	int x, y, width, height;
	bool mBeActive;
	bool mBeExist;
	bool mBeSelected;
	double mAspectRatio;
	float mBoarderColor[3];
	float mBeSelectedColor[3];
	int mBoarderSize;

private:
	//Rect2D (*sizerFunc)(int x, int y, int width, int height);
	
};

class ViewportManager : public Singleton<ViewportManager>
{
public:
	// The construct function is called when createViewport is used
	ViewportManager(int);
	//ViewportManeger();
	~ViewportManager();

	void createViewport(int);//const std::string&);
	void activeViewport(int);//const std::string&);
	void resizeViewport(int x, int y, int w, int h, int idx=0);
	
	int left(int idx=0) const;
	int bottom(int idx=0) const;
	int width(int idx=0) const;
	int height(int idx=0) const;

	void updateClientRectSize(int w, int h);
	Viewport getViewport(int idx = 0);
	Viewport* getViewportPtr(int idx = 0);
	int whichViewport(int win_x, int win_y);
	int selectViewport(int win_x, int win_y);
	bool isSelected(int);
	void maximize(int idx);

	int getNumOfViewport() { return mNumOfViewport; };

	int mMaxViewportId;
private:
	ViewportManager()
		: mViewportBuf(0)
	{};
	Viewport* mViewportBuf;

	//std::map<std::string, Viewport> mViewportBuf;
	int mNumOfViewport;
	int mClientRectWidth;
	int mClientRectHeight;
	
};

}

#endif