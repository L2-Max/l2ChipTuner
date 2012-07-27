/*
 * ECUDeviceUnitSelectionModel.cpp
 *
 *  Created on: 28.09.2009
 *      Author: mars
 */

#include "ECUDeviceUnitSelectionModel.h"

CECUDeviceUnitSelectionModel::CECUDeviceUnitSelectionModel(){}
CECUDeviceUnitSelectionModel::~CECUDeviceUnitSelectionModel(){}

void CECUDeviceUnitSelectionModel::Select( const Diagnostic::TUnitKeys& aKeys )
{
   _selectedKeys = aKeys;
}

void CECUDeviceUnitSelectionModel::SetCurrent( const Diagnostic::CUnitKey& aKey )
{
   _currentKey = aKey;
}

void CECUDeviceUnitSelectionModel::Clear()
{
   _selectedKeys.clear();
   _currentKey = Diagnostic::CUnitKey();
}

const Diagnostic::TUnitKeys& CECUDeviceUnitSelectionModel::selected()const
{
   return _selectedKeys;
}

const Diagnostic::CUnitKey& CECUDeviceUnitSelectionModel::current()const
{
   return _currentKey;
}

bool CECUDeviceUnitSelectionModel::hasSelection()const
{
   return current().isValid() || selected().size();
}
