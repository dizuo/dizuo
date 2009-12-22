
/* class : queue
 * This is not a thread-safe class.
 * It's equal to the std :: queue in the <queue> header file.
 *
 * Editer: luoya.
 * Date  : Tue , 23 , 2009
 *********************************************************************************/

#ifndef _DUOLUOXIANZHI_QUEUE_H_
#define _DUOLUOXIANZHI_QUEUE_H_

#include "list.h"

_DUOLUO_DLO_BEGIN_
    
template <class T>
class Queue   {
public :
    Queue ()
    {}

    ~Queue ()
    {}

    void clear ()
    { list.clear () ; }

    const bool empty () const
    { return list.empty () ; }

    void push ( const T& v )
    { list.push_back ( v ) ; }

    void pop ()
    { list.pop_back () ; }

    T top ()
    { return list.back () ; }

    const unsigned int size () const
    { return list.size () ; }

protected :
    ;

private :
    dlo :: List<T> list ;

} ;

_DUOLUO_DLO_END_

#endif /* _DUOLUOXIANZHI_QUEUE_H_ */
