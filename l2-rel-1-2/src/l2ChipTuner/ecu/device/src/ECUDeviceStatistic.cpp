/*
 * ECUDeviceStatistic.cpp
 *
 *  Created on: 15.01.2009
 *      Author: mars
 */

#include "ECUDeviceStatistic.h"

#include <l2MutexLocker.h>

namespace ECU
{
   CDeviceStatistic::CDeviceStatistic()
   {
      _reqCount = 0;
      _reqQueue = 0;
      _reqErrorCount = 0;
   }
   
   CDeviceStatistic::~CDeviceStatistic(){}
   
   void CDeviceStatistic::IncReqCount()
   {
      l2max::CMutexLocker locker( _mutex );
      
      ++_reqCount;
   }
   
   unsigned int CDeviceStatistic::reqCount()const
   {
      l2max::CMutexLocker locker( _mutex );
      
      return _reqCount;
   }
   
   void CDeviceStatistic::ResetReqCount()
   {
      l2max::CMutexLocker locker( _mutex );
      
      _reqCount = 0;
   }
   
   void CDeviceStatistic::SetReqQueueSize( unsigned int aSize )
   {
      l2max::CMutexLocker locker( _mutex );
      
      _reqQueue = aSize;
   }
   
   unsigned int CDeviceStatistic::reqQueueSize()const
   {
      l2max::CMutexLocker locker( _mutex );
      
      return _reqQueue;
   }
   
   void CDeviceStatistic::SetReqQueueMaxSize( unsigned int aSize )
   {
      l2max::CMutexLocker locker( _mutex );
      
      _reqQueueMax = aSize;
   }
   
   unsigned int CDeviceStatistic::reqQueueMaxSize()const
   {
      l2max::CMutexLocker locker( _mutex );
      
      return _reqQueueMax;
   }
   
   void CDeviceStatistic::IncReqErrorCount()
   {
      l2max::CMutexLocker locker( _mutex );
      
      ++_reqErrorCount;
   }
   
   unsigned int CDeviceStatistic::reqErrorCount()const
   {
      l2max::CMutexLocker locker( _mutex );
      
      return _reqErrorCount;
   }
   
   void CDeviceStatistic::ResetReqErrorCount()
   {
      l2max::CMutexLocker locker( _mutex );
      
      _reqErrorCount = 0;
   }
}
