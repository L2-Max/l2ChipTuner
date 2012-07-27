/*
 * ECUFlowMeterDeviceNotify.h
 *
 *  Created on: 12.12.2008
 *      Author: mars
 */

#ifndef ECUFLOWMETERDEVICENOTIFY_H_
#define ECUFLOWMETERDEVICENOTIFY_H_

namespace ECU
{
   namespace FlowMeter
   {
      struct IDeviceNotify
      {
         virtual void updated() = 0;

         virtual ~IDeviceNotify(){}
      };
   }
}

#endif /* ECUFLOWMETERDEVICENOTIFY_H_ */
