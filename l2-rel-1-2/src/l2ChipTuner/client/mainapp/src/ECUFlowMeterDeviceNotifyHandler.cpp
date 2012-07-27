/*
 * ECUFlowMeterDeviceNotifyHandler.cpp
 *
 *  Created on: 12.12.2008
 *      Author: mars
 */

#include "ECUFlowMeterDeviceNotifyHandler.h"

CECUFlowMeterDeviceNotifyHandler::CECUFlowMeterDeviceNotifyHandler( QObject* aSuccessor )
{
   connect( this, SIGNAL( updatedEvent() ), aSuccessor, SLOT( OnUpdated() ) );
}

CECUFlowMeterDeviceNotifyHandler::~CECUFlowMeterDeviceNotifyHandler(){}

void CECUFlowMeterDeviceNotifyHandler::updated()
{
   updatedEvent();
}
