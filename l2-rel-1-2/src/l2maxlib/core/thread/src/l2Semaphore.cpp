/*
 * l2Semaphore.cpp
 *
 *  Created on: 08.10.2008
 *      Author: mars
 */

#include "l2Semaphore.h"

#include <l2ExceptionThrower.h>

#ifdef __WIN32__
	#include <windows.h>
#elif __UNIX__
	#include <semaphore.h>
#endif

namespace l2max
{
   struct CSemaphore::SSemaphorePrivateHandle
   {
   #ifdef __WIN32__
      HANDLE _semaphore;
   #elif __UNIX__
      sem_t _semaphore;
   #endif
   };
   
   CSemaphore::CSemaphore( int aCount )
   {
      _semaphorePrivateHandle = new SSemaphorePrivateHandle();
   
      bool isOkay( false );
   #ifdef __WIN32__
      isOkay = ( _semaphorePrivateHandle->_semaphore = CreateSemaphore( 0, aCount, aCount, 0 ) );
   #elif __UNIX__
   	isOkay = !sem_init( &_semaphorePrivateHandle->_semaphore, 1, aCount );
   #endif
   
   	CHECK_OBJECT_STATE( isOkay, "Semaphore not created" );
   }
   
   CSemaphore::~CSemaphore()
   {
   #ifdef __WIN32__
      CloseHandle( _semaphorePrivateHandle->_semaphore );
   #elif __UNIX__
      sem_destroy( &_semaphorePrivateHandle->_semaphore );
   #endif
   
      delete _semaphorePrivateHandle;
   }
   
   bool CSemaphore::Acquire( int aTimeout )
   {
      bool ret( false );
      
   #ifdef __WIN32__
      ret = ( WaitForSingleObject( _semaphorePrivateHandle->_semaphore, aTimeout ) == WAIT_OBJECT_0 );
   #elif __UNIX__
      ret = !sem_wait( &_semaphorePrivateHandle->_semaphore );
   #endif
      return ret;
   }
   
   void CSemaphore::Release()
   {
   #ifdef __WIN32__
      ReleaseSemaphore( _semaphorePrivateHandle->_semaphore, 1, 0 );
   #elif __UNIX__
      sem_post( &_semaphorePrivateHandle->_semaphore );
   #endif
   }
}
