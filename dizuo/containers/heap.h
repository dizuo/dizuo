

/* HeapAlloc.h
 * contains functions that implement the new and delete.
 *
 * Editer: luoya.
 * Date  : Tue , 23 , 2009
 *********************************************************************************/

#ifndef _DUOLUOXIANZHI_HEAP_H_
#define _DUOLUOXIANZHI_HEAP_H_

#include <stdlib.h>

_DUOLUO_DLO_BEGIN_

inline
size_t align_up ( size_t address , size_t base )
{ return ( ( --address ) / ( base ) + 1 ) * ( base ) ; }

inline
void* heap_alloc ( size_t numElements , size_t sizeElement )
{ return calloc ( 1 , align_up ( numElements * sizeElement , _DUOLUO_ALIGN_NUM_ ) ) ; }

inline
void  heap_free ( void* p )
{ free ( p ) ; }

template <typename T>
inline
T*    heap_new ()
{ return ::new T ; }

template <typename T>
inline
T*    heap_new ( const int numElement )
{ return ::new T[numElement] ; }


template <typename T>
inline
void  heap_delete ( T* p )
{ ::delete p ; }

template <typename T>
inline
void  heap_delete_arr ( T* p )
{ ::delete[] p ; }

/* class name:Heap
 * class description: A class with simple heap allocator.
 * 
 * editor: luoya
 * date: August,28,2009
 ************************************************************************/

template<class T>
class Heap   {
public :
	Heap ()
	{}

	T* alloc_obj ()
	{ return heap_new<T> () ; }

	T* alloc_arr ( const int numElement )
	{ return heap_new<T> ( numElement ) ; }

	void  free_obj ( T* p )
	{ heap_delete ( p ) ; }

	void  free_arr ( T* p )
	{ heap_delete_arr ( p ) ; }

protected :
	;

private :
	Heap ( const Heap& rhs ) ;
	Heap& operator = ( const Heap& rhs ) ;
} ;

_DUOLUO_DLO_END_

#endif /* _DUOLUOXIANZHI_HEAPALLOC_H_ */
