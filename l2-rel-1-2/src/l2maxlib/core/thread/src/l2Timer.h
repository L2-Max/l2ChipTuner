/*
 * l2Timer.h
 *
 *  Created on: 10.10.2008
 *      Author: mars
 */

#ifndef L2TIMER_H_
#define L2TIMER_H_

#include "l2Thread.h"
#include "l2Mutex.h"
#include "l2WaitCondition.h"

#include <set>

namespace l2max
{
   struct ITimerConsumer;
   
   class CTimer : public CThread
   {
      typedef std::set< ITimerConsumer* > TTimerConsumers;
      TTimerConsumers _consumers;

      mutable CMutex _mutex;
      CWaitCondition _stopCondition;
      
      unsigned int _interval;
      
      virtual void timeout();
      virtual unsigned int run();
      
      CTimer( const CTimer& );
      CTimer& operator = ( const CTimer& );
      
   public:
      CTimer();
      virtual ~CTimer();

      static unsigned int clock();
      
      void StartTimer( unsigned int aInterval );
      void StopTimer();
      void ChangeInterval( unsigned int aInterval );
      
      unsigned int interval()const;
      
      void RegisterConsumer( ITimerConsumer* aConsumer );
   };
}

#endif /* L2TIMER_H_ */
