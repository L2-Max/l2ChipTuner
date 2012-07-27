#ifndef L2THREAD_H_
#define L2THREAD_H_

#include <l2Exception.h>

namespace l2max
{
   class CThread
   {
      struct SThreadPrivateHandle;
      
      SThreadPrivateHandle* _threadPrivateHandle;

#ifdef __WIN32__
      static unsigned int privateThreadProc( void* aParameter );
#elif __UNIX__
      static void* privateThreadProc( void* aParameter );
#endif

      CThread( const CThread& );
      CThread& operator = ( const CThread& );

      virtual unsigned int run()=0;

      void CloseThread();

   public:
      enum ELockType
      {
         ltInfinite = -1
      };

   	CThread();
   	virtual ~CThread();

   	static void Sleep( unsigned int ms );

   	void Start();

   	bool Wait( unsigned int ms = ltInfinite )const;
   	bool Terminate();
   	bool isRun()const;
   };
}

#endif /*L2THREAD_H_*/
