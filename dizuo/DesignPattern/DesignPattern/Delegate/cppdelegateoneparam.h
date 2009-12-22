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

#ifndef		_CPPDELEGATEONEPARAM_H_
#define		_CPPDELEGATEONEPARAM_H_


#include	<algorithm>
#include	"cppdelegateinterface.h"
using std::find;

/*
*	通用事件委托类(1参数)	
*/

template <typename P1>
struct DeleageFunctionOneParamBase : public DelegateFunction
{
	virtual void operator()(P1 p1)=0;
};

template <typename P1>
struct DeleageFunctionOneParam : public DeleageFunctionOneParamBase<P1>
{
	void (*m_fptr)(P1);
	DeleageFunctionOneParam(void (*fptr)(P1))
		: m_fptr(fptr)
	{}

	bool operator==(const DelegateFunction &oth)
	{
		return memcmp(this,&oth,sizeof(*this))==0;
	}
	virtual void operator()(P1 p1)
	{
		m_fptr(p1);
	}
	virtual DelegateFunction * clone()
	{
		return new DeleageFunctionOneParam<P1>(m_fptr);
	}
};

template <class ParamT,typename P1>
struct ClassDeleageFunctionOneParam : public DeleageFunctionOneParamBase<P1>
{
	ParamT * m_pThis;
	void (ParamT::*m_fptr)(P1);
	ClassDeleageFunctionOneParam(ParamT * t,void (ParamT::*fptr)(P1))
	{
		m_pThis = t;
		m_fptr = fptr;
	}
	bool operator==(const DelegateFunction &oth)
	{
		return memcmp(this,&oth,sizeof(*this))==0;
	}
	virtual void operator()(P1 p1)
	{
		(m_pThis->*m_fptr)(p1);
	}
	virtual DelegateFunction * clone()
	{
		return new ClassDeleageFunctionOneParam<ParamT,P1>(m_pThis,m_fptr);
	}
};

template <typename P1>
class CppDelegateOneParam : public CppDelegate
{
	typedef		DeleageFunctionOneParamBase<P1>		FunctionType;
public:
	template <class ParamT>
	void addDelegate(ParamT * t,void (ParamT::*fptr)(P1))
	{
		addDelegateStorage(new ClassDeleageFunctionOneParam<ParamT,P1>(t,fptr));
	}

	void addDelegate(void (*fptr)(P1))
	{
		addDelegateStorage(new DeleageFunctionOneParam<P1>(fptr));
	}

	template <class ParamT>
	void removeDelegate(ParamT * t,void (ParamT::*fptr)(P1))
	{
		removeDelegateStorage(&ClassDeleageFunctionOneParam<ParamT,P1>(t,fptr));
	}
	void removeDelegate(void (*fptr)(P1))
	{
		removeDelegateStorage(&DeleageFunctionOneParam<P1>(fptr));
	}
	
	void operator()(P1 p1)
	{
		list<StorageT>::iterator b = m_Delegates.begin();
		list<StorageT>::iterator e = m_Delegates.end();
		for(;b != e; ++b){
			(*(FunctionType*)(*b))(p1);
		}
	}
};

#endif 