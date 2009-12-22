
/* class : List
 * This is not a thread-safe class.
 * It's equal to the std :: list in the <list> header file.
 *
 * Editer: luoya.
 * Date  : Fri , 19 , 2009
 *********************************************************************************/

#ifndef _DUOLUOXIANZHI_LIST_H_
#define _DUOLUOXIANZHI_LIST_H_

#include "fixpool.h"

_DUOLUO_DLO_BEGIN_

template <class Ty>
class ListEntry   {
public :
    ListEntry () :
        next ( 0 ) ,
        prev ( 0 )
    {}

    Ty data ;
    
    ListEntry<Ty>* next ;
    ListEntry<Ty>* prev ;
} ;

template <class T ,
		  class allocator = FixPool< ListEntry<T> > >
class List   {
public :
    class iterator   {
    public :
        friend class List<T> ;

        iterator ()
        { list = 0 ; }

        void operator ++ ()
        { list = list -> next ; }

        bool operator == ( iterator& rhs )
        { return list == rhs.list ; }

        bool operator != ( iterator& rhs )
        { return list != rhs.list ; }

        void operator -- ()
        { list = list -> prev ; }

        void operator + ( int add_index )
        {
            for ( int i = 0 ; i < add_index ; ++i )   {
                list = list -> next ;
            }
        }

        void operator - ( int add_index )
        {
            for ( int i = 0 ; i < add_index ; ++i )   {
                list = list -> prev ;
            }
        }

        ListEntry<T>*& operator -> ()
        { return list ; }

        T& operator * ()
        { return list -> data ; }

    private :
        ListEntry<T>* list ;
    } ;

public :
    List ()
    { init () ; }
        
    virtual ~List ()
    { destroy () ; }

    void clear ()
    {
        ListEntry<T>* tmpNode = head -> next ;

        if ( head -> next != tail )   {
            while ( tmpNode -> next )   {
                ListEntry<T>* next_ptr = tmpNode -> next ;
                alloc.free_obj ( tmpNode ) ;
                tmpNode = next_ptr ;
            }

            init () ;
        }
    }

    void erase ( iterator it )
    {
        it -> prev -> next = it -> next ;
        it -> next -> prev = it -> prev ;

        //delete it -> data ;
        allocator.free ( it -> data ) ;
        --lise_size ;
    }

    void erase ( iterator begin , iterator end )
    {
        ListEntry<T>* lp = begin.list ;

        while ( lp != tail -> data && lp != end.list )   {
            ListEntry<T>* nextlp = lp -> next ;

            lp -> prev -> next = lp -> next ;
            lp -> next -> prev = lp -> prev ;
            allocator.free ( lp ) ;

            lp = nextlp ;
            --list_size ;
        }
    }

    void assign ( iterator& begin , iterator& end )
    {
        clear () ;
        iterator it = begin ;

        while ( it != end )   {
            push_back ( it -> data ) ;
            ++lise_size ;
        }
    }

    const unsigned int size () const
    { return list_size ; }

    void insert ( iterator& it , T& data )
    {
        ListEntry<T>* tmpNode = alloc.alloc_obj () ;
        tmpNode -> data = data ;

        tmpNode -> next = it -> next ;
        tmpNode -> prev = it.list ;
        it -> next = tmpNode ;
        tmpNode -> next -> prev = tmpNode ;

        ++list_size ;
    }

    void push_back ( const T& data )
    {
        ListEntry<T>* tmpNode = alloc.alloc_obj () ;
        tmpNode -> data = data ;

        tmpNode -> next = tail ;
        tmpNode -> prev = tail -> prev ;
        tail -> prev = tmpNode  ;
        tmpNode -> prev -> next = tmpNode ;

        ++list_size ;
    }

    void push_front ( const T& data )
    {
        ListEntry<T>* tmpNode = alloc.alloc_obj () ;
        tmpNode -> data = data ;

        tmpNode -> prev = head ;
        tmpNode -> next = head -> next ;
        head -> next = tmpNode  ;
        tmpNode -> next -> prev = tmpNode ;

        ++lise_size ;
    }

    void pop_back ()
    {
        ListEntry<T>* tmpNode = tail -> prev ;
        tmpNode -> prev -> next = tail ;
        tail -> prev = tmpNode -> prev ;

        //delete tmpNode ;
        alloc.free_obj ( tmpNode ) ;
        
        --list_size ;
    }

    void pop_front ()
    {
        ListEntry<T>* tmpNode = head -> next ;
        tmpNode -> next -> prev = head ;
        head -> next = tmpNode -> next ;

        //delete tmpNode ;
        alloc.free_obj ( tmpNode ) ;
        --list_size ;
    }

    T back ()
    { return *( tail -> prev -> get() ) ; }

    T front ()
    { return *( head -> next -> get() ) ; }

    bool empty ()
    { return size == 0 ; }

    iterator begin ()
    { 
        iterator it = iter_begin ;
        ++it ;
        return it ; 
    }

    iterator end ()
    { return iter_end ; }

private :

    ListEntry<T>* head ;
    ListEntry<T>* tail ;

    iterator iter_begin ;
    iterator iter_end ;

    allocator alloc ;
    
    unsigned int list_size ;

    void destroy ()
    {
        clear () ;

        alloc.free_obj ( ( ListEntry<T>* ) head ) ;
        alloc.free_obj ( ( ListEntry<T>* ) tail ) ;
    }

    void init ()
    {
		head = alloc.alloc_obj () ;
		tail = alloc.alloc_obj () ;
        head -> next = tail ;
        tail -> prev = head ;
        list_size = 0 ;

        iter_begin.list = head ;
        iter_end.list = tail ;
    }
} ;

_DUOLUO_DLO_END_

#endif /* _DUOLUOXIANZHI_LIST_H_ */

