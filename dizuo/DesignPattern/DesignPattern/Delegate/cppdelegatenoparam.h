/**
 *
 *	Copyright (C) 2007 JammaSoft. All rights reserved.
 *
 *	http://www.jamma.cn
 *
 *	ManfyLee
 *
 *	Manfred@jamma.cn
 *
 *
 **/

#ifndef		_CPPDELEGATENOPARAM_H_
#define		_CPPDELEGATENOPARAM_H_

#include	<algorithm>
#include	"cppdelegateinterface.h"
using std::find;

/*
*	通用事件委托类(没有参数)	
*/


struct DeleageFunctionNoParamBase : public DelegateFunction
{
	virtual void operator()(void)=0;
};

struct DeleageFunctionNoParam : public DeleageFunctionNoParamBase
{
	void (*m_fptr)();
	DeleageFunctionNoParam(void (*fptr)())
		: m_fptr(fptr)
	{}

	bool operator==(const DelegateFunction &oth)
	{
		return memcmp(this,&oth,sizeof(*this))==0;
	}

	virtual void operator()(void)
	{
		m_fptr();
	}

	virtual DelegateFunction * clone()
	{
		return new DeleageFunctionNoParam(m_fptr);
	}
};

template <class ParamT>
struct ClassDeleageFunctionNoParam : public DeleageFunctionNoParamBase
{
	ParamT * m_pThis;
	void (ParamT::*m_fptr)();
	ClassDeleageFunctionNoParam(ParamT * t,void (ParamT::*fptr)())
	{
		m_pThis = t;
		m_fptr = fptr;
	}
	bool operator==(const DelegateFunction &oth)
	{
		return memcmp(this,&oth,sizeof(*this))==0;
	}
	virtual void operator()(void)
	{
		(m_pThis->*m_fptr)();
	}
	virtual DelegateFunction * clone()
	{
		return new ClassDeleageFunctionNoParam<ParamT>(m_pThis,m_fptr);
	}
};

class CppDelegateNoParam : public CppDelegate
{
	typedef		DeleageFunctionNoParamBase		FunctionType;
public:
	template <class ParamT>
	void addDelegate(ParamT * t,void (ParamT::*fptr)())
	{
		addDelegateStorage(new ClassDeleageFunctionNoParam<ParamT>(t,fptr));
	}

	void addDelegate(void (*fptr)())
	{
		addDelegateStorage(new DeleageFunctionNoParam(fptr));
	}

	template <class ParamT>
	void removeDelegate(ParamT * t,void (ParamT::*fptr)())
	{
		removeDelegateStorage(&ClassDeleageFunctionNoParam<ParamT>(t,fptr));
	}
	void removeDelegate(void (*fptr)())
	{
		removeDelegateStorage(&DeleageFunctionNoParam(fptr));
	}
	void operator()()
	{
		list<StorageT>::iterator b = m_Delegates.begin();
		list<StorageT>::iterator e = m_Delegates.end();
		for(;b != e; ++b){
			(*(FunctionType*)(*b))();
		}
	}
};

#endif 