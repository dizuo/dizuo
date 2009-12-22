
/* class name: FrameBuffer
 * This is not a thread-safe class.
 *
 * class descripition: Construct a allocator provided a tool that is like
 *                     a stack-like memory pool. Allocate operation and 
 *                     free operation will follow the behavior as stack.
 *
 * editor: luoya
 * date: Sep , 14 , 2009
 *************************************************************************/

#ifndef _DUOLUOXIANZHI_FRAMEBUFFER_H_
#define _DUOLUOXIANZHI_FRAMEBUFFER_H_

#ifndef _WIN32
#error Only for windows operating system.
#else
#include <windows.h>
#endif /* _WIN32 */

_DUOLUO_DLO_BEGIN_

struct FrameUnit   {
	FrameUnit ()
	{ size = 0 , ptr = NULL ; }

	size_t size ;
	void*  ptr ;
} ;

class FrameBuffer   {
public :
	FrameBuffer ()
	{ init () ; }

	~FrameBuffer ()
	{ destroy () ; }

	FrameUnit alloc ( size_t memsize )
	{
		FrameUnit u ;

		if ( thisPtr < tailPtr )   {
			u.size = _DUOLUO_ALIGN_UP_ ( memsize , _DUOLUO_ALIGN_NUM_ ) ;
			u.ptr  = thisPtr ;
			thisPtr += u.size ;
		}

		return u ;
	}

	void  free ( FrameUnit u )
	{
		thisPtr -= u.size ;
	}

	bool  initialize ( size_t totalSize )
	{
		destroy () ;
		size_t tSize = _DUOLUO_ALIGN_UP_ ( totalSize , _DUOLUO_ALIGN_NUM_ ) ;
		heap = HeapCreate ( HEAP_NO_SERIALIZE | HEAP_GENERATE_EXCEPTIONS ,
			                tSize ,
							0 ) ;
		if ( heap )   {
			buf = ( char* ) HeapAlloc ( heap ,
							            HEAP_NO_SERIALIZE | HEAP_GENERATE_EXCEPTIONS | HEAP_ZERO_MEMORY ,
							            tSize ) ;
			if ( buf )   {
				thisPtr = headPtr = buf ;
				tailPtr = ( headPtr + HeapSize ( heap , HEAP_NO_SERIALIZE , buf ) ) ;
				return true ;
			}

			return false ;
		}

		return false ;
	}

private :
	FrameBuffer ( const FrameBuffer& rhs ) ;
	FrameBuffer& operator = ( const FrameBuffer& rhs ) ;

	char*  thisPtr ;
	char*  headPtr ;
	char*  tailPtr ;

	char*  buf ;
	HANDLE heap ;

	void init ()
	{
		thisPtr = headPtr = tailPtr = NULL ;
		buf     = NULL ;
		heap    = NULL ;
	}

	void destroy ()
	{
		if ( heap )   {
			HeapDestroy ( heap ) ;
			heap   = NULL ;
		}
	}
} ;


_DUOLUO_DLO_END_

#endif /* _DUOLUOXIANZHI_FRAMEBUFFER_H_ */
