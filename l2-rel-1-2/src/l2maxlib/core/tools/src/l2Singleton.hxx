#ifndef L2SINGLETON_HXX_
#define L2SINGLETON_HXX_

#include "l2SingletonImpl.h"
#include "l2ISingleton.h"

#include <l2MutexLocker.h>

#include <typeinfo>

namespace l2max
{
   template< class T >
      class CSingleton : protected CSingletonImpl,
                         protected ISingleton
      {
         CSingleton( const CSingleton& );
         const CSingleton& operator = ( const CSingleton& );

      protected:
      	CSingleton(){}
      	virtual ~CSingleton(){}

      public:
         static T* GetInstance()
         {
            CMutexLocker locker( CSingletonImpl::_mutex );
            
            T* ret( 0 );
            
            if( !( ret = static_cast< T* >( CSingletonImpl::singleton( typeid( T ).name() ) ) ) )
            {
               ret = new T();
               
               CSingletonImpl::Register( *ret );
            }
            
            return ret;
         }
      };
}

#endif /*L2SINGLETON_H_*/
