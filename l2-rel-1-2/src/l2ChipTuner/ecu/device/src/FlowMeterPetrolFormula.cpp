/*
 * FlowMeterPetrolFormula.cpp
 *
 *  Created on: 23.10.2009
 *      Author: mars
 */

#include "FlowMeterPetrolFormula.h"
#include "ECUUnitsTable.h"

#include <AppDefs.h>
#include <AppContext.h>

#include <l2ExceptionThrower.h>

namespace ECU
{
   namespace FlowMeter
   {
      CFlowMeterPetrolFormula::CFlowMeterPetrolFormula( const CUnitsTable& aTable )
      {
         _rpmUnit = aTable.data( Diagnostic::dtPresentEngineSpeed );
         _pulseUnit = aTable.data( Diagnostic::dtBasePulseWidth );
         
         CHECK_OBJECT_STATE( !_rpmUnit.IsNull(), "RPM unit is not available" );
         CHECK_OBJECT_STATE( !_pulseUnit.IsNull(), "Base Pulse Width unit is not available" );
         
         l2max::CVariant injCal;
         CAppContext::GetInstance()->MainRegistry().Read( V_ECU_FLOWMETER_INJCAL, injCal );
         
         _injectorCalibration = injCal.toDouble();
      }
      
      CFlowMeterPetrolFormula::~CFlowMeterPetrolFormula(){}
      
      double CFlowMeterPetrolFormula::flow()const
      {
         double ret( .0 );
         
         ret += _injectorCalibration / 1000. / 60. * 2.;
         ret *= _pulseUnit->value( Diagnostic::uSecond ).toDouble() / 1000.;
         ret *= _rpmUnit->value( Diagnostic::uRevolutionsPerMinute ).toInt() / 60.;
         ret *= 2.;
         
         return ret;
      }
   }
}
