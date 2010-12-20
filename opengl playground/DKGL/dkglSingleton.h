#ifndef _DKGL_SINGLETON_H_
#define _DKGL_SINGLETON_H_

#include <assert.h>

namespace DKGL
{

template <class T> class Singleton
{
    static T* ms_Singleton;

public:
    Singleton( void )
    {
        assert( !ms_Singleton );
#if defined( _MSC_VER ) && _MSC_VER < 1200	 
            int offset = (int)(T*)1 - (int)(Singleton <T>*)(T*)1;
            ms_Singleton = (T*)((int)this + offset);
#else
	    ms_Singleton = static_cast< T* >( this );
#endif
    }

   ~Singleton( void )
        {  assert( ms_Singleton );  ms_Singleton = 0;  }
    static T& GetSingleton( void )
        {  assert( ms_Singleton );  return ( *ms_Singleton );  }
    static T* GetSingletonPtr( void )
        {  return ( ms_Singleton );  }
};

template <class T>  T* Singleton<T>::ms_Singleton = 0;

}
#endif