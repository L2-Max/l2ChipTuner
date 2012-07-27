/*
 * QueueThreadSafe.h
 *
 *  Created on: 07.10.2008
 *      Author: mars
 */

#ifndef QUEUETHREADSAFE_HXX_
#define QUEUETHREADSAFE_HXX_

#include <l2Mutex.h>
#include <l2WaitCondition.h>

#include <deque>

template < class T >
   class CQueueThreadSafe
   {
      bool _ignorePush;
      unsigned short _maxQueueSize;

      mutable l2max::CMutex _queueMutex;
      mutable l2max::CWaitCondition _changeCondition;

      typedef std::deque< T > TQueue;
      TQueue _queue;

   public:
      CQueueThreadSafe();
      
      void SetMaxQueueSize( unsigned short aSize );
      void IgnorePush( bool anIgnore );
      
      unsigned short maxQueueSize()const;
      
      bool front( T& t )const;
      void pop();
      void push( const T& t );
      void clear();
      typename TQueue::size_type size()const;
   };

#endif /* QUEUETHREADSAFE_H_ */
