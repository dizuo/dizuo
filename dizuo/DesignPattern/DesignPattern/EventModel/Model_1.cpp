#include <iostream>
#include <string>

using namespace std;

//---------------------------------------------------------------------------

//无参数的事件的处理者的基类
class _CEventVoid_Handler_    {
public:
	virtual void Do() = 0;
};
//无参数的事件
class CEventVoid    {
public:
	void Fire()
	{    if( _ph != NULL )    _ph->Do();    }
	void Bind( _CEventVoid_Handler_ * ph )
	{    _ph = ph;    }
	CEventVoid()
	{    _ph = NULL;    }
private:
	_CEventVoid_Handler_ * _ph;
};

//---------------------------------------------------------------------------

//int型参数的事件的处理者的基类
class _CEventInt_Handler_    {
public:
	virtual void Do(int p1) = 0;
};
//int型参数的事件
class CEventInt    {
public:
	void Fire(int p1)
	{    if( _ph != NULL )    _ph->Do(p1);    }
	void Bind( _CEventInt_Handler_ * ph )
	{    _ph = ph;    }
	CEventInt()
	{    _ph = NULL;    }
private:
	_CEventInt_Handler_ * _ph;
};

//---------------------------------------------------------------------------

//int,string型参数的事件的处理者的基类
class _CEventIntString_Handler_    {
public:
	virtual void Do(int p1, const string& p2) = 0;
};
//int,string型参数的事件
class CEventIntString    {
public:
	void Fire(int p1, const string& p2)
	{    if( _ph != NULL )    _ph->Do(p1, p2);    }
	void Bind( _CEventIntString_Handler_ * ph )
	{    _ph = ph;    }
	CEventIntString()
	{    _ph = NULL;    }
private:
	_CEventIntString_Handler_ * _ph;
};

//---------------------------------------------------------------------------

//实际使用的例子：事件的触发者
class CSender
{
public:
	//触发事件
	void SendEvent()
	{
		evoid.Fire();
		eint.Fire(123);
		eintstring.Fire(123,"abc");
	}

public:
	//定义事件
	CEventVoid    evoid;
	CEventInt    eint;
	CEventIntString eintstring;
};

//实际使用的例子：事件的处理者
class CHandler
{
public:
	CHandler()
		: hvoid( *this ), hint( *this ), hintstring( *this )
	{
	}

	//事件处理函数
	void DoEvent1()
	{
		cout << "DoEvent1" << endl;
	}
	void DoEvent2(int p)
	{
		cout << "DoEvent2 " << p << endl;
	}
	void DoEvent3(int p1, const string& p2)
	{
		cout << "DoEvent3 " << p1 << " " << p2 << endl;
	}

	//定义事件处理函数的关联器
	class _CHandler_DoEvent1_ : public _CEventVoid_Handler_    {
	public:
		_CHandler_DoEvent1_( CHandler & h ) : _h(h){}
		CHandler & _h;
		virtual void Do()
		{ _h.DoEvent1();    }
	} hvoid;

	class _CHandler_DoEvent2_ : public _CEventInt_Handler_    {
	public:
		_CHandler_DoEvent2_( CHandler & h ) : _h(h){}
		CHandler & _h;
		virtual void Do(int p1)
		{    _h.DoEvent2(p1);    }
	} hint;

	class _CHandler_DoEvent3_ : public _CEventIntString_Handler_    {
	public:
		_CHandler_DoEvent3_( CHandler & h ) : _h(h){}
		CHandler & _h;
		virtual void Do(int p1, const string& p2)
		{    _h.DoEvent3(p1, p2);    }
	} hintstring;
};

int main(int argc, char* argv[])
{
	CHandler h;
	CSender s;
	//绑定事件
	s.evoid.Bind( &h.hvoid );
	s.eint.Bind( &h.hint );
	s.eintstring.Bind( &h.hintstring );
	s.SendEvent();

	return 0;
}
/*
DoEvent1
DoEvent2 123
DoEvent3 123 abc
请按任意键继续. . .
*/
