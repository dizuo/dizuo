
#ifndef _DUOLUOXIANZHI_MAP_H_
#define _DUOLUOXIANZHI_MAP_H_

#include "tree/AATree.h"

#ifdef _MSC_VER
#	pragma pack ( push , _DUOLUO_PACKING_ )
#	pragma warning ( push , 3 )
#endif /* _MSC_VER */

_DUOLUO_DLO_BEGIN_

template <class T , class K>
class Map_Trait   {
public :
	typedef	T	value ;
	typedef K	key ;

public :
	Map_Trait () {}
	Map_Trait ( const T& value , const K& key )
	{ v = value ; k = key ; }

	const value& getValue () const
	{ return v ; }

	const key&   getKey () const
	{ return k ; }

	value& getValue ()
	{ return v ; }

	key&   getKey ()
	{ return k ; }

private :
	T	v ;
	K	k ;
} ;

template <class Type , 
          class Key ,
		  class Alloc = FixPool< AAnode< Map_Trait<Type , Key> > > >
class Map   {
public :
	Map ()
	{}

	~Map ()
	{}

	Type& operator [] ( const Key& k )
	{ return ( aatree.findNode ( k ) -> getValue() ) ; }

	const Type& operator [] ( const Key& k ) const
	{ return ( *aatree.findNode ( k ) -> getValue () ) ; }

	class iterator   {
    public :
        friend class Map<Type , Key , Alloc> ;

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

        Map_Trait<Type , Key>* operator -> () const
        { return &( lp -> node_trait ) ; }

        Map_Trait<Type , Key>& operator * () const
        { return lp -> node_trait ; }

    private :
        AAnode< Map_Trait<Type , Key> >* lp ;
    } ;

	const bool empty () const
	{ return aatree.empty () ; }

	void clear ()
	{ aatree.clear () ; }

	void insert ( const Type& t , const Key& k )
	{ aatree.insert ( Map_Trait<Type , Key> ( t , k ) ) ; }

	void insert ( const Map_Trait<Type , Key>& p )
	{ aatree.insert ( p ) ; }

	void earse ( const Key& k )
	{ aatree.earse ( k ) ; }

	void earse ( const iterator& iter )
	{ aatree.earse ( iter.lp ) ; }

	iterator find ( const Key& k )
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
	AATree< Map_Trait<Type , Key> , Alloc> aatree ;
} ;

_DUOLUO_DLO_END_

#ifdef _MSC_VER
#	pragma warning ( pop )
#	pragma pack ( pop )
#endif /* _MSC_VER */

#endif /* _DUOLUOXIANZHI_MAP_H_ */
