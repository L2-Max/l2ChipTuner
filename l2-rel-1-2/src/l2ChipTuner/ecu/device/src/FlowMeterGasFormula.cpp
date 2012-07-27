/*
 * FlowMeterGasFormula.cpp
 *
 *  Created on: 23.10.2009
 *      Author: mars
 */

#include "FlowMeterGasFormula.h"
#include "ECUUnitsTable.h"

#include <AppDefs.h>
#include <AppContext.h>

#include <l2ExceptionThrower.h>

namespace ECU
{
   namespace FlowMeter
   {
      CFlowMeterGasFormula::CFlowMeterGasFormula( const CUnitsTable& aTable )
      {
         _rpmUnit = aTable.data( Diagnostic::dtPresentEngineSpeed );
         _mapUnit = aTable.data( Diagnostic::dtMap );
         _matUnit = aTable.data( Diagnostic::dtMat );
         _cutOffState = aTable.data( Diagnostic::dtDecelCutOff );
         
         CHECK_OBJECT_STATE( !_rpmUnit.IsNull(), "RPM unit is not available" );
         CHECK_OBJECT_STATE( !_mapUnit.IsNull(), "MAP unit is not available" );
         CHECK_OBJECT_STATE( !_matUnit.IsNull(), "MAT unit is not available" );
         
         l2max::CVariant literWeight;
         CAppContext::GetInstance()->MainRegistry().Read( V_ECU_FLOWMETER_GASLITERWEIGHT, literWeight );
         
         _literWeight = literWeight.toDouble();
         
         
         l2max::CVariant lambda;
         CAppContext::GetInstance()->MainRegistry().Read( V_ECU_FLOWMETER_GASLAMBDA, lambda );
         
         _lambda = lambda.toDouble();
      }
      
      CFlowMeterGasFormula::~CFlowMeterGasFormula(){}
      
      double CFlowMeterGasFormula::flow()const
      {
         double ret( .0 );

         if( _cutOffState.IsNull() || !_cutOffState->value( Diagnostic::uBinary ).toBool() )
         {
            static std::map< int, double > approxAirVolumeCorrection;
   
            if( !approxAirVolumeCorrection.size() )
            {
               approxAirVolumeCorrection[ 1000 ] = .55;
               approxAirVolumeCorrection[ 2000 ] = .6;
               approxAirVolumeCorrection[ 3000 ] = .7;
               approxAirVolumeCorrection[ 4000 ] = .8;
               approxAirVolumeCorrection[ 5000 ] = .85;
               approxAirVolumeCorrection[ 6000 ] = .9;
               approxAirVolumeCorrection[ 7000 ] = .95;
            }
   
            int rpm( _rpmUnit->value( Diagnostic::uRevolutionsPerMinute ).toInt() );
            int map( _mapUnit->value( Diagnostic::uPascal ).toInt() );
            int mat( _matUnit->value( Diagnostic::uCelsius ).toInt() );
            
            double airVolume( 1.5 * rpm / 2. / 60. );
            
            airVolume *= approxAirVolumeCorrection.lower_bound( rpm )->second;
   
            double airMass( ( map * airVolume / ( 8.3114 * ( mat + 273.15 ) ) ) * ( .029 ) );
            
            ret = airMass / _lambda;
            ret /= _literWeight;
         }
         
         return ret;
      }
   }
}
