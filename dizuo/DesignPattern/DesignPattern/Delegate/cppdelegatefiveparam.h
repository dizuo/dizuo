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

#ifndef		_CPPDELEGATEFIVEPARAM_H_
#define		_CPPDELEGATEFIVEPARAM_H_


#include	<algorithm>
#include	"cppdelegateinterface.h"

using std::find;

/*
*	通用事件委托类(5参数)	
*/

template <typename P1,typename P2,typename P3,typename P4,typename P5>
struct DeleageFunctionFiveParamBase : public DelegateFunction
{
	virtual void operator()(P1 p1,P2 p2,P3 p3,P4 p4,P5 p5)=0;
};

template <typename P1,typename P2,typename P3,typename P4,typename P5>
struct DeleageFunctionFiveParam : public DeleageFunctionFiveParamBase<P1,P2,P3,P4,P5>
{
	void (*m_fptr)(P1,P2,P3,P4,P5);
	DeleageFunctionFiveParam(void (*fptr)(P1,P2,P3,P4,P5))
		: m_fptr(fptr)
	{}

	bool operator==(const DelegateFunction &oth)
	{
		return memcmp(this,&oth,sizeof(*this))==0;
	}
	virtual void operator()(P1 p1,P2 p2,P3 p3,P4 p4,P5 p5)
	{
		m_fptr(p1,p2,p3,p4,p5);
	}
	virtual DelegateFunction * clone()
	{
		return new DeleageFunctionFiveParam<P1,P2,P3,P4,P5>(m_fptr);
	}
};

template <class ParamT,typename P1,typename P2,typename P3,typename P4,typename P5>
struct ClassDeleageFunctionFiveParam : public DeleageFunctionFiveParamBase<P1,P2,P3,P4,P5>
{
	ParamT * m_pThis;
	void (ParamT::*m_fptr)(P1,P2,P3,P4,P5);
	ClassDeleageFunctionFiveParam(ParamT * t,void (ParamT::*fptr)(P1,P2,P3,P4,P5))
	{
		m_pThis = t;
		m_fptr = fptr;
	}
	bool operator==(const DelegateFunction &oth)
	{
		return memcmp(this,&oth,sizeof(*this))==0;
	}
	virtual void operator()(P1 p1,P2 p2,P3 p3,P4 p4,P5 p5)
	{
		(m_pThis->*m_fptr)(p1,p2,p3,p4,p5);
	}
	virtual DelegateFunction * clone()
	{
		return new ClassDeleageFunctionFiveParam
			<ParamT,P1,P2,P3,P4,P5>(m_pThis,m_fptr);
	}
};

template <typename P1,typename P2,typename P3,typename P4,typename P5>
class CppDelegateFiveParam : public CppDelegate
{
	typedef		DeleageFunctionFiveParamBase<P1,P2,P3,P4,P5>	FunctionType;
public:
	template <class ParamT>
	void addDelegate(ParamT * t,void (ParamT::*fptr)(P1,P2,P3,P4,P5))
	{
		addDelegateStorage(new ClassDeleageFunctionFiveParam
			<ParamT,P1,P2,P3,P4,P5>(t,fptr));
	}

	void addDelegate(void (*fptr)(P1,P2,P3,P4,P5))
	{
		addDelegateStorageT(new DeleageFunctionFiveParam
			<P1,P2,P3,P4,P5>(fptr));
	}

	template <class ParamT>
	void removeDelegate(ParamT * t,void (ParamT::*fptr)(P1,P2,P3,P4,P5))
	{
		removeDelegateStorage(&ClassDeleageFunctionFiveParam
			<ParamT,P1,P2,P3,P4,P5>(t,fptr));
	}
	void removeDelegate(void (*fptr)(P1,P2,P3,P4,P5))
	{
		removeDelegateStorage(&DeleageFunctionFiveParam
			<P1,P2,P3,P4,p5>(fptr));
	}

	void operator()(P1 p1,P2 p2,P3 p3,P4 p4,P5 p5)
	{
		list<StorageT>::iterator b = m_Delegates.begin();
		list<StorageT>::iterator e = m_Delegates.end();
		for(;b != e; ++b){
			(*(FunctionType*)(*b))(p1,p2,p3,p4,p5);
		}
	}
};

#endif 