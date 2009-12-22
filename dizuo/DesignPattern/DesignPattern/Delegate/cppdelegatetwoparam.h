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

#ifndef		_CPPDELEGATETWOPARAM_H_
#define		_CPPDELEGATETWOPARAM_H_

#include	<algorithm>
#include	"cppdelegateinterface.h"
using std::find;

/*
*	通用事件委托类(2参数)	
*/

template <typename P1,typename P2>
struct DeleageFunctionTwoParamBase : public DelegateFunction
{
	virtual void operator()(P1 p1,P2 p2)=0;
};

template <typename P1,typename P2>
struct DeleageFunctionTwoParam : public DeleageFunctionTwoParamBase<P1,P2>
{
	void (*m_fptr)(P1,P2);
	DeleageFunctionTwoParam(void (*fptr)(P1,P2))
		: m_fptr(fptr)
	{}

	bool operator==(const DelegateFunction &oth)
	{
		return memcmp(this,&oth,sizeof(*this))==0;
	}
	virtual void operator()(P1 p1,P2 p2)
	{
		m_fptr(p1,p2);
	}
	virtual DelegateFunction * clone()
	{
		return new DeleageFunctionTwoParam<P1,P2>(m_fptr);
	}
};

template <class ParamT,typename P1,typename P2>
struct ClassDeleageFunctionTwoParam : public DeleageFunctionTwoParamBase<P1,P2>
{
	ParamT * m_pThis;
	void (ParamT::*m_fptr)(P1,P2);
	ClassDeleageFunctionTwoParam(ParamT * t,void (ParamT::*fptr)(P1,P2))
	{
		m_pThis = t;
		m_fptr = fptr;
	}
	bool operator==(const DelegateFunction &oth)
	{
		return memcmp(this,&oth,sizeof(*this))==0;
	}
	virtual void operator()(P1 p1,P2 p2)
	{
		(m_pThis->*m_fptr)(p1,p2);
	}
	virtual DelegateFunction * clone()
	{
		return new ClassDeleageFunctionTwoParam<ParamT,P1,P2>(m_pThis,m_fptr);
	}
};

template <typename P1,typename P2>
class CppDelegateTwoParam : public CppDelegate
{
	typedef			DeleageFunctionTwoParamBase<P1,P2>		FunctionType;
public:
	template <class ParamT>
	void addDelegate(ParamT * t,void (ParamT::*fptr)(P1,P2))
	{
		addDelegateStorage(new ClassDeleageFunctionTwoParam
			<ParamT,P1,P2>(t,fptr));
	}

	void addDelegate(void (*fptr)(P1,P2))
	{
		addDelegateStorage(new DeleageFunctionTwoParam<P1,P2>(fptr));
	}

	template <class ParamT>
	void removeDelegate(ParamT * t,void (ParamT::*fptr)(P1,P2))
	{
		removeDelegateStorage(&ClassDeleageFunctionTwoParam<ParamT,P1,P2>(t,fptr));
	}
	void removeDelegate(void (*fptr)(P1,P2))
	{
		removeDelegateStorage(&DeleageFunctionTwoParam<P1,P2>(fptr));
	}
	
	void operator()(P1 p1,P2 p2)
	{
		list<StorageT>::iterator b = m_Delegates.begin();
		list<StorageT>::iterator e = m_Delegates.end();
		for(;b != e; ++b){
			(*(FunctionType*)(*b))(p1,p2);
		}
	}
};

#endif 