/*
 * QueueThreadSafe.cxx
 *
 *  Created on: 07.10.2008
 *      Author: mars
 */

#include "QueueThreadSafe.hxx"
#include "ECURequest.h"

#include <l2MutexLocker.h>
#include <l2Trace.h>

template class CQueueThreadSafe< ECU::CRequest >;

template < class T >
   CQueueThreadSafe< T >::CQueueThreadSafe()
   {
      _ignorePush = false;
      _maxQueueSize = 1;
   }

template < class T >
   void CQueueThreadSafe< T >::SetMaxQueueSize( unsigned short aSize )
   {
      TRACE_FUN( Frequently, "CQueueThreadSafe< T >::SetMaxQueueSize" );

      l2max::CMutexLocker locker( _queueMutex );

      _maxQueueSize = aSize;

      if( !_maxQueueSize )
      {
         _maxQueueSize = 1;
      }
   }

template < class T >
   unsigned short CQueueThreadSafe< T >::maxQueueSize()const
   {
      TRACE_FUN( Frequently, "CQueueThreadSafe< T >::maxQueueSize" );

      l2max::CMutexLocker locker( _queueMutex );

      return _maxQueueSize;
   }

template < class T >
   void CQueueThreadSafe< T >::IgnorePush( bool anIgnore )
   {
      TRACE_FUN( Frequently, "CQueueThreadSafe< T >::IgnorePush" );

      l2max::CMutexLocker locker( _queueMutex );

      _ignorePush = anIgnore;
   }

template < class T >
   bool CQueueThreadSafe< T >::front( T& t )const
   {
      TRACE_FUN( Frequently, "CQueueThreadSafe< T >::front" );

      bool ret( false );

      l2max::CMutexLocker locker( _queueMutex );

      while( !( ret = _queue.size() ) )
      {
         ret = _changeCondition.wait( _queueMutex );
      }

      t = _queue.front();

      return ret;
   }

template < class T >
   void CQueueThreadSafe< T >::pop()
   {
      TRACE_FUN( Frequently, "CQueueThreadSafe< T >::pop" );

      l2max::CMutexLocker locker( _queueMutex );

      _queue.pop_front();

      _changeCondition.wake();
   }

template < class T >
   void CQueueThreadSafe< T >::push( const T& t )
   {
      TRACE_FUN( Frequently, "CQueueThreadSafe< T >::push" );

      l2max::CMutexLocker locker( _queueMutex );

      while( _queue.size() >= _maxQueueSize )
      {
         _changeCondition.wait( _queueMutex );
      }

      if( !_ignorePush )
      {
         _queue.push_back( t );
      }

      _changeCondition.wake();
   }

template < class T >
   void CQueueThreadSafe< T >::clear()
   {
      TRACE_FUN( Frequently, "CQueueThreadSafe< T >::clear" );

      l2max::CMutexLocker locker( _queueMutex );

      _queue.clear();

      _changeCondition.wake();
   }

template < class T >
   typename CQueueThreadSafe< T >::TQueue::size_type CQueueThreadSafe< T >::size()const
   {
      TRACE_FUN( Frequently, "CQueueThreadSafe< T >::size" );

      l2max::CMutexLocker locker( _queueMutex );

      return _queue.size();
   }
