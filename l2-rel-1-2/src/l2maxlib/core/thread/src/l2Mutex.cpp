#include "l2Mutex.h"

#include <l2ExceptionThrower.h>

#include <iostream>

#ifdef __WIN32__
	#include <windows.h>
#elif __UNIX__
	#include <pthread.h>
#endif

namespace l2max
{
   struct CMutex::SMutexPrivateHandle
   {
   #ifdef __WIN32__
    	HANDLE _mutex;
	#elif __UNIX__
	   pthread_mutex_t _mutex;
	   pthread_mutexattr_t _attribute;
	#endif

   };
   
   CMutex::CMutex( EMutexType aType )
   {
      _mutexPrivateHandle = new SMutexPrivateHandle();
      
      bool isOkay( false );
      
   #ifdef __WIN32__
    	isOkay = ( _mutexPrivateHandle->_mutex = ::CreateMutex( 0, FALSE, 0 ) );
	#elif __UNIX__
	   int theType( PTHREAD_MUTEX_ERRORCHECK );
	   
	   if( aType == mtRecursive )
	   {
	      theType = PTHREAD_MUTEX_RECURSIVE;
	   }
	   
	   isOkay = !pthread_mutexattr_init( &_mutexPrivateHandle->_attribute );
	   
	   if( isOkay )
	   {
	   	isOkay = !pthread_mutexattr_settype( &_mutexPrivateHandle->_attribute, theType );
	   }
	   
	   if( isOkay )
	   {
	      isOkay = !pthread_mutex_init( &_mutexPrivateHandle->_mutex, &_mutexPrivateHandle->_attribute );
	   }

	#endif

      CHECK_OBJECT_STATE( isOkay, "Mutex not created" );
	   
      _mutexType = aType;
      _lockCount = 0;
   }

   CMutex::~CMutex()
   {
   #ifdef __WIN32__
      while( _lockCount )
      {
         UnLock();
      }

      CloseHandle( _mutexPrivateHandle->_mutex );
   #elif __UNIX__
      UnLock();
      
      pthread_mutex_destroy( &_mutexPrivateHandle->_mutex );
      pthread_mutexattr_destroy( &_mutexPrivateHandle->_attribute );
   #endif
   
      delete _mutexPrivateHandle;
   }

   bool CMutex::Lock( unsigned int aTime )
   {
   	bool ret( false );

   #ifdef __WIN32__
      DWORD waitRet( WaitForSingleObject( _mutexPrivateHandle->_mutex, aTime == ltInfinite ? INFINITE : aTime ) );

      ret = ( waitRet == WAIT_OBJECT_0 ||
              waitRet == WAIT_ABANDONED );

      if( ret )
      {
         if( _lockCount && _mutexType == mtNonRecursive )
         {
            ReleaseMutex( _mutexPrivateHandle->_mutex );
         }
         else
         {
            ++_lockCount;
         }
      }
   #elif __UNIX__
      timespec theTimeSpec;
      theTimeSpec.tv_sec = aTime / 1000;
      theTimeSpec.tv_nsec = ( aTime % 1000 ) * 1000;
      
      ret = ( pthread_mutex_timedlock( &_mutexPrivateHandle->_mutex, &theTimeSpec ) != ETIMEDOUT );
      
   #endif

      return ret;
   }

   void CMutex::UnLock()
   {
   #ifdef __WIN32__
   	if( _lockCount )
   	{
   	   --_lockCount;

   		ReleaseMutex( _mutexPrivateHandle->_mutex );
   	}
   #elif __UNIX__
   	pthread_mutex_unlock( &_mutexPrivateHandle->_mutex );
   #endif
   }

   bool CMutex::ReLock( unsigned int aTime )
   {
      bool ret( false );

      UnLock();

      ret = Lock( aTime );

      return ret;
   }

   unsigned int CMutex::lockCount()const
   {
      return _lockCount;
   }
}
