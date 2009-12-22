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

#ifndef		_CPPDELEGATESIXPARAM_H_
#define		_CPPDELEGATESIXPARAM_H_

#include	<algorithm>
#include	"cppdelegateinterface.h"
using std::find;

/*
*	通用事件委托类(6参数)	
*/

template <typename P1,typename P2,typename P3,typename P4,typename P5,typename P6>
struct DeleageFunctionSixParamBase : public DelegateFunction
{
	virtual void operator()(P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6)=0;
};

template <typename P1,typename P2,typename P3,typename P4,typename P5,typename P6>
struct DeleageFunctionSixParam : public DeleageFunctionSixParamBase<P1,P2,P3,P4,P5,P6>
{
	void (*m_fptr)(P1,P2,P3,P4,P5,P6);
	DeleageFunctionSixParam(void (*fptr)(P1,P2,P3,P4,P5,P6))
		: m_fptr(fptr)
	{}

	bool operator==(const DelegateFunction &oth)
	{
		return memcmp(this,&oth,sizeof(*this))==0;
	}
	virtual void operator()(P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6)
	{
		m_fptr(p1,p2,p3,p4,p5,p6);
	}
	virtual DelegateFunction * clone()
	{
		return new DeleageFunctionSixParam
			<P1,P2,P3,P4,P5,P6>(m_fptr);
	}
};

template <class ParamT,typename P1,typename P2,typename P3,typename P4,typename P5,typename P6>
struct ClassDeleageFunctionSixParam : public DeleageFunctionSixParamBase<P1,P2,P3,P4,P5,P6>
{
	ParamT * m_pThis;
	void (ParamT::*m_fptr)(P1,P2,P3,P4,P5,P6);
	ClassDeleageFunctionSixParam(ParamT * t,void (ParamT::*fptr)(P1,P2,P3,P4,P5,P6))
	{
		m_pThis = t;
		m_fptr = fptr;
	}
	bool operator==(const DelegateFunction &oth)
	{
		return memcmp(this,&oth,sizeof(*this))==0;
	}
	virtual void operator()(P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6)
	{
		(m_pThis->*m_fptr)(p1,p2,p3,p4,p5,p6);
	}
	virtual DelegateFunction * clone()
	{
		return new ClassDeleageFunctionSixParam
			<ParamT,P1,P2,P3,P4,P5,P6>(m_pThis,m_fptr);
	}
};

template <typename P1,typename P2,typename P3,typename P4,typename P5,typename P6>
class CppDelegateSixParam : public CppDelegate
{
	typedef			DeleageFunctionSixParamBase<P1,P2,P3,P4,P5,P6>		FunctionType;
public:
	template <class ParamT>
	void addDelegate(ParamT * t,void (ParamT::*fptr)(P1,P2,P3,P4,P5,P6))
	{
		addDelegateStorage(new ClassDeleageFunctionSixParam
			<ParamT,P1,P2,P3,P4,P5,P6>(t,fptr));
	}

	void addDelegate(void (*fptr)(P1,P2,P3,P4,P5,P6))
	{
		addDelegateStorage(new DeleageFunctionSixParam
			<P1,P2,P3,P4,P5,P6>(fptr));
	}

	template <class ParamT>
	void removeDelegate(ParamT * t,void (ParamT::*fptr)(P1,P2,P3,P4,P5,P6))
	{
		removeDelegateStorage(&ClassDeleageFunctionSixParam<ParamT,P1,P2,P3,P4,P5,P6>(t,fptr));
	}
	void removeDelegate(void (*fptr)(P1,P2,P3,P4,P5,P6))
	{
		removeDelegateStorage(&DeleageFunctionSixParam<P1,P2,P3,P4,p5,P6>(fptr));
	}

	void operator()(P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6)
	{
		list<StorageT>::iterator b = m_Delegates.begin();
		list<StorageT>::iterator e = m_Delegates.end();
		for(;b != e; ++b){
			(*(FunctionType*)(*b))(p1,p2,p3,p4,p5,p6);
		}
	}
};

#endif 