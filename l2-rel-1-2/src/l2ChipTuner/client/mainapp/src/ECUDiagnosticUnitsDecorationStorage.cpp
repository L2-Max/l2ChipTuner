/*
 * ECUDiagnosticUnitsDecorationStorage.cpp
 *
 *  Created on: 17.02.2009
 *      Author: mars
 */

#include "ECUDiagnosticUnitsDecorationStorage.h"

CECUDiagnosticUnitsDecorationStorage::CECUDiagnosticUnitsDecorationStorage()
{
   _dataUnitReadOnlyIcon = QIcon( ":/Device/View/deviceViewMonitorUnitRDO.png" );
   _dataUnitReadWriteIcon = QIcon( ":/Device/View/deviceViewMonitorUnitRW.png" );
}

CECUDiagnosticUnitsDecorationStorage::~CECUDiagnosticUnitsDecorationStorage(){}

QIcon CECUDiagnosticUnitsDecorationStorage::unitIcon( EUnitType aType )const
{
   QIcon ret;

   if( aType == utReadOnly )
   {
      ret = _dataUnitReadOnlyIcon;
   }
   else if( aType == utReadWrite )
   {
      ret = _dataUnitReadWriteIcon;
   }

   return ret;
}
