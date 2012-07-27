#include "l2Trace.h"
#include "l2TraceStream.h"
#include "l2Profiler.h"

#include <l2Mutex.h>
#include <l2Timer.h>

#ifdef __WIN32__
   #include <windows.h>
#endif

namespace l2max
{
   /////////////////////////////////////////////////////////////////////
   CTraceImpl::CTraceImpl(){}

   template < class T >
      CTraceImpl& CTraceImpl::operator << ( T aValue )
      {
         CTraceStream::GetInstance()->stream() << aValue;

         return *this;
      }

   template <>
      CTraceImpl& CTraceImpl::operator << ( bool aValue )
      {
         CTraceStream::GetInstance()->stream() << ( aValue ? "true" : "false" );

         return *this;
      }

   CTraceImpl& CTraceImpl::operator << ( std::ostream&( *pfn )( std::ostream& ) )
   {
      CTraceStream::GetInstance()->stream() << pfn;

      return *this;
   }

   template CTraceImpl& CTraceImpl::operator << ( char );
   template CTraceImpl& CTraceImpl::operator << ( unsigned char );
   template CTraceImpl& CTraceImpl::operator << ( short );
   template CTraceImpl& CTraceImpl::operator << ( unsigned short );
   template CTraceImpl& CTraceImpl::operator << ( int );
   template CTraceImpl& CTraceImpl::operator << ( unsigned int );
   template CTraceImpl& CTraceImpl::operator << ( long );
   template CTraceImpl& CTraceImpl::operator << ( unsigned long );
   template CTraceImpl& CTraceImpl::operator << ( unsigned long long );
   template CTraceImpl& CTraceImpl::operator << ( float );
   template CTraceImpl& CTraceImpl::operator << ( double );
   template CTraceImpl& CTraceImpl::operator << ( void* );

   /////////////////////////////////////////////////////////////////////
   CMutex CTrace::_mutex;

   CTrace::CTrace( const std::string aFile, unsigned int aLine )
   {
      CTrace::_mutex.Lock();
   }

   CTrace::~CTrace()
   {
      CTrace::_mutex.UnLock();
   }

   bool CTrace::IsLevelMatch( ETraceLevel aLevel )
   {
      return CTraceStream::GetInstance()->IsLevelMatch( aLevel );
   }

   CTraceImpl& CTrace::impl()
   {
      CTraceImpl::operator << ( CTraceStream::dateTimeMsecs() ) << ":TH"

   #ifdef __WIN32__
                           << GetCurrentThreadId()
	#endif
	                        << ": ";
      return *this;
   }

   /////////////////////////////////////////////////////////////////////
   unsigned short CTraceFn::_dept( 0 );

   CMutex CTraceFn::_mutex;

   CTraceFn::CTraceFn( ETraceLevel aLevel, const std::string& aName,
                                           const std::string aFile,
                                           unsigned int aLine )
   {
      CMutexLocker locker( CTraceFn::_mutex );

      _name = aName;
      _file = aFile;
      _line = aLine;

      _isShow = CTrace::IsLevelMatch( aLevel );

      if( _isShow )
      {
         std::string spaces( CTraceFn::_dept * 3, ' ' );

         CTrace( _file, _line ).impl() << "TraceFn: " << spaces << _name << " | C" << std::endl;

         ++CTraceFn::_dept;
      }
      
      _callTime = CTraceFn::CallTime();
      _threadTime = CTraceFn::ThreadTime();
   }

   CTraceFn::~CTraceFn()
   {
      CMutexLocker locker( CTraceFn::_mutex );
      
      unsigned long callTimeDiff( CTraceFn::CallTime() - _callTime );
      unsigned long long threadTimeDiff( CTraceFn::ThreadTime() - _threadTime );

      CProfiler::GetInstance()->RegisterFunction( _name, callTimeDiff, threadTimeDiff );

      if( _isShow )
      {
         --CTraceFn::_dept;

         std::string spaces( CTraceFn::_dept * 3, ' ' );

         CTrace( _file, _line ).impl() << "TraceFn: " << spaces << _name << " | R" << std::endl;
      }
   }
   
   unsigned long CTraceFn::CallTime()
   {
      return ::GetTickCount();
   }

   unsigned long long CTraceFn::ThreadTime()
   {
      unsigned long long ret( 0 );
      
      FILETIME creationTime;
      FILETIME exitTime;
      FILETIME kernelTime;
      FILETIME userTime;
      
      if( GetThreadTimes( GetCurrentThread(), &creationTime, &exitTime, &kernelTime, &userTime ) )
      {
         unsigned long long theKernelTime( static_cast< unsigned long long >( kernelTime.dwHighDateTime ) << 32 | kernelTime.dwLowDateTime );
         unsigned long long theUserTime( static_cast< unsigned long long >( userTime.dwHighDateTime ) << 32 | userTime.dwLowDateTime );
         
         ret = ( theKernelTime + theUserTime );
      }
      
      return ret;
   }
}
