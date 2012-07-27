/*
 * ECUFlowMeterDeviceNotifyHandler.h
 *
 *  Created on: 12.12.2008
 *      Author: mars
 */

#ifndef ECUFLOWMETERDEVICENOTIFYHANDLER_H_
#define ECUFLOWMETERDEVICENOTIFYHANDLER_H_

#include <ECUFlowMeterDeviceNotify.h>

#include <QObject>

class CECUFlowMeterDeviceNotifyHandler : public QObject,
                                         public ECU::FlowMeter::IDeviceNotify
{
   Q_OBJECT;

   virtual void updated();

public:
   CECUFlowMeterDeviceNotifyHandler( QObject* aSuccessor );
   virtual ~CECUFlowMeterDeviceNotifyHandler();

signals:
   void updatedEvent();
};

#endif /* ECUFLOWMETERDEVICENOTIFYHANDLER_H_ */
