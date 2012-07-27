/*
 * l2Timer.cpp
 *
 *  Created on: 10.10.2008
 *      Author: mars
 */

#include "l2Timer.h"
#include "l2MutexLocker.h"
#include "l2ITimerConsumer.h"

#include <l2Trace.h>

namespace l2max
{
   CTimer::CTimer(){}

   CTimer::~CTimer()
   {
      StopTimer();
   }

   void CTimer::timeout()
   {
      CMutexLocker locker( _mutex );

      for( TTimerConsumers::iterator i( _consumers.begin() ); i != _consumers.end(); ++i )
      {
         ( *i )->timeout();
      }
   }

   unsigned int CTimer::run()
   {
      TRACE_FUN( Routine, "CTimer::run" );

      unsigned int ret( 0 );

      unsigned int interval( _interval );

      while( !_stopCondition.waitForEvent( interval ) )
      {
         unsigned int oldTime( CTimer::clock() );

         timeout();

         unsigned int newTime( CTimer::clock() );

         CMutexLocker locker( _mutex );

         interval = _interval - ( newTime - oldTime );

         if( interval > _interval )
         {
            interval = 0;
         }
      }

      return ret;
   }

   unsigned int CTimer::clock()
   {
      return ::clock();
   }

   void CTimer::StartTimer( unsigned int aInterval )
   {
      if( !isRun() )
      {
         ChangeInterval( aInterval );

         Start();
      }
   }

   void CTimer::StopTimer()
   {
      if( isRun() )
      {
         _stopCondition.wake();

         Wait();
      }
   }

   void CTimer::ChangeInterval( unsigned int anInterval )
   {
      CMutexLocker locker( _mutex );

      _interval = anInterval;
   }

   unsigned int CTimer::interval()const
   {
      return _interval;
   }

   void CTimer::RegisterConsumer( ITimerConsumer* aConsumer )
   {
      CMutexLocker locker( _mutex );

      _consumers.insert( aConsumer );
   }
}
