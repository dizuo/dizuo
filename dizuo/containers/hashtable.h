

/* class : HashTable
 * This is not a thread-safe class.
 * It constructs a hash table which can easy find a value within.
 *
 * Editer: luoya.
 * Date  : August , 28 , 2009
 *********************************************************************************/

#ifndef _DUOLUOXIANZHI_HASHTABLE_H_
#define _DUOLUOXIANZHI_HASHTABLE_H_

#include <iostream>
#include "list.h"

_DUOLUO_DLO_BEGIN_

static size_t hashtable_size = 0 ;

template <class T ,
          class allocator = Heap< List<T> > >

class HashTable   {
public :
	HashTable ( size_t initSize = 32 ) :
	    table_size ( initSize ) ,
		get_index ( default_index )
	{ init () ; }

	~HashTable ()
	{ destroy () ; }

	void        insert   ( T& value )
	{
		size_t index = get_index ( value ) ;

		if ( !table[index] )   {
			table[index] = alloc.alloc_obj () ;
			table[index] -> push_back ( value ) ;
			return ;
		}

		List<T> :: iterator it = table[index] -> begin() ;
		for ( ; it != table[index] -> end() ; ++it )   {
			if ( value == *it )
				return ;
		}
		
		table[index] -> push_back ( value ) ;
	}

	void        remove   ( T& value )
	{
		size_t index = get_index ( value ) ;

		if ( table[index] )   {
			List<T> :: iterator it = table[index] -> begin() ;
			for ( ; it != table[index] -> end() ; ++it )   {
				if ( value == *it )   {
					table[index] -> erase ( it ) ;
					return ;
				}
			}
		}
	}

	bool        find     ( T& value )
	{
		size_t index = get_index ( value ) ;

		if ( table[index] )   {
			List<T> :: iterator it = table[index] -> begin() ;
			for ( ; it != table[index] -> end() ; ++it )   {
				if ( value == *it )   {
					return true ;
				}
			}
		}

		return false ;
	}

	T*          find     ( T& value , bool ( *cmp ) ( T& lhs , T& rhs ) )
	{
		size_t index = get_index ( value ) ;

		if ( table[index] )   {
			List<T> :: iterator it = table[index] -> begin() ;
			for ( ; it != table[index] -> end() ; ++it )   {
				if ( cmp ( value , *it ) )   {
					return &( *it ) ;
				}
			}
		}

		return 0 ;
	}

	void        cnt_index ( size_t ( *cnt ) ( T& value ) )
	{ get_index = cnt ; }

private :
	HashTable ( const HashTable& rhs ) ;
	HashTable& operator = ( const HashTable& rhs ) ;

	allocator   alloc ;
	size_t		table_size ;
	List<T>**   table ;
	size_t      ( *get_index ) ( T& value ) ;

	void        init ()
	{ 
		hashtable_size = table_size ;
		table = ( dlo :: List<T>** ) heap_alloc ( table_size , sizeof ( List<T>* ) ) ; 
	}

	void        destroy ()
	{
		for ( size_t i = 0 ; i < table_size ; ++i )   {
			if ( table[i] )
				alloc.free_obj ( table[i] ) ;
		}

		heap_free ( table ) ;
		table = 0 ;
	}

	static size_t default_index ( T& value )
	{
		size_t size = sizeof ( T ) ;
		unsigned char* firstByte = ( unsigned char* ) &value ;
		size_t ret = ( hashtable_size + 1 ) * firstByte[0] ;
		for ( size_t i = 1 ; i < size ; ++i )   {
			++firstByte ;
			ret += hashtable_size * firstByte[0] ;
		}

		//std :: cout << ret % hashtable_size << std :: endl ;
		return ( ret % hashtable_size ) ;
	}
} ;

_DUOLUO_DLO_END_

#endif /* _DUOLUOXIANZHI_HASHTABLE_H_ */
