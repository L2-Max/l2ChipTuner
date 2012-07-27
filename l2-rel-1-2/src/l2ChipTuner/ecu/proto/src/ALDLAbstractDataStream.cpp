/*
 * ALDLAbstractDataStream.cpp
 *
 *  Created on: 27.10.2008
 *      Author: mars
 */

#include "ALDLAbstractDataStream.h"
#include "ALDLDiagnosticDataFormulas.h"

#include <l2AbstractIODevice.h>
#include <l2Variant.h>
#include <l2Trace.h>

namespace ALDL
{
   /////////////////////////////////////////////////////////////////////////////////////////////////
   CAbstractMode1Data0::CAbstractMode1Data0()
   {
      _dataMeasureUnitsMap[ Diagnostic::dtCoolantTemp ][ Diagnostic::uCelsius ]                       = Diagnostic::pNoPrefix;
      _dataMeasureUnitsMap[ Diagnostic::dtAirTemp ][ Diagnostic::uCelsius ]                           = Diagnostic::pNoPrefix;

      _dataMeasureUnitsMap[ Diagnostic::dtMap ][ Diagnostic::uVolt ]                                  = Diagnostic::pNoPrefix;
      _dataMeasureUnitsMap[ Diagnostic::dtMap ][ Diagnostic::uPascal ]                                = Diagnostic::pKilo;

      _dataMeasureUnitsMap[ Diagnostic::dtTps ][ Diagnostic::uVolt ]                                  = Diagnostic::pNoPrefix;
      _dataMeasureUnitsMap[ Diagnostic::dtTps ][ Diagnostic::uPercent ]                               = Diagnostic::pNoPrefix;

      _dataMeasureUnitsMap[ Diagnostic::dtPresentEngineSpeed ][ Diagnostic::uRevolutionsPerMinute ]   = Diagnostic::pNoPrefix;
      _dataMeasureUnitsMap[ Diagnostic::dtO2 ][ Diagnostic::uVolt ]                                   = Diagnostic::pMilli;
      _dataMeasureUnitsMap[ Diagnostic::dtCO ][ Diagnostic::uVolt ]                                   = Diagnostic::pNoPrefix;
      _dataMeasureUnitsMap[ Diagnostic::dtSpeed ][ Diagnostic::uKilometerPerHour ]                    = Diagnostic::pNoPrefix;
      _dataMeasureUnitsMap[ Diagnostic::dtMat ][ Diagnostic::uCelsius ]                               = Diagnostic::pNoPrefix;
      _dataMeasureUnitsMap[ Diagnostic::dtBat ][ Diagnostic::uVolt ]                                  = Diagnostic::pNoPrefix;
      _dataMeasureUnitsMap[ Diagnostic::dtSparcAdvance ][ Diagnostic::uDegree ]                       = Diagnostic::pNoPrefix;
      _dataMeasureUnitsMap[ Diagnostic::dtDesiredEngineSpeed ][ Diagnostic::uRevolutionsPerMinute ]   = Diagnostic::pNoPrefix;
      _dataMeasureUnitsMap[ Diagnostic::dtBasePulseWidth ][ Diagnostic::uSecond ]                     = Diagnostic::pMilli;


      _formulaMap[ Diagnostic::dtCoolantTemp ][ Diagnostic::uCelsius ]                       = *new CBasicFormula( .75, 0, -40 );
      _formulaMap[ Diagnostic::dtAirTemp ][ Diagnostic::uCelsius ]                           = *new CBasicFormula( .75, 0, -40 );

      _formulaMap[ Diagnostic::dtMap ][ Diagnostic::uVolt ]                                  = *new CBasicFormula( 5. / 255., 2 );
      _formulaMap[ Diagnostic::dtMap ][ Diagnostic::uPascal ]                                = *new CBasicFormula( 1. / 2.71, 0, 0, 28.06 );

      _formulaMap[ Diagnostic::dtTps ][ Diagnostic::uVolt ]                                  = *new CBasicFormula( 5. / 255., 2 );
      _formulaMap[ Diagnostic::dtTps ][ Diagnostic::uPercent ]                               = *new CBasicFormula( 1. / 2.55, 0 );

      _formulaMap[ Diagnostic::dtPresentEngineSpeed ][ Diagnostic::uRevolutionsPerMinute ]   = *new CBasicFormula( 25 );
      _formulaMap[ Diagnostic::dtO2 ][ Diagnostic::uVolt ]                                   = *new CBasicFormula( 4.34 );
      _formulaMap[ Diagnostic::dtCO ][ Diagnostic::uVolt ]                                   = *new CBasicFormula( 5. / 255., 2 );
      _formulaMap[ Diagnostic::dtSpeed ][ Diagnostic::uKilometerPerHour ]                    = *new CBasicFormula( 1. );
      _formulaMap[ Diagnostic::dtMat ][ Diagnostic::uCelsius ]                               = *new CBasicFormula( .75, 0, -40 );
      _formulaMap[ Diagnostic::dtBat ][ Diagnostic::uVolt ]                                  = *new CBasicFormula( 1. / 10., 1 );
      _formulaMap[ Diagnostic::dtSparcAdvance ][ Diagnostic::uDegree ]                       = *new CBasicFormula( 90. / 255. );
      _formulaMap[ Diagnostic::dtDesiredEngineSpeed ][ Diagnostic::uRevolutionsPerMinute ]   = *new CBasicFormula( 6.3 );
      _formulaMap[ Diagnostic::dtBasePulseWidth ][ Diagnostic::uSecond ]                     = *new CBasicFormula( 1. / 131.07, 2 );
      _formulaMap[ Diagnostic::dtAirFuel ][ Diagnostic::uCounts ]                            = *new CBasicFormula( 1. / 10., 1 );

      _dataMeasureUnitsMap[ Diagnostic::dtACClutch ][ Diagnostic::uBinary ]                  = Diagnostic::pNoPrefix;
      _dataMeasureUnitsMap[ Diagnostic::dtACSwitch ][ Diagnostic::uBinary ]                  = Diagnostic::pNoPrefix;
      _dataMeasureUnitsMap[ Diagnostic::dtCoolantSw ][ Diagnostic::uBinary ]                 = Diagnostic::pNoPrefix;
      _dataMeasureUnitsMap[ Diagnostic::dtHiFan ][ Diagnostic::uBinary ]                     = Diagnostic::pNoPrefix;
      _dataMeasureUnitsMap[ Diagnostic::dtLoFan ][ Diagnostic::uBinary ]                     = Diagnostic::pNoPrefix;
      _dataMeasureUnitsMap[ Diagnostic::dtFuelCutOff ][ Diagnostic::uBinary ]                = Diagnostic::pNoPrefix;
      _dataMeasureUnitsMap[ Diagnostic::dtFuelPump ][ Diagnostic::uBinary ]                  = Diagnostic::pNoPrefix;
      _dataMeasureUnitsMap[ Diagnostic::dtFieldService ][ Diagnostic::uBinary ]              = Diagnostic::pNoPrefix;
      _dataMeasureUnitsMap[ Diagnostic::dtCharPurge ][ Diagnostic::uBinary ]                 = Diagnostic::pNoPrefix;
      _dataMeasureUnitsMap[ Diagnostic::dtVGIS ][ Diagnostic::uBinary ]                      = Diagnostic::pNoPrefix;
      _dataMeasureUnitsMap[ Diagnostic::dtTPSClosed ][ Diagnostic::uBinary ]                 = Diagnostic::pNoPrefix;
      _dataMeasureUnitsMap[ Diagnostic::dtDecelCutOff ][ Diagnostic::uBinary ]               = Diagnostic::pNoPrefix;
      _dataMeasureUnitsMap[ Diagnostic::dtBlockLearn ][ Diagnostic::uBinary ]                = Diagnostic::pNoPrefix;
      _dataMeasureUnitsMap[ Diagnostic::dtClosedLoop ][ Diagnostic::uBinary ]                = Diagnostic::pNoPrefix;
      _dataMeasureUnitsMap[ Diagnostic::dtReachLean ][ Diagnostic::uBinary ]                 = Diagnostic::pNoPrefix;
      _dataMeasureUnitsMap[ Diagnostic::dtCatConvOver ][ Diagnostic::uBinary ]               = Diagnostic::pNoPrefix;
      _dataMeasureUnitsMap[ Diagnostic::dtIdleRun ][ Diagnostic::uBinary ]                   = Diagnostic::pNoPrefix;
      _dataMeasureUnitsMap[ Diagnostic::dtIntReset ][ Diagnostic::uBinary ]                  = Diagnostic::pNoPrefix;
      _dataMeasureUnitsMap[ Diagnostic::dtRpmLow ][ Diagnostic::uBinary ]                    = Diagnostic::pNoPrefix;
      _dataMeasureUnitsMap[ Diagnostic::dtIACReset ][ Diagnostic::uBinary ]                  = Diagnostic::pNoPrefix;
      _dataMeasureUnitsMap[ Diagnostic::dtIACResetReq ][ Diagnostic::uBinary ]               = Diagnostic::pNoPrefix;
      _dataMeasureUnitsMap[ Diagnostic::dtO2Ready ][ Diagnostic::uBinary ]                   = Diagnostic::pNoPrefix;
      _dataMeasureUnitsMap[ Diagnostic::dtACRun ][ Diagnostic::uBinary ]                     = Diagnostic::pNoPrefix;
      _dataMeasureUnitsMap[ Diagnostic::dtParkNeutral ][ Diagnostic::uBinary ]               = Diagnostic::pNoPrefix;
      _dataMeasureUnitsMap[ Diagnostic::dtACReq ][ Diagnostic::uBinary ]                     = Diagnostic::pNoPrefix;
      _dataMeasureUnitsMap[ Diagnostic::dtFANReq ][ Diagnostic::uBinary ]                    = Diagnostic::pNoPrefix;
   }

   CAbstractMode1Data0::~CAbstractMode1Data0(){}

   void CAbstractMode1Data0::Adjust()
   {
      TRACE_FUN( Routine, "CAbstractMode1Data0::Adjust" );

      if( hasKey( Diagnostic::dtIdleRun ) && CDataAdapter::value( Diagnostic::dtIdleRun, Diagnostic::uBinary ).toBool() )
      {
         if( hasKey( Diagnostic::dtTps ) && CDataAdapter::value( Diagnostic::dtTps, Diagnostic::uPercent ).toInt() )
         {
            CBasicFormula& formula( static_cast< CBasicFormula& >( *_formulaMap[ Diagnostic::dtTps ][ Diagnostic::uPercent ] ) );

            double base( CDataAdapter::value( Diagnostic::dtTps, Diagnostic::uCounts ).toInt() );

            formula.SetBase( -base );
            formula.SetScaling( 1 / ( 2.32 - base / 100. ) );
         }
      }
   }

   /////////////////////////////////////////////////////////////////////////////////////////////////
   CAbstractMode4::CAbstractMode4( Diagnostic::CDataAdapter& anAdapter ) : Diagnostic::CControlAdapter( anAdapter ){}
   CAbstractMode4::~CAbstractMode4(){}
}
