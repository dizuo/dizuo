#ifndef _SMARTPTR_H_
#define _SMARTPTR_H_

#include <cassert>
#include "global.h"

#ifndef NULL
#define NULL	0
#endif

//  [3/20/2009 RenYaFei]
//		��delete��ΪSAFE_DELETE

// A template class that provides smart pointer functionality.
/*
   The principle is that each instance of this class holds a pointer to a structure
   that contains a reference counter of the object and the pointer to the object itself.
   The reference is incremented each time the smart pointer is copied and decremented 
   each time the smart pointer is destroyed.
   When no one reference the pointer anymore, the pointed object is destroyed.
*/

template <class T>
class SmartPtr
{
public:
	//! Default constructor. Initializes the pointer to NULL
	SmartPtr()
	{
		m_pPtrInfo = NULL;
	}
	// Contructor with a pointer to track. pPointee is the pointer to keep 
	// track of. Note that the smart pointer will keep ownership of this 
	// pointer and will delete it itself, so don't delete it  yourself.
	SmartPtr(T* pPointee)
	{
		m_pPtrInfo = new TSharedData;
		m_pPtrInfo->m_pPointee = pPointee;
		m_pPtrInfo->m_iRefCount = 1;
	}
	// Copy constructor. The argument is a smart pointer to make a copy of. 
	// The smart pointer will increment the reference counter of the pointed object.
	SmartPtr(const SmartPtr& other)
	{
		m_pPtrInfo = other.m_pPtrInfo;
		if (m_pPtrInfo)
			m_pPtrInfo->m_iRefCount++;
	}
	// Assignment operator. The argument is a smart pointer to make a copy of. 
	// The smart pointer will increment the reference counter of the pointed 
	// object. If the smart pointer was already tracking a variable, the reference 
	// counter for this variable will be decremented (and the pointer destroyed 
	// if it becomes 0).
	SmartPtr& operator=(const SmartPtr& other)
	{
		if (this != &other)
		{
			if (m_pPtrInfo)
			{
				m_pPtrInfo->m_iRefCount--;
				if (m_pPtrInfo->m_iRefCount == NULL)
				{
					SAFE_DELETE( m_pPtrInfo->m_pPointee );
					SAFE_DELETE( m_pPtrInfo );
				}
			}
			m_pPtrInfo = other.m_pPtrInfo;
			if (m_pPtrInfo)
				m_pPtrInfo->m_iRefCount++;
		}
		return *this;
	}

	// Destructor. It decrements the shared reference counter. 
	// If it becomes 0, the pointed variable is destroyed.
	~SmartPtr()  
	{ 
		if (m_pPtrInfo)
		{
			m_pPtrInfo->m_iRefCount--;
			if (m_pPtrInfo->m_iRefCount == 0)
			{
				SAFE_DELETE( m_pPtrInfo->m_pPointee );
				SAFE_DELETE( m_pPtrInfo );
			}
		}
	}


	// Overloading of the * operator to access the contents of the pointed variable.
	T& operator* () const  
	{ 
		assert(m_pPtrInfo != NULL);
		return *(m_pPtrInfo->m_pPointee); 
	}
	// Overloading of the -> operator that returns the pointer to the variable.
	T* operator->() const  
	{ 
		assert(m_pPtrInfo != NULL);
		return m_pPtrInfo->m_pPointee; 
	}

	// Check to see if the pointer to the variable is NULL.
	bool isNull() const  
	{ 
		if (m_pPtrInfo && m_pPtrInfo->m_pPointee)
			return false;
		return true;
	}

private:
	// Structure shared across smart pointers.
	struct TSharedData
	{
		T*	m_pPointee;
		int m_iRefCount;
	};
	TSharedData* m_pPtrInfo;
};

//  [5/6/2009 RYF]
template<class T>
inline bool operator!=(const SmartPtr<T>& left, const SmartPtr<T>& right)
{
	return !(*right == *left);
}

template<class T>
inline bool operator==(const SmartPtr<T>& left, const SmartPtr<T>& right)
{
	return (*left == *right);
}

#endif  // _SMARTPTR_H_