/*
 * AbstractMeterModel.cpp
 *
 *  Created on: 05.03.2009
 *      Author: mars
 */

#include "AbstractMeterModel.h"

#include <AppDefs.h>
#include <ECUDevice.h>
#include <ECUUnitsTable.h>
#include <ECUDeviceView.h>

#include <l2Trace.h>

CAbstractMeterModel::CAbstractMeterModel( const Diagnostic::CUnitKey& aKey,
                                          const CECUDeviceView& aDeviceView,
                                          IMeterUpdateConsumer& aUpdateConsumer ) :
 _deviceView( aDeviceView ),                                             
 _updateConsumer( aUpdateConsumer )
{
   TRACE_FUN( Routine, "CAbstractMeterModel::CAbstractMeterModel" );
   
   _dataUnit = aDeviceView.ecuDevice().unitsTable().data( aKey );
   _registryKey = S_METER_GRAPH + _deviceView.registryNamespace() + "::" + _dataUnit->synonym();
}

CAbstractMeterModel::~CAbstractMeterModel()
{
   TRACE_FUN( Routine, "CAbstractMeterModel::~CAbstractMeterModel" );
}

std::string CAbstractMeterModel::unit()const
{
   return _dataUnit->name();
}

std::string CAbstractMeterModel::text()const
{
   return _dataUnit->text();
}

const std::string& CAbstractMeterModel::registryKey()const
{
   return _registryKey;
}

void CAbstractMeterModel::update(){}

IMeterUpdateConsumer& CAbstractMeterModel::updateConsumer()const
{
   return _updateConsumer;
}

ECU::HDataUnit CAbstractMeterModel::dataUnit()const
{
   return _dataUnit;
}
