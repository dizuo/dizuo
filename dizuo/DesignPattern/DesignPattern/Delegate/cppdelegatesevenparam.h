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

#ifndef		_CPPDELEGATESEVENPARAM_H_
#define		_CPPDELEGATESEVENPARAM_H_


#include	<algorithm>
#include	"cppdelegateinterface.h"
using std::find;

/*
*	通用事件委托类(7参数)	
*/

template <typename P1,typename P2,typename P3,typename P4,typename P5,typename P6,typename P7>
struct DeleageFunctionSevenParamBase : public DelegateFunction
{
	virtual void operator()(P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6,P7 p7)=0;
};

template <typename P1,typename P2,typename P3,typename P4,typename P5,typename P6,typename P7>
struct DeleageFunctionSevenParam : public DeleageFunctionSevenParamBase<P1,P2,P3,P4,P5,P6,P7>
{
	void (*m_fptr)(P1,P2,P3,P4,P5,P6,P7);
	DeleageFunctionSevenParam(void (*fptr)(P1,P2,P3,P4,P5,P6,P7))
		: m_fptr(fptr)
	{}

	bool operator==(const DelegateFunction &oth)
	{
		return memcmp(this,&oth,sizeof(*this))==0;
	}
	virtual void operator()(P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6,P7 p7)
	{
		m_fptr(p1,p2,p3,p4,p5,p6,p7);
	}
	virtual DelegateFunction * clone()
	{
		return new DeleageFunctionSevenParam
			<P1,P2,P3,P4,P5,P6,P7>(m_fptr);
	}
};

template <class ParamT,typename P1,typename P2,typename P3,typename P4,typename P5,typename P6,typename P7>
struct ClassDeleageFunctionSevenParam : public DeleageFunctionSevenParamBase<P1,P2,P3,P4,P5,P6,P7>
{
	ParamT * m_pThis;
	void (ParamT::*m_fptr)(P1,P2,P3,P4,P5,P6,P7);
	ClassDeleageFunctionSevenParam(ParamT * t,void (ParamT::*fptr)(P1,P2,P3,P4,P5,P6,P7))
	{
		m_pThis = t;
		m_fptr = fptr;
	}
	bool operator==(const DelegateFunction &oth)
	{
		return memcmp(this,&oth,sizeof(*this))==0;
	}
	virtual void operator()(P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6,P7 p7)
	{
		(m_pThis->*m_fptr)(p1,p2,p3,p4,p5,p6,p7);
	}
	virtual DelegateFunction * clone()
	{
		return new ClassDeleageFunctionSevenParam
			<ParamT,P1,P2,P3,P4,P5,P6,P7>(m_pThis,m_fptr);
	}
};

template <typename P1,typename P2,typename P3,typename P4,typename P5,typename P6,typename P7>
class CppDelegateSevenParam : public CppDelegate
{
	typedef		DeleageFunctionSevenParamBase<P1,P2,P3,P4,P5,P6,P7>		FunctionType;
public:
	template <class ParamT>
	void addDelegate(ParamT * t,void (ParamT::*fptr)(P1,P2,P3,P4,P5,P6,P7))
	{
		addDelegateStorage(new ClassDeleageFunctionSevenParam
			<ParamT,P1,P2,P3,P4,P5,P6,P7>(t,fptr));
	}

	void addDelegate(void (*fptr)(P1,P2,P3,P4,P5,P6,P7))
	{
		addDelegateStorage(new DeleageFunctionSevenParam
			<P1,P2,P3,P4,P5,P6,P7>(fptr));
	}

	template <class ParamT>
	void removeDelegate(ParamT * t,void (ParamT::*fptr)(P1,P2,P3,P4,P5,P6,P7))
	{
		removeDelegateStorage(&ClassDeleageFunctionSevenParam<ParamT,P1,P2,P3,P4,P5,P6,P7>(t,fptr));
	}
	void removeDelegate(void (*fptr)(P1,P2,P3,P4,P5,P6,P7))
	{
		removeDelegateStorage(&DeleageFunctionSevenParam<P1,P2,P3,P4,p5,P6,P7>(fptr));
	}
	
	void operator()(P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6,P7 p7)
	{
		list<StorageT>::iterator b = m_Delegates.begin();
		list<StorageT>::iterator e = m_Delegates.end();
		for(;b != e; ++b){
			(*(FunctionType*)(*b))(p1,p2,p3,p4,p5,p6,p7);
		}
	}
};

#endif 