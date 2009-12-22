
/* class : allocator
 * This is not a thread-safe class.
 * It is a memory pool made by duoluoxianzhi
 *
 * Editer: luoya.
 * Date  : Tue , 23 , 2009
 *********************************************************************************/

#ifndef _DUOLUOXIANZHI_FIXPOOL_H_
#define _DUOLUOXIANZHI_FIXPOOL_H_

#include "heap.h"
#include "vector.h"

_DUOLUO_DLO_BEGIN_
    
template <class T>
class FixPool   {
public :
    FixPool () :
        bnum ( 8 ) ,
        exp ( 4 ) ,
        unit_size ( sizeof ( T ) )
    { init () ; }

    ~FixPool ()
    { destroy () ; }

    T* alloc_obj ()
    {
        return apply () ;
    }

    void free_obj ( T* buf )
    {
        alloc_vec.push_back ( buf ) ;
    }

    void change_strategy ( unsigned int block_num = 32 , unsigned char block_exp = 4 )
    { 
        bnum  = block_num ;
        exp   = block_exp ;
    }

protected :
    unsigned int         bnum ;
    unsigned int         unit_size ;
    unsigned int         curr_size ;
    unsigned char        exp ;
    T*                   buf_lp ;

    dlo :: Vector<T*> buf_vec ;
    dlo :: Vector<T*> alloc_vec ;
   
private :
    void init ()
	{
        mem_apply () ;
    }

    void destroy ()
    {
		for ( unsigned int i = 0 ; i < buf_vec.size () ; ++i )   {
            heap_delete_arr<T> ( buf_vec[i] ) ;
		}
    }

    void mem_apply ()
    {
        bnum *= exp ;
        curr_size = bnum ;

        // allocate memory for T* buffer.
        buf_lp = heap_new<T> ( curr_size ) ;
        buf_vec.push_back ( buf_lp ) ;
    }

    T* apply ()
    {
        if ( !alloc_vec.empty () )   {
            T* buf = alloc_vec.back () ;
            alloc_vec.pop_back () ;

			return buf ;
        }

        if ( curr_size == 0 )   {
            mem_apply () ;
        }

        T* buf = buf_lp ;
        ++buf_lp ;
        --curr_size ;

		return buf ;
    }

} ;

_DUOLUO_DLO_END_

#endif /* _DUOLUOXIANZHI_FIXPOOL_H_ */
