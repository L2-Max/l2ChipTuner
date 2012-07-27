/*
 * ECUFlowMeterDeviceView.h
 *
 *  Created on: 10.12.2008
 *      Author: mars
 */

#ifndef ECUFLOWMETERDEVICEVIEW_H_
#define ECUFLOWMETERDEVICEVIEW_H_

#include "ECUFlowMeterDeviceNotifyHandler.h"

#include <QScrollArea>

class CFlowMeterDeviceViewPage;

namespace ECU
{
   class CDevice;

   namespace FlowMeter
   {
      class CDevice;
   }
}

class CECUFlowMeterDeviceView : public QScrollArea
{
   Q_OBJECT;

   ECU::FlowMeter::CDevice* _flowMeterDevice;

   CFlowMeterDeviceViewPage* _flowMeterDeviceViewPage;

   CECUFlowMeterDeviceNotifyHandler _ecuFlowMeterDeviceNotifyHandler;

public:
   CECUFlowMeterDeviceView( QWidget* aParent );
   virtual ~CECUFlowMeterDeviceView();

   void InstallFlowMeterDevice( ECU::FlowMeter::CDevice* aMeter );

private slots:
   void OnUpdated();
   void OnReset();
};

#endif /* ECUFLOWMETERDEVICEVIEW_H_ */
