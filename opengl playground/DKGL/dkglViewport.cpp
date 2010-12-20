#include "dkglViewport.h"

namespace DKGL
{

//Rect2D::Rect2D(int _x, int _y, int  _width, int _height) : x(_x), y(_y) , width(_width), height(_height){}

Viewport::Viewport()
{
	//sizerFunc = NULL;
	mBeActive = false;
	mBeExist = true;
	mBeSelected = false;
	mBoarderSize = 4;
	mBoarderColor[0] = mBoarderColor[1] = mBoarderColor[2] = 0.5f;
	mBeSelectedColor[0] = mBeSelectedColor[1] = mBeSelectedColor[2] = 1.0f;
}

Viewport::Viewport(int _x, int _y, int _w, int _h)
{
	//sizerFunc = NULL;
	x = _x; y = _y; width = _w; height = _h;
	mBeActive = false;
	mBeExist = true;
	mBeSelected = false;
	mBoarderSize = 4;
	mBoarderColor[0] = mBoarderColor[1] = mBoarderColor[2] = 0.5f;
	mBeSelectedColor[0] = mBeSelectedColor[1] = mBeSelectedColor[2] = 1.0f;
	mAspectRatio = static_cast<double>(width) / height;
}

void Viewport::onSize(const intRect2D& rect, ViewportSizerAdaptor& sizer)
{
	intRect2D rs;
	rs = sizer(rect);

	x = rs.x; y = rs.y; width = rs.width, height = rs.height;
	mAspectRatio = static_cast<double>(width)/ height;
}

void Viewport::drawBoarder() const
{
	glViewport(x, y, width, height);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();		//projection_push
    glLoadIdentity();
	glOrtho((GLdouble)0, (GLdouble)width,
		(GLdouble)0, (GLdouble)height, -1.0, 10000.0);
    glMatrixMode(GL_MODELVIEW);
	glPushMatrix();		//modelView_push
	glLoadIdentity();

	glPushAttrib(GL_LIGHTING);
	glDisable(GL_LIGHTING);

	if(mBeSelected)
		glColor3f(mBeSelectedColor[0],mBeSelectedColor[1], mBeSelectedColor[2]);
	else
		glColor3f(mBoarderColor[0],mBoarderColor[1], mBoarderColor[2]);

	glBegin(GL_QUADS);
	glVertex2i(0, 0);
	glVertex2i(mBoarderSize, 0);
	glVertex2i(mBoarderSize, height);
	glVertex2i(0, height);

	glVertex2i(0, 0);
	glVertex2i(width -1, 0);
	glVertex2i(width -1, mBoarderSize);
	glVertex2i(0, mBoarderSize);

	glVertex2i(width-mBoarderSize, 0);
	glVertex2i(width, 0);
	glVertex2i(width, height);
	glVertex2i(width-mBoarderSize, height);

	glVertex2i(0, height-mBoarderSize);
	glVertex2i(width, height-mBoarderSize);
	glVertex2i(width, height);
	glVertex2i(0, height);

	glEnd();

	glPopMatrix();		//modelView_pop
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();		//projection_pop
	glPopAttrib();
}

//////////////////////////////////////////////////////////////////////////

ViewportManager::ViewportManager(int vpn)
	: mViewportBuf(0)
{
	mViewportBuf = new Viewport[vpn];
	mNumOfViewport = vpn;
	mMaxViewportId = -1;
}
	
ViewportManager::~ViewportManager()
{
	delete [] mViewportBuf;
	mViewportBuf = 0;
}

void ViewportManager::createViewport(int n)
{
	int i;
	if (mViewportBuf)
	{
		delete [] mViewportBuf;
		mViewportBuf = 0;
	}

	mViewportBuf = new Viewport[n];
	mNumOfViewport = n;

	for(i=0;i<mNumOfViewport;i++)
		mViewportBuf[i].mBeActive = true;
}

void ViewportManager::activeViewport(int idx)
{
	mViewportBuf[idx].mBeActive = true;
}

void ViewportManager::resizeViewport(int x, int y, int w, int h, int idx)
{
	mViewportBuf[idx].x = x;
	mViewportBuf[idx].y = y;
	mViewportBuf[idx].width = w;
	mViewportBuf[idx].height = h;
	mViewportBuf[idx].mAspectRatio = (double)w/(double)h;
}

void ViewportManager::updateClientRectSize(int w, int h)
{
	mClientRectWidth = w;
	mClientRectHeight = h;

	if(mMaxViewportId>=0)
	{
		mViewportBuf[mMaxViewportId].height = h;
		mViewportBuf[mMaxViewportId].width = w;
		mViewportBuf[mMaxViewportId].mAspectRatio = static_cast<double>(w)/ h;
	}
}

int ViewportManager::left(int idx) const
{
	return mViewportBuf[idx].x;
}

int ViewportManager::bottom(int idx) const
{
	return mViewportBuf[idx].y;
}

int ViewportManager::width(int idx) const
{
	return mViewportBuf[idx].width;
}

int ViewportManager::height(int idx) const 
{
	return mViewportBuf[idx].height;
}

void ViewportManager::maximize(int idx)
{
	int i;
	for(i=0;i<mNumOfViewport;i++)
		mViewportBuf[i].mBeActive = false;

	mViewportBuf[idx].mBeActive = true;
	mViewportBuf[idx].height = mClientRectHeight;
	mViewportBuf[idx].width = mClientRectWidth;
	mViewportBuf[idx].x = 0;
	mViewportBuf[idx].y = 0;
	mViewportBuf[idx].mAspectRatio = (double)mClientRectWidth/(double)mClientRectHeight;
	mMaxViewportId = idx;
}

bool ViewportManager::isSelected(int vp)
{
	return mViewportBuf[vp].mBeSelected;
}

int ViewportManager::selectViewport(int win_x, int win_y)
{
	int v, i;
	v = whichViewport(win_x, win_y);
	for(i=0;i<mNumOfViewport;i++)
	{
		if(i==v)
			mViewportBuf[i].mBeSelected = true;
		else	//make sure that only one viewport is actived in every time
			mViewportBuf[i].mBeSelected = false;
	}
	return v;
}

int ViewportManager::whichViewport(int winx, int winy)
{
	int i;

	// remap the cursor coordinate
	winy = mClientRectHeight - winy;
	// 跟viewport设置参数一致

	for(i=0;i<mNumOfViewport;i++)
	{
		if((winx>mViewportBuf[i].x) && (winx < (mViewportBuf[i].x + mViewportBuf[i].width)) 
			&& ( winy > mViewportBuf[i].y ) && ( winy < (mViewportBuf[i].y + mViewportBuf[i].height))
			&& mViewportBuf[i].mBeActive)
			return i;
	}

	return -1;
}

Viewport ViewportManager::getViewport(int idx)
{
	return mViewportBuf[idx];
}

Viewport* ViewportManager::getViewportPtr(int idx)
{
	return &mViewportBuf[idx];
}

}// namespace DKGL