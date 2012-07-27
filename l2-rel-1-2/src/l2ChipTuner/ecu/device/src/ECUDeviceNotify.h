/*
 * ECUDeviceNotify.h
 *
 *  Created on: 05.10.2008
 *      Author: L2-Max
 */

#ifndef ECUDEVICENOTIFY_H_
#define ECUDEVICENOTIFY_H_

#include <AbstractProto.h>

namespace ECU
{
   struct IDeviceNotify
   {
      virtual void diagnosticSessionStarted() = 0;
      virtual void diagnosticSessionStoped() = 0;
      virtual void dataRead() = 0;
      virtual void dataWriten( CAbstractProto::EWriteResultCode aCode ) = 0;
      virtual void stopped() = 0;
      virtual void statUpdated() = 0;

      virtual ~IDeviceNotify(){}
   };
}

#endif /* ECUDEVICENOTIFY_H_ */
