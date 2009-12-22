
/* class : stack
 * This is not a thread-safe class.
 * It's equal to the std :: stack in the <stack> header file.
 *
 * Editer: luoya.
 * Date  : Tue , 23 , 2009
 *********************************************************************************/

#ifndef _DUOLUOXIANZHI_STACK_H_
#define _DUOLUOXIANZHI_STACK_H_

#include "vector.h"

_DUOLUO_DLO_BEGIN_
    
template <class T>
class Stack   {
public :
    Stack ()
    {}

    virtual ~Stack ()
    {}

    void clear ()
    { list.clear () ; }

    const bool empty () const
    { return list.empty () ; }

    void push ( const T& v )
    { list.push_back ( v ) ; }

    void pop ()
    { list.pop_back () ; }

    T front ()
    { return list.front () ; }

    T back ()
    { return list.back () ; }

    const unsigned int size () const
    { return list.size () ; }

protected :
    ;

private :
    Vector<T> list ;

} ;

_DUOLUO_DLO_END_

#endif /* _DUOLUOXIANZHI_STACK_H_ */
