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

#ifndef		_CPPDELEGATEINTERFACE_H_
#define		_CPPDELEGATEINTERFACE_H_
#include	<list>
using std::list;

struct DelegateFunction
{
	virtual bool operator==(const DelegateFunction &oth) = 0;
	virtual DelegateFunction * clone()=0;
	bool operator!=(const DelegateFunction &oth)
	{
		return !(*this == oth);
	}
	virtual ~DelegateFunction(){}
};

class CppDelegate
{
protected:
	typedef DelegateFunction* StorageT;
	list<StorageT>	m_Delegates;
	
	void addDelegateStorage(StorageT dg)
	{
		m_Delegates .push_back(dg);
	}
	void removeDelegateStorage(StorageT dg)
	{
		list<StorageT>::iterator b = m_Delegates.begin();
		list<StorageT>::iterator e = m_Delegates.end();
		for(;b!=e;++b){
			if(*(*b)==*dg){
				delete (*b);
				m_Delegates.erase(b);
				break;
			}
		}
	}
public:
	void removeAll()
	{
		list<StorageT>::iterator b = m_Delegates.begin();
		list<StorageT>::iterator e = m_Delegates.end();
		for(;b != e; ++b){
			delete (*b);
		}
		m_Delegates.clear();
	}
	virtual ~CppDelegate()
	{
		//printf("size = %d \n",m_Delegates.size());
		removeAll();
	}
	CppDelegate&
		operator=(const CppDelegate& oth)
	{
		removeAll();
		list<StorageT>::const_iterator b = oth.m_Delegates.begin();
		list<StorageT>::const_iterator e = oth.m_Delegates.end();
		for(;b!=e;++b){
			addDelegateStorage((*b)->clone());
		}
		return *this;
	}
};



#endif //_CPPDELEGATEINTERFACE_H_