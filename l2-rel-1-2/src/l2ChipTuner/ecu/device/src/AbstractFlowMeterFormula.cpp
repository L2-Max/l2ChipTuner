/*
 * AbstractFlowMeterFormula.cpp
 *
 *  Created on: 23.10.2009
 *      Author: mars
 */

#include "AbstractFlowMeterFormula.h"
#include "FlowMeterPetrolFormula.h"
#include "FlowMeterGasFormula.h"

#include <AppDefs.h>
#include <AppContext.h>
#include <FlowMeterDefs.h>

namespace ECU
{
   namespace FlowMeter
   {
      namespace Factory
      {
         CAbstractFlowMeterFormula* CreateFlowMeterFormula( const CUnitsTable& aTable )
         {
            CAbstractFlowMeterFormula* ret( 0 );
            
            l2max::CVariant fuelType;
            CAppContext::GetInstance()->MainRegistry().Read( V_ECU_FLOWMETER_FUELTYPE, fuelType );
            
            if( fuelType.toInt() == ::FlowMeter::ftGas )
            {
               ret = new CFlowMeterGasFormula( aTable );
            }
            else
            {
               ret = new CFlowMeterPetrolFormula( aTable );
            }
            
            return ret;
         }
      }
      
      CAbstractFlowMeterFormula::CAbstractFlowMeterFormula(){}
      CAbstractFlowMeterFormula::~CAbstractFlowMeterFormula(){}
   }
}
