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

#ifndef		_CPPDELEGATEFOURPARAM_H_
#define		_CPPDELEGATEFOURPARAM_H_

#include	<algorithm>
#include	"cppdelegateinterface.h"
using std::find;

/*
*	通用事件委托类(4参数)	
*/

template <typename P1,typename P2,typename P3,typename P4>
struct DeleageFunctionFourParamBase : public DelegateFunction
{
	virtual void operator()(P1 p1,P2 p2,P3 p3,P4 p4)=0;
};

template <typename P1,typename P2,typename P3,typename P4>
struct DeleageFunctionFourParam : public DeleageFunctionFourParamBase<P1,P2,P3,P4>
{
	void (*m_fptr)(P1,P2,P3,P4);
	DeleageFunctionFourParam(void (*fptr)(P1,P2,P3,P4))
		: m_fptr(fptr)
	{}
	bool operator==(const DelegateFunction &oth)
	{
		return memcmp(this,&oth,sizeof(*this))==0;
	}
	virtual void operator()(P1 p1,P2 p2,P3 p3,P4 p4)
	{
		m_fptr(p1,p2,p3,p4);
	}
	virtual DelegateFunction * clone()
	{
		return new DeleageFunctionFourParam<P1,P2,P3,P4>(m_fptr);
	}
};

template <class ParamT,typename P1,typename P2,typename P3,typename P4>
struct ClassDeleageFunctionFourParam : public DeleageFunctionFourParamBase<P1,P2,P3,P4>
{
	ParamT * m_pThis;
	void (ParamT::*m_fptr)(P1,P2,P3,P4);
	ClassDeleageFunctionFourParam(ParamT * t,void (ParamT::*fptr)(P1,P2,P3,P4))
	{
		m_pThis = t;
		m_fptr = fptr;
	}
	bool operator==(const DelegateFunction &oth)
	{
		return memcmp(this,&oth,sizeof(*this))==0;
	}
	virtual void operator()(P1 p1,P2 p2,P3 p3,P4 p4)
	{
		(m_pThis->*m_fptr)(p1,p2,p3,p4);
	}
	virtual DelegateFunction * clone()
	{
		return new ClassDeleageFunctionFourParam
			<ParamT,P1,P2,P3,P4>(m_pThis,m_fptr);
	}
};

template <typename P1,typename P2,typename P3,typename P4>
class CppDelegateFourParam : public CppDelegate
{
	typedef		DeleageFunctionFourParamBase<P1,P2,P3,P4>	FunctionType;
public:
	template <class ParamT>
	void addDelegate(ParamT * t,void (ParamT::*fptr)(P1,P2,P3,P4))
	{
		addDelegateStorage(new ClassDeleageFunctionFourParam
			<ParamT,P1,P2,P3,P4>(t,fptr));
	}

	void addDelegate(void (*fptr)(P1,P2,P3,P4))
	{
		addDelegateStorage(new DeleageFunctionFourParam<P1,P2,P3,P4>(fptr));
	}

	template <class ParamT>
	void removeDelegate(ParamT * t,void (ParamT::*fptr)(P1,P2,P3,P4))
	{
		removeDelegateStorage(&ClassDeleageFunctionFourParam<ParamT,P1,P2,P3,P4>(t,fptr));
	}
	void removeDelegate(void (*fptr)(P1,P2,P3,P4))
	{
		removeDelegateStorage(&DeleageFunctionFourParam<P1,P2,P3,P4>(fptr));
	}

	void operator()(P1 p1,P2 p2,P3 p3,P4 p4)
	{
		list<StorageT>::iterator b = m_Delegates.begin();
		list<StorageT>::iterator e = m_Delegates.end();
		for(;b != e; ++b){
			(*(FunctionType*)(*b))(p1,p2,p3,p4);
		}
	}
};

#endif 