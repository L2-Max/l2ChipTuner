#ifndef L2TRACE_H_
#define L2TRACE_H_

#include "l2TraceDefs.h"

#include <string>
#include <ostream>

namespace l2max
{
   /////////////////////////////////////////////////////////////////////////
   class CTraceImpl
   {
      CTraceImpl( CTraceImpl& );
      CTraceImpl& operator = ( const CTraceImpl& );

   protected:
      CTraceImpl();

   public:
      CTraceImpl& operator << ( std::ostream&( *pfn )( std::ostream& ) );

      template < class T >
         CTraceImpl& operator << ( T aValue );
   };

   /////////////////////////////////////////////////////////////////////////
   class CMutex;

   class CTrace : protected CTraceImpl
   {
      static CMutex _mutex;

      CTrace( const CTrace& );
      CTrace& operator = ( const CTrace& );

   public:
      CTrace( const std::string aFile, unsigned int aLine );
      ~CTrace();

      static bool IsLevelMatch( ETraceLevel aLevel );

      CTraceImpl& impl();
   };

	class CTraceFn
	{
	   static unsigned short _dept;
	   static CMutex _mutex;

	   bool _isShow;

	   unsigned int _line;
	   unsigned long _callTime;
	   unsigned long long _threadTime;

		std::string _name;
      std::string _file;
      
	public:
		CTraceFn( ETraceLevel aLevel, const std::string& aName,
		                              const std::string aFile,
		                              unsigned int aLine );
		~CTraceFn();
		
		static unsigned long CallTime();
		static unsigned long long ThreadTime();
	};
}

#endif /*L2TRACE_H_*/
