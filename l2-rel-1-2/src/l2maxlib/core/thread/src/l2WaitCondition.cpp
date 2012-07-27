/*
 * l2WaitCondition.cpp
 *
 *  Created on: 08.10.2008
 *      Author: mars
 */

#include "l2WaitCondition.h"
#include "l2Mutex.h"

#include <l2Trace.h>
#include <l2ExceptionThrower.h>

#ifdef __WIN32__
	#include <windows.h>
#elif __UNIX__
	#include <pthread.h>
#endif

namespace l2max
{
   struct CWaitCondition::SWaitConditionPrivateHandle
   {
   #ifdef __WIN32__
   	HANDLE _waitCondition;
   #elif __UNIX__
   	pthread_cond_t _waitCondition;
   #endif
   };
   
   CWaitCondition::CWaitCondition( bool aManual )
   {
      TRACE_FUN( Frequently, "CWaitCondition::CWaitCondition" );

      _waitConditionPrivateHandle = new SWaitConditionPrivateHandle();
   
   #ifdef __WIN32__
      _waitConditionPrivateHandle->_waitCondition = ::CreateEvent( 0, aManual, false, 0 );
   #elif __UNIX__
   	pthread_cond_init( &_waitConditionPrivateHandle->_waitCondition, 0 );
   #endif
   }

   CWaitCondition::~CWaitCondition()
   {
      TRACE_FUN( Frequently, "CWaitCondition::~CWaitCondition" );

   #ifdef __WIN32__
      ::CloseHandle( _waitConditionPrivateHandle->_waitCondition );
   #elif __UNIX__
   	pthread_cond_destroy( &_waitConditionPrivateHandle->_waitCondition );
   #endif
   	
   	delete _waitConditionPrivateHandle;
   }

   bool CWaitCondition::wait( CMutex& aMutex, unsigned int aTime )
   {
      TRACE_FUN( Frequently, "CWaitCondition::wait" );

      bool ret( false );

      aMutex.UnLock();

      ret = waitForEvent( aTime );

      aMutex.Lock();

      return ret;
   }

   bool CWaitCondition::waitForEvent( unsigned int aTime )
   {
      bool ret( false );
      
   #ifdef __WIN32__
      ret = ( ::WaitForSingleObject( _waitConditionPrivateHandle->_waitCondition, aTime ) == WAIT_OBJECT_0 );
	#elif __UNIX__
		timespec theTimeSpec;
		theTimeSpec.tv_sec = aTime / 1000;
		theTimeSpec.tv_nsec = ( aTime % 1000 ) * 1000;
		
		ret = !pthread_cond_timedwait( &_waitConditionPrivateHandle->_waitCondition, 0, &theTimeSpec );
	#endif
		
      return ret;
   }

   void CWaitCondition::wake()
   {
      TRACE_FUN( Frequently, "CWaitCondition::wake" );
      
   #ifdef __WIN32__
      ::SetEvent( _waitConditionPrivateHandle->_waitCondition );
   #elif __UNIX__
   	pthread_cond_signal( &_waitConditionPrivateHandle->_waitCondition );
   #endif
   }

   bool CWaitCondition::reset()
   {
      TRACE_FUN( Frequently, "CWaitCondition::reset" );
      
      bool ret( false );
      
   #ifdef __WIN32__
   	ret = ::ResetEvent( _waitConditionPrivateHandle->_waitCondition );
   #elif __UNIX__

   #endif
      
      return ret;
   }
}
