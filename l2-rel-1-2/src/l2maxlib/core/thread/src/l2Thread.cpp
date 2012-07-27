#include "l2Thread.h"

#include <l2Trace.h>
#include <l2Exception.h>
#include <l2ExceptionThrower.h>

#ifdef __WIN32__
	#include <windows.h>
#elif __UNIX__
	#include <pthread.h>
#endif

namespace l2max
{
#ifdef __WIN32__
   unsigned int CThread::privateThreadProc( void* aParameter )
   {
      TRACE_FUN( Routine, "CThread::privateThreadProc" );

      return reinterpret_cast< CThread* >( aParameter )->run();
   }
#elif __UNIX__
   void* CThread::privateThreadProc( void* aParameter )
   {
      TRACE_FUN( Routine, "CThread::privateThreadProc" );

      return reinterpret_cast< void* >( reinterpret_cast< CThread* >( aParameter )->run() );
   }
#endif
   
   struct CThread::SThreadPrivateHandle
   {
   #ifdef __WIN32__
   	HANDLE _thread;
   #elif __UNIX__
   	pthread_t _thread;
   #endif
   };

   CThread::CThread()
   {
      _threadPrivateHandle = 0;
   }

   CThread::~CThread()
   {
      if( isRun() )
      {
         TraceCritical << "Destroying running thread" << std::endl;

         Terminate();
      }

      CloseThread();
   }

   void CThread::CloseThread()
   {
      if( _threadPrivateHandle )
      {
      #ifdef __WIN32__
         CloseHandle( _threadPrivateHandle->_thread );
      #elif __UNIX__
      #endif
      }
      
      delete _threadPrivateHandle;
      _threadPrivateHandle = 0;
   }

   void CThread::Start()
   {
      TRACE_FUN( Routine, "CThread::Start" );

      bool isOkay( false );
      
      CloseThread();
      
      _threadPrivateHandle = new SThreadPrivateHandle();
      
   #ifdef __WIN32__
      unsigned int threadId( 0 );

      isOkay = ( _threadPrivateHandle->_thread = CreateThread( 0, 0, reinterpret_cast< LPTHREAD_START_ROUTINE >( CThread::privateThreadProc ),
                                                               this, 0, reinterpret_cast< DWORD* >( &threadId ) ) );
	#elif __UNIX__
	   isOkay = !pthread_create( &_threadPrivateHandle->_thread, 0, CThread::privateThreadProc, this );
	#endif
	
      CHECK_OBJECT_STATE( isOkay, "Thread not created" );
   }

   void CThread::Sleep( unsigned int ms )
   {
   #ifdef __WIN32__
      ::Sleep( ms );
   #elif __UNIX__
      timespec theTimeSpec;
      theTimeSpec.tv_sec = ms / 1000;
      theTimeSpec.tv_nsec = ( ms % 1000 ) * 1000;
      
      timespec remaining;
      
   	nanosleep( &theTimeSpec, &remaining );
   #endif
   }

   bool CThread::Wait( unsigned int ms )const
   {
      bool ret( false );
      
      if( _threadPrivateHandle )
      {
      #ifdef __WIN32__
         ret = ( ::WaitForSingleObject( _threadPrivateHandle->_thread, ( ms == ltInfinite ? INFINITE : ms ) ) == WAIT_OBJECT_0 );
      #elif __UNIX__
         void* res( 0 );
         ret = !pthread_join( _threadPrivateHandle->_thread, &res );
      #endif
      }
      
      return ret;
   }

   bool CThread::Terminate()
   {
   	bool ret( false );
   	
   	if( _threadPrivateHandle )
   	{
      #ifdef __WIN32__
         ret = ::TerminateThread( _threadPrivateHandle->_thread, ltInfinite );
      #elif __UNIX__
         ret = !pthread_cancel( _threadPrivateHandle->_thread );
      #endif
   	}
   	
  		CloseThread();
  	
      return ret;
   }

   bool CThread::isRun()const
   {
      return _threadPrivateHandle && !Wait( 0 );
   }
}
