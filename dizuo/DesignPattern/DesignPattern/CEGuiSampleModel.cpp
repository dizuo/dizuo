#include <iostream>
#include <string>
#include <vector>
#include <ctime>
using namespace std;
#define SAFE_DELETE(p) {if(p){delete p; p = 0;}}

//代表整个CEGUI窗口系统
class System{
private:
	System()
		: text("")
	{}
	~System(){}

	string text;
public:
	static System* getSingleton(){
		static System *singleton = new System;
		return singleton;
	}

	void RenderGUI(){
		cout << text <<endl;
	}

	void setText(string tex){
		text = tex;
	}
};

//前置申明
class Sample;
class BaseApplication
{
public:
	virtual bool excute(Sample* sampleApp) = 0;
};

class OpenGLBaseApplication : public BaseApplication
{
public:
	virtual bool excute(Sample *sampleApp);
};

class DXBaseApplication : public BaseApplication
{
public:
	virtual bool excute(Sample *sampleApp);
};

class OrgeBaseApplication : public BaseApplication
{
public:
	virtual bool excute(Sample *sampleApp);
};

//选择器类
class RenderModeSelector
{
public:
	enum RenderMode{
		OpenGL, DX, Orge
	};
	
	void setMode(RenderMode m){
		mode = m;
	}
	RenderMode getMode(){
		return mode;
	};
private:
	RenderMode mode;
};

//Sample定义
class Sample
{
public:
	Sample()
		: sampleApp(0), selecter(0)
	{
		srand( (unsigned)time( NULL ) );
	}

	~Sample()
	{
		SAFE_DELETE(sampleApp);
		SAFE_DELETE(selecter);
	}
	//给子类改写接口
	virtual bool InitialiseSample() = 0;
	virtual bool cleanSample() = 0;

	virtual bool Initialise(){
		selecter = new RenderModeSelector();
		//selecter->setMode(  rand() );
		//selecter->getMode()
		switch( rand()%3 )
		{
		case RenderModeSelector::OpenGL:
			sampleApp = new OpenGLBaseApplication;
			break;
		case RenderModeSelector::DX:
			sampleApp = new DXBaseApplication;
			break;
		case RenderModeSelector::Orge:
			sampleApp = new OrgeBaseApplication;
			break;
		}
		
		//核心函数: 动态选择
		if( sampleApp->excute(this) )
			return true;

		return false;
	}
	virtual void cleanup(){
		cout << "exit..." <<endl;
	}
	virtual int run(){
		if(Initialise())
			cleanup();

		return 0;
	}

private:
	BaseApplication *sampleApp;
	RenderModeSelector *selecter;
};

//在一个cpp中，所以必须调整类和成员的申明和定义顺序
bool OpenGLBaseApplication::excute(Sample *sampleApp)
{
	//接受Sample继承体系中对窗口System的修改
	sampleApp->InitialiseSample();

	//这里:绘制窗口
	System::getSingleton()->RenderGUI();
	//这里:场景绘制
	cout << "OpenGL render..." << endl;
	return true;
}
bool DXBaseApplication::excute(Sample *sampleApp)
{
	sampleApp->InitialiseSample();

	//这里:绘制窗口
	System::getSingleton()->RenderGUI();
	//这里:场景绘制
	cout << "DX render..." << endl;
	return true;
}
bool OrgeBaseApplication::excute(Sample *sampleApp)
{
	sampleApp->InitialiseSample();

	//这里:绘制窗口
	System::getSingleton()->RenderGUI();
	//这里:场景绘制
	cout << "Orge render..." << endl;
	return true;
}

//自己定义的sample
class MySample : public Sample
{
public:
	//重写initialiseSample函数只是改变system窗口的一些属性...
	//核心窗口显示，渲染不会改变，
	bool InitialiseSample(){
		//改变system属性
		System::getSingleton()->setText("My Sample pV_Vq");
		return true;
	}
	bool cleanSample(){
		//;;;
		return true;
	}
};

int main()
{
	//OOP封装思想
	MySample app;

	return app.run();
}
//						[3/22/2009 RenYaFei]
//////////////////////////////////////////////////////////////////////////
//整体上使用了分层的方法:												//
//BaseApplication继承体系												//
//RenderSelector类														//
//sample继承体系：拥有两个指针，动态地选择渲染类型，					//
//					修改单件类窗口的属性，								//	
//////////////////////////////////////////////////////////////////////////
//要开发的话，不知注重窗口UI的设计，更加注重窗口里面secen的渲染，		//
//		可以参考这个体系，重新搭建一个框架体系.							//
//		类似的方法...													//
//////////////////////////////////////////////////////////////////////////