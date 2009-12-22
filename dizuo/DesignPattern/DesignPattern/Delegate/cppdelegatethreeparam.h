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

#ifndef		_CPPDELEGATETHREEPARAM_H_
#define		_CPPDELEGATETHREEPARAM_H_

#include	<algorithm>
#include	"cppdelegateinterface.h"
using std::find;

/*
*	通用事件委托类(3参数)	
*/

template <typename P1,typename P2,typename P3>
struct DeleageFunctionThreeParamBase : public DelegateFunction
{
	virtual void operator()(P1 p1,P2 p2,P3 p3)=0;
};

template <typename P1,typename P2,typename P3>
struct DeleageFunctionThreeParam : public DeleageFunctionThreeParamBase<P1,P2,P3>
{
	void (*m_fptr)(P1,P2,P3);
	DeleageFunctionThreeParam(void (*fptr)(P1,P2,P3))
		: m_fptr(fptr)
	{}

	bool operator==(const DelegateFunction &oth)
	{
		return memcmp(this,&oth,sizeof(*this))==0;
	}
	virtual void operator()(P1 p1,P2 p2,P3 p3)
	{
		m_fptr(p1,p2,p3);
	}
	virtual DelegateFunction * clone()
	{
		return new DeleageFunctionThreeParam
			<P1,P2,P3>(m_fptr);
	}
};

template <class ParamT,typename P1,typename P2,typename P3>
struct ClassDeleageFunctionThreeParam : public DeleageFunctionThreeParamBase<P1,P2,P3>
{
	ParamT * m_pThis;
	void (ParamT::*m_fptr)(P1,P2,P3);
	ClassDeleageFunctionThreeParam(ParamT * t,void (ParamT::*fptr)(P1,P2,P3))
	{
		m_pThis = t;
		m_fptr = fptr;
	}
	bool operator==(const DelegateFunction &oth)
	{
		return memcmp(this,&oth,sizeof(*this))==0;
	}
	virtual void operator()(P1 p1,P2 p2,P3 p3)
	{
		(m_pThis->*m_fptr)(p1,p2,p3);
	}
	virtual DelegateFunction * clone()
	{
		return new ClassDeleageFunctionThreeParam<ParamT,P1,P2,P3>(m_pThis,m_fptr);
	}
};

template <typename P1,typename P2,typename P3>
class CppDelegateThreeParam : public CppDelegate
{
	typedef		DeleageFunctionThreeParamBase<P1,P2,P3>		FunctionType;
public:
	template <class ParamT>
	void addDelegate(ParamT * t,void (ParamT::*fptr)(P1,P2,P3))
	{
		addDelegateStorage(new ClassDeleageFunctionThreeParam
			<ParamT,P1,P2,P3>(t,fptr));
	}

	void addDelegate(void (*fptr)(P1,P2,P3))
	{
		addDelegateStorage(new DeleageFunctionThreeParam
			<P1,P2,P3>(fptr));
	}

	template <class ParamT>
	void removeDelegate(ParamT * t,void (ParamT::*fptr)(P1,P2,P3))
	{
		removeDelegateStorage(&ClassDeleageFunctionThreeParam
			<ParamT,P1,P2,P3>(t,fptr));
	}
	void removeDelegate(void (*fptr)(P1,P2,P3))
	{
		removeDelegateStorage(&DeleageFunctionThreeParam<P1,P2,P3>(fptr));
	}

	void operator()(P1 p1,P2 p2,P3 p3)
	{
		list<StorageT>::iterator b = m_Delegates.begin();
		list<StorageT>::iterator e = m_Delegates.end();
		for(;b != e; ++b){
			(*(FunctionType*)(*b))(p1,p2,p3);
		}
	}
};

#endif 