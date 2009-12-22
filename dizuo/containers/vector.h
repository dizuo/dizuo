
/* class : vector
 * This is not a thread-safe class.
 * It's equal to the std :: vector in the <vector> header file.
 *
 * Editer: luoya.
 * Date  : Tue , 23 , 2009
 *********************************************************************************/

#ifndef _DUOLUOXIANZHI_VECTOR_H_
#define _DUOLUOXIANZHI_VECTOR_H_

#include <memory.h>
#include "heap.h"

_DUOLUO_DLO_BEGIN_

template <class T , 
          class allocator = Heap<T> >
class Vector   {
public :
    Vector () :
        arr ( 0 ) ,
        vec_size ( 0 ) ,
        exp ( 4 ) ,
        mem_size ( 32 ) ,
		unit_size_inbyte ( sizeof ( T ) )
    {
        init () ;
    }

    Vector ( const Vector& rhs )
    {
        vec_size = rhs.vec_size ;
        exp      = rhs.exp ;

        init () ;

        assign ( rhs.begin () , rhs.empty () ) ;
    }

    Vector& operator = ( const Vector& rhs )
    {
        vec_size = rhs.vec_size ;
        exp      = rhs.exp ;

        init () ;
        assign ( rhs.begin () , rhs.empty () ) ;

        return *this ;
    }

    virtual ~Vector ()
    {
        destroy () ;
    }

    const T& operator [] ( const unsigned int index ) const
    { return arr[index] ; }

    T& operator [] ( const unsigned int index )
    { return arr[index] ; }

    class iterator   {
    public :
        friend class Vector<T , allocator> ;

        iterator ()
        { vec_lp = 0 ; }

        void operator ++ ()
        { ++vec_lp ; }

        const bool operator == ( const iterator& rhs ) const
        { return vec_lp == rhs.vec_lp ; }

        const bool operator != ( const iterator& rhs ) const
        { return vec_lp != rhs.vec_lp ; }

        void operator -- ()
        { --vec_lp ; }

        iterator operator + ( int add_index )
        {
			iterator tmpIt ( *this ) ;
            for ( int i = 0 ; i < add_index ; ++i , ++tmpIt.vec_lp ) ;

			return tmpIt ;
        }

        iterator operator - ( int add_index )
        {
            iterator tmpIt ( *this ) ;
            for ( int i = 0 ; i < add_index ; ++i , --tmpIt.vec_lp ) ;

			return tmpIt ;
        }

        unsigned int operator - ( const iterator& rhs ) const
        { return vec_lp - rhs.vec_lp ; }

        Vector<T , allocator>*& operator -> () const
        { return vec_lp ; }

        T& operator * () const
        { return *vec_lp ; }

    private :
        T* vec_lp ;
    } ;

    void clear ()
    {
        destroy () ;
        vec_size = 0 ;
        mem_size = 32 ;
        init () ;
    }

    void erase ( const iterator& it )
    {
        mem_move ( it.vec_lp , &arr[vec_size - 1] , -1 ) ;
        --vec_size ;
        reset_iter () ;
    }

    void erase ( const iterator& begin , const iterator& end )
    {
        int offset = end - begin ;
        mem_move ( end.vec_lp , &arr[vec_size - 1] , -offset ) ;

        vec_size -= offset ;
        reset_iter () ;
    }

    void assign ( const iterator& begin , const iterator& end )
    {
        destroy () ;

        unsigned int cpy_size = end - begin ;
        while ( mem_size < cpy_size )
            mem_size *= exp ;

        vec_size = cpy_size ;
        mem_resize ( mem_size , begin.vec_lp , cpy_size ) ;
    }

    void push_back ( const T& v )
    {
        mem_inc () ;
        arr[vec_size] = v ;

		++vec_size ;
		reset_iter () ;
    }

    void pop_back ()
    {
        --vec_size ;
        reset_iter () ;
    }

    void push_front ( const T& v )
    {
        mem_inc () ;
		if ( vec_size != 0 )
			mem_move ( arr , &arr[vec_size - 1] , 1 ) ;

        arr[0] = v ;
        ++vec_size ;
        reset_iter () ;
    }

    void pop_front ()
    {
        mem_move ( &arr[1] , &arr[vec_size - 1] , -1 ) ;
        --vec_size ;

        reset_iter () ;
    }

    void insert ( const iterator& it , const T& v )
    {
        mem_inc () ;
        mem_move ( it.vec_lp , &arr[vec_size - 1] , 1 ) ;

        ( *it ) = v ;
        ++vec_size ;

        reset_iter () ;
    }

    T front ()
    { return *iter_begin ; }

    T back ()
    { return *( iter_end - 1 ) ; }

    const bool empty () const
    { return vec_size == 0 ; }

    const unsigned int size () const
    { return vec_size ; }

    void set_exp ( unsigned char new_exp )
    { if ( new_exp > 1 ) exp = new_exp ; }
    const unsigned char get_exp () const
    { return exp ; }

    const T* getVec () const
    { return arr ; }

    const iterator& begin () const
    { return iter_begin ; }

    const iterator& end () const
    { return iter_end ; }

protected :
	allocator     alloc ;
    T*            arr ;
    unsigned int  vec_size ;
    unsigned char exp ;
    unsigned int  mem_size ;
	unsigned int  unit_size_inbyte ;

    iterator      iter_begin ;
    iterator      iter_end ;

    void mem_resize ( const unsigned int new_size , const T* src , const unsigned int cpy_size )
    {
		T* tmpArr = alloc.alloc_arr ( new_size + 1 ) ;
		for ( unsigned int i = 0 ; i < cpy_size ; ++i )
			tmpArr[i] = arr[i] ;

        destroy () ;
        arr = tmpArr ;
        reset_iter () ;
    }

    void mem_move ( T* begin_lp , T* end_lp , const int offset )
    {
		if ( offset < 0 )   {
			T* rp = begin_lp + offset ;
			T* lp = begin_lp ;

			while ( lp != end_lp )   {
				*rp = *lp ;
				++rp ;
				++lp ;
			}

			*rp = *lp ;
		}
		else if ( offset > 0 )  {
			T* rp = end_lp + offset ;
			T* lp = end_lp ;

			while ( lp != begin_lp )   {
				*rp = *lp ;
				--rp ;
				--lp ;
			}

			*rp = *lp ;
		}
    }

private :
    void init ()
    {
		arr = alloc.alloc_arr ( mem_size + 1 ) ;
        reset_iter () ;
    }

    void destroy ()
    {
        if ( arr )   {
			alloc.free_arr ( arr ) ;
            arr = 0 ;
        }
    }

    void reset_iter ()
    {
        iter_begin.vec_lp = arr ;
        iter_end.vec_lp = arr + vec_size ;
    }

    void mem_inc ()
    {
        if ( vec_size == mem_size )   {
            mem_size *= exp ;
            mem_resize ( mem_size , arr , vec_size ) ;
        }
    }
} ;

_DUOLUO_DLO_END_

#endif /* _DUOLUOXIANZHI_VECTOR_H_ */
