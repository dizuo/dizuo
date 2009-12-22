
#ifndef _DUOLUOXIANZHI_CONTAINERS_H_
#define _DUOLUOXIANZHI_CONTAINERS_H_

#define _DUOLUO_ALIGN_NUM_			( 4 )
#define _DUOLUO_ALIGN_UP_(a, b)		( ( ( a ) - 1 ) / ( b ) + 1 ) * ( b )

_DUOLUO_DLO_BEGIN_

template <class T , class K>
class Pair   {
public :
	Pair ()
	{}

	Pair ( const T& f , const K& s )
	{ first = f , second = s ; }

	T first ;
	K second ;
} ;

_DUOLUO_DLO_END_

#include "heap.h"

#include "framebuffer.h"

#include "vector.h"

#include "stack.h"

#include "fixpool.h"

#include "list.h"

#include "queue.h"

#include "hashtable.h"

#include "tree/tree.h"

#include "map.h"

#include "set.h"

#endif /* _DUOLUOXIANZHI_CONTAINERS_H_ */
