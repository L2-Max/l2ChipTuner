/*
 * ECUDeviceStatistic.h
 *
 *  Created on: 15.01.2009
 *      Author: mars
 */

#ifndef ECUDEVICESTATISTIC_H_
#define ECUDEVICESTATISTIC_H_

#include <l2Mutex.h>
#include <l2Singleton.hxx>

namespace ECU
{
   class CDeviceStatistic : public l2max::CSingleton< CDeviceStatistic >
   {
      friend class l2max::CSingleton< CDeviceStatistic >;
      
      mutable l2max::CMutex _mutex;
      
      unsigned int _reqCount;
      unsigned int _reqQueue;
      unsigned int _reqQueueMax;
      unsigned int _reqErrorCount;
      
      CDeviceStatistic( const CDeviceStatistic& );
      CDeviceStatistic& operator = ( const CDeviceStatistic& );
      
      CDeviceStatistic();
      virtual ~CDeviceStatistic();
      
   public:
      void IncReqCount();
      void IncReqErrorCount();
      
      unsigned int reqCount()const;
      void ResetReqCount();
      
      void SetReqQueueSize( unsigned int aSize );
      unsigned int reqQueueSize()const;
      
      void SetReqQueueMaxSize( unsigned int aSize );
      unsigned int reqQueueMaxSize()const;
      
      unsigned int reqErrorCount()const;
      void ResetReqErrorCount();
   };
}

#endif /* ECUDEVICESTATISTIC_H_ */
