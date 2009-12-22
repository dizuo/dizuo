#include <iostream>
#include <string>
#include <map>
using namespace std;

//---------------------------------------------------------------------------
template <typename ReturnT, typename ParamT>
class _EventHandlerBase_1
{
public:
	virtual ReturnT notify(ParamT param) = 0;
};

template <typename ListenerT, typename ReturnT, typename ParamT>
class _EventHandler_1 : public _EventHandlerBase_1<ReturnT, ParamT>
{
	typedef ReturnT (ListenerT::*PtrMember)(ParamT);
	ListenerT* m_object;
	PtrMember m_member;
public:
	_EventHandler_1(ListenerT* object, PtrMember member)
		: m_object(object), m_member(member)
	{}
	ReturnT notify(ParamT param)
	{    return (m_object->*m_member)(param);    }
};
//---------------------------------------------------------------------------
typedef int CppEventHandler;

template <typename ReturnT,typename ParamT>
class CppEvent1
{
	typedef std::map<int,_EventHandlerBase_1<ReturnT,ParamT> *> HandlersMap;
	HandlersMap m_handlers;
	int m_count;
public:
	CppEvent1() : m_count(0)
	{}
	~CppEvent1()
	{
		for( HandlersMap::iterator it = m_handlers.begin(); 
			 it != m_handlers.end(); 
			 it++ )
		{
			delete it->second;
		}
	}
	template<typename ListenerT>
	CppEventHandler attach( ListenerT* object, ReturnT (ListenerT::*member)(ParamT) )
	{
		typedef ReturnT (ListenerT::*PtrMember)(ParamT);
		m_handlers[m_count] = new _EventHandler_1<ListenerT, ReturnT,ParamT>(object,member);
		return m_count++;
	}
	bool detach(CppEventHandler id)
	{
		HandlersMap::iterator it = m_handlers.find(id);
		if(it == m_handlers.end())
			return false;
		delete it->second;
		m_handlers.erase(it);
		return true;
	}
	ReturnT notify(ParamT param)
	{
		for( HandlersMap::iterator it = m_handlers.begin();
			 it != m_handlers.end(); 
			 it++ )
		{
			it->second->notify(param);
		}
	}
};
//---------------------------------------------------------------------------
class CHandler
{
public:
	void DoEvent4_1( int p )
	{
		cout << "DoEvent4_1 " << p << endl;
	}
	void DoEvent4_2( int p )
	{
		cout << "DoEvent4_2 " << p << endl;
	}
};
int main(int argc, char* argv[])
{
	CHandler h;
	CppEvent1<void, int> ceInt;
	CppEventHandler eh1 = ceInt.attach( &h, &CHandler::DoEvent4_1 );
	CppEventHandler eh2 = ceInt.attach( &h, &CHandler::DoEvent4_2 );
	ceInt.notify(200);
	ceInt.detach( eh1 );
	ceInt.notify(201);

	system("PAUSE");
	return 0;
}
/*
DoEvent4_1 200
DoEvent4_2 200
DoEvent4_2 201
请按任意键继续. . .
*/
//---------------------------------------------------------------------------

