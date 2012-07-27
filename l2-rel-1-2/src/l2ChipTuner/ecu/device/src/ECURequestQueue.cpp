/*
 * ECURequestQueue.cpp
 *
 *  Created on: 25.01.2009
 *      Author: mars
 */

#include "ECURequestQueue.h"

#include <l2Timer.h>

namespace ECU
{
   CRequestQueue::CRequestQueue(){}
   CRequestQueue::~CRequestQueue(){}

   CRequestQueue::ESizeState CRequestQueue::sizeState()const
   {
      CRequestQueue::ESizeState ret( ssNoChange );

      static unsigned int lastQueueSize( 0 );
      static unsigned int standTimeout( 0 );
      static int queueGrowing( -1 );

      double queueDiff( double( size() ) / ( maxQueueSize() / 2 ) );

      if( queueDiff > 1. )
      {
         if( queueGrowing >= 0 )
         {
            ret = ssInc;
         }
      }
      else if( queueDiff < 1. )
      {
         if( queueGrowing <= 0 )
         {
            ret = ssDec;
         }
      }

      if( size() < lastQueueSize )
      {
         queueGrowing = -1;
         standTimeout = 0;
      }
      else if( size() > lastQueueSize )
      {
         queueGrowing = 1;
         standTimeout = 0;
      }
      else
      {
         static unsigned int lastTimeout( l2max::CTimer::clock() );

         standTimeout += l2max::CTimer::clock() - lastTimeout;
         
         if( !standTimeout )
         {
            ++standTimeout;
         }
         
         lastTimeout = l2max::CTimer::clock();

         if( standTimeout > 1000 )
         {
            queueGrowing = 0;
         }
      }

      lastQueueSize = size();

      return ret;
   }
}
