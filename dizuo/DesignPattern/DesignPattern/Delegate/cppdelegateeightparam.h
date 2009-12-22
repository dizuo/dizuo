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

#ifndef		_CPPDELEGATEEIGHTPARAM_H_
#define		_CPPDELEGATEEIGHTPARAM_H_

#include	<algorithm>
#include	"cppdelegateinterface.h"
using std::find;

/*
*	通用事件委托类(8参数)	
*/

template <typename P1,typename P2,typename P3,typename P4,typename P5,typename P6,typename P7,typename P8>
struct DeleageFunctionEightParamBase : public DelegateFunction
{
	virtual void operator()(P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6,P7 p7,P8 p8)=0;
};

template <typename P1,typename P2,typename P3,typename P4,typename P5,typename P6,typename P7,typename P8>
struct DeleageFunctionEightParam : public DeleageFunctionEightParamBase<P1,P2,P3,P4,P5,P6,P7,P8>
{
	void (*m_fptr)(P1,P2,P3,P4,P5,P6,P7,P8);
	DeleageFunctionEightParam(void (*fptr)(P1,P2,P3,P4,P5,P6,P7,P8))
		: m_fptr(fptr)
	{}

	bool operator==(const DelegateFunction &oth)
	{
		return memcmp(this,&oth,sizeof(*this))==0;
	}
	virtual void operator()(P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6,P7 p7,P8 p8)
	{
		m_fptr(p1,p2,p3,p4,p5,p6,p7,p8);
	}
	virtual DelegateFunction * clone()
	{
		return new DeleageFunctionEightParam
			<P1,P2,P3,P4,P5,P6,P7,P8>(m_fptr);
	}
};

template <class ParamT,typename P1,typename P2,typename P3,typename P4,typename P5,typename P6,typename P7,typename P8>
struct ClassDeleageFunctionEightParam : public DeleageFunctionEightParamBase<P1,P2,P3,P4,P5,P6,P7,P8>
{
	ParamT * m_pThis;
	void (ParamT::*m_fptr)(P1,P2,P3,P4,P5,P6,P7,P8);
	ClassDeleageFunctionEightParam(ParamT * t,void (ParamT::*fptr)(P1,P2,P3,P4,P5,P6,P7,P8))
	{
		m_pThis = t;
		m_fptr = fptr;
	}
	bool operator==(const DelegateFunction &oth)
	{
		return memcmp(this,&oth,sizeof(*this))==0;
	}
	virtual void operator()(P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6,P7 p7,P8 p8)
	{
		(m_pThis->*m_fptr)(p1,p2,p3,p4,p5,p6,p7,p8);
	}
	virtual DelegateFunction * clone()
	{
		return new ClassDeleageFunctionEightParam
			<ParamT,P1,P2,P3,P4,P5,P6,P7,P8>(m_pThis,m_fptr);
	}
};

template <typename P1,typename P2,typename P3,typename P4,typename P5,typename P6,typename P7,typename P8>
class CppDelegateEightParam : public CppDelegate
{
	typedef		DeleageFunctionEightParamBase<P1,P2,P3,P4,P5,P6,P7,P8>	FunctionType;
public:
	template <class ParamT>
	void addDelegate(ParamT * t,void (ParamT::*fptr)(P1,P2,P3,P4,P5,P6,P7,P8))
	{
		addDelegateStorage(new ClassDeleageFunctionEightParam
			<ParamT,P1,P2,P3,P4,P5,P6,P7,P8>(t,fptr));
	}

	void addDelegate(void (*fptr)(P1,P2,P3,P4,P5,P6,P7,P8))
	{
		addDelegateStorage(new DeleageFunctionEightParam
			<P1,P2,P3,P4,P5,P6,P7,P8>(fptr));
	}

	template <class ParamT>
	void removeDelegate(ParamT * t,void (ParamT::*fptr)(P1,P2,P3,P4,P5,P6,P7,P8))
	{
		removeDelegateStorage(&ClassDeleageFunctionEightParam
			<ParamT,P1,P2,P3,P4,P5,P6,P7,P8>(t,fptr));
	}

	void removeDelegate(void (*fptr)(P1,P2,P3,P4,P5,P6,P7,P8))
	{
		removeDelegateStorage(&DeleageFunctionEightParam
			<P1,P2,P3,P4,p5,P6,P7,P8>(fptr));
	}


	void operator()(P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6,P7 p7,P8 p8)
	{
		list<StorageT>::iterator b = m_Delegates.begin();
		list<StorageT>::iterator e = m_Delegates.end();
		for(;b != e; ++b){
			(*(FunctionType*)(*b))(p1,p2,p3,p4,p5,p6,p7,p8);
		}
	}
};

#endif 