
#ifndef _DUOLUOXIANZHI_SET_H_
#define _DUOLUOXIANZHI_SET_H_

#include "tree/AATree.h"

_DUOLUO_DLO_BEGIN_

template <class T>
class Set_Trait   {
public :
	typedef	T	    value ;
	typedef value	key ;

public :
	Set_Trait () {}
	Set_Trait ( const T& value )
	{ v = value ; }

	const value& getValue () const
	{ return v ; }

	const key&   getKey () const
	{ return v ; }

	value& getValue ()
	{ return v ; }

	key&   getKey ()
	{ return v ; }

private :
	T	v ;
} ;

template <class Type ,
		  class Alloc = FixPool< AAnode< Set_Trait<Type> > > >
class Set   {
public :
	Set ()
	{}

	~Set ()
	{}

	class iterator   {
    public :
        friend class Set<Type , Alloc> ;

        iterator ()
        { lp = 0 ; }

        void operator ++ ()
        { lp = lp -> next ; }

        const bool operator == ( const iterator& rhs ) const
        { return lp == rhs.lp ; }

        const bool operator != ( const iterator& rhs ) const
        { return lp != rhs.lp ; }

        void operator -- ()
        { lp = lp -> prev ; }

        iterator operator + ( int add_index )
        {
			iterator tmpIt ( *this ) ;
            for ( int i = 0 ; i < add_index ; ++i , ++tmpIt.lp ) ;

			return tmpIt ;
        }

        iterator operator - ( int add_index )
        {
            iterator tmpIt ( *this ) ;
            for ( int i = 0 ; i < add_index ; ++i , --tmpIt.lp ) ;

			return tmpIt ;
        }

        Set_Trait<Type>* operator -> () const
        { return &( lp -> node_trait ) ; }

        Set_Trait<Type>& operator * () const
        { return lp -> node_trait ; }

    private :
        AAnode< Set_Trait<Type> >* lp ;
    } ;

	const bool empty () const
	{ return aatree.empty () ; }

	void clear ()
	{ aatree.clear () ; }

	void insert ( const Type& t )
	{ aatree.insert ( Set_Trait<Type> ( t ) ) ; }

	void insert ( const Set_Trait<Type>& p )
	{ aatree.insert ( p ) ; }

	void earse ( const Type& k )
	{ aatree.earse ( k ) ; }

	void earse ( const iterator& iter )
	{ aatree.earse ( iter.lp ) ; }

	iterator find ( const Type& k )
	{ 
		iterator it ;
		it.lp = aatree.findNode ( k ) ;
		if ( it.lp )
			return it ;

		return end () ;
	}

	iterator begin () const
	{
		iterator it ;
		it.lp = aatree.getThreadHead () -> next ;
		return it ;
	}

	iterator end () const
	{ 
		iterator it ;
		it.lp = aatree.getThreadTail () ;
		return it ;
	}

private :
	AATree< Set_Trait<Type> , Alloc> aatree ;
} ;

_DUOLUO_DLO_END_

#endif /* _DUOLUOXIANZHI_SET_H_ */
