/*
 * ECUDeviceReadDataTimer.h
 *
 *  Created on: 19.02.2009
 *      Author: mars
 */

#ifndef ECUDEVICREADDATATIMER_H_
#define ECUDEVICREADDATATIMER_H_

#include <l2Timer.h>

namespace ECU
{
   class CDevice;
   class CRequestQueue;

   class CDeviceReadDataTimer : public l2max::CTimer
   {
      CDevice& _device;
      
      virtual void timeout();

   public:
      CDeviceReadDataTimer( CDevice& aDevice );
      virtual ~CDeviceReadDataTimer();
      
      void StartTimer();
      
      void IncInterval();
      void DecInterval();
   };
}

#endif /* ECUDEVICREADDATATIMER_H_ */
