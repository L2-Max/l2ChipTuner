/*
 * DiagnosticTable.cpp
 *
 *  Created on: 18.11.2008
 *      Author: L2-Max
 */

#include "DiagnosticTable.h"
#include "DescriptionTableBuilder.hxx"

namespace Diagnostic
{
   ///////////////////////////////////////////////////////////////////////////////////
   namespace
   {
      SDataDescription g_dataDescriptions[] =
         {
          { dtCoolantTemp, "COOLDEG", "Coolant Temperature", "Raw linearized coolant temperature" },
          { dtAirTemp, "AIRDEG", "Air temperature", "Raw linearized outside air temperature" },
          { dtMap, "ADMAP", "MAP", "A/D Result from manifold pressure sensor input" },
          { dtTps, "ADTHROT", "TPS", "Out put from Trottle Position Sensor" },
          { dtPresentEngineSpeed, "NTRPMX", "RPM", "Present Engine speed" },
          { dtO2, "ADO2A", "O2", "Out put from Oxygen Sensor" },
          { dtSpeed, "FILTMPH", "KM/H", "Vehicle Speed" },
          { dtACPresure, "ACPRES", "A/C Pressure", "A/C Pressure Transducer A/D counts" },
          { dtMat, "MATDEG", "MAT", "Manifold Air Temperature" },
          { dtBat, "ADBAT", "BAT", "Batery Voltage" },
          { dtCO, "FILDIMPT", "CO", "CO Potentiometer" },
          { dtSparcAdvance, "SAP", "Sparc Advance", "Sparc Advance relative to Top Dead Center" },
          { dtIACDesiredPosition, "ISDSMP", "IAC Desired", "Idle Air Control desired motor position" },
          { dtIACPresentPosition, "ISSPMP", "IAC Present", "Idle Air Control Present motor position" },
          { dtDesiredEngineSpeed, "DESSPD", "Desired Idle Rpm", "Desired Idle Engine speed" },
          { dtBasePulseWidth, "DISPBPW", "Base Pulse Width", "Injection Base Pulse Width" },
          { dtAirFuel, "AIRFUEL", "Air/Fuel", "Desired Air/Fuel Ratio" },
          { dtBlm, "BLM", "BLM", "Block Learn Multiplier" },
          { dtBlmCell, "BLMCELL", "BLMCELL", "Block Learn Multiplier" },
          { dtInt, "INT", "INT", "Closed Loop Integrator Value" },
          { dtOctane, "OCTANE", "Octane", "Octane corrector" },

          { dtACClutch, "ACCLUTCH", "A/C Clutch", "A/C clutch is on"},
          { dtACSwitch, "ACSWITCH", "A/C Switch", "A/C switch is on" },
          { dtCoolantSw, "COOLSW", "Coolant Switch", "Coolant temperature switch" },
          { dtHiFan, "HIGHFAN", "High Fan", "Hign fan is on" },
          { dtLoFan, "LOWFAN", "Low Fan", "Low fan is on" },
          { dtFuelCutOff, "FUELCUTOFF", "Safety Cut Off", "Safety fuel cut off is active" },
          { dtFuelPump, "FUELPUMP", "Fuel Pump", "Fuel pump enabled" },
          { dtFieldService, "FLDSRVC", "Field Service", "Field service transition" },
          { dtCharPurge, "CHARPURGE", "Purge Clutch", "Charcoal canister purge duty cycle" },
          { dtVGIS, "VGIS", "VGIS", "Variable Geometry Induction System" },
          { dtTPSClosed, "TPSCLOSED", "TPS Closed", "Trottle Position Sensor is closed" },
          { dtDecelCutOff, "DECELCUTOFF", "Decel Cut-Off", "Deceleration fuel cut-off" },
          { dtBlockLearn, "BLOCKLEARN", "Block Learn", "Block learn enabled" },
          { dtClosedLoop, "CLOSEDLOOP", "Closed Loop", "Closed loop" },
          { dtReachLean, "REACHLEAN", "Reach/Lean", "Reach = 'Yes' / lean = 'No' mixture" },
          { dtCatConvOver, "CATCONVOVER", "Cat. Conv. Over", "Catalytic converter overtemp" },
          { dtIdleRun, "IDLERUN", "Idle Run", "Idle run" },
          { dtIntReset, "INTRESET", "INT Reset", "Reset fuel integrator" },
          { dtRpmLow, "RPMLOW", "RPM Low", "RPM is low" },
          { dtIACReset, "IACRESET", "IAC Reset", "IAC reset" },
          { dtIACResetReq, "IACRESETREQ", "IAC Reset Req.", "IAC reset request" },
          { dtO2Ready, "O2READY", "O2 Ready", "O2 ready" },
          { dtACRun, "ACRUN", "A/C Run", "A/C is run" },
          { dtIACRun, "IACRUN", "IAC Run", "IAC is run" },
          { dtParkNeutral, "PARKNEUTRAL", "Park/Neutral", "Park/Neutral" },
          { dtACReq, "ACREQ", "A/C Request", "A/C request" },
          { dtFANReq, "FANREQ", "FAN Request", "FAN request" },
         };
   }

   CDescriptionTableHandle< TDataDescriptionMap > dataDescriptionMap()
   {
      return buildDescriptionTable< TDataDescriptionMap >( g_dataDescriptions, sizeof( g_dataDescriptions ) / sizeof( *g_dataDescriptions ) );
   }

   ///////////////////////////////////////////////////////////////////////////////////
   namespace
   {
      SGroupDescription g_groupDescription[] =
         {
          { gtAirCondition, "Air Condition", "" },
          { dtEngineParameters, "Engine", "" },
          { dtECUParameters, "ECU", "" },
          { gtOther, "Other", "" }
         };
   }

   CDescriptionTableHandle< TGroupDescriptionMap > groupDescriptionMap()
   {
      return buildDescriptionTable< TGroupDescriptionMap >( g_groupDescription, sizeof( g_groupDescription ) / sizeof( *g_groupDescription ) );
   }

   ///////////////////////////////////////////////////////////////////////////////////
   namespace
   {
      SUnitDescription g_unitDescription[] =
         {
          { uCounts, "", "counts", "counts" },
          { uVolt, "V", "volt", "voltage" },
          { uPercent, "%", "percent", "percent" },
          { uDegree, "˚", "degree", "plane degree" },
          { uCelsius, "˚C", "celsius", "thermodynamic temperature" },
          { uPascal, "Pa", "pascal", "pressure" },
          { uKilometerPerHour, "km/h", "kilometer per hour", "speed" },
          { uRevolutionsPerMinute, "rpm", "revolution per hour", "angular velocity" },
          { uSecond, "s", "second", "time" },
          { uBinary, "", "true / false", "state" }
         };
   }

   CDescriptionTableHandle< TUnitDescriptionMap > unitDescriptionMap()
   {
      return buildDescriptionTable< TUnitDescriptionMap >( g_unitDescription, sizeof( g_unitDescription ) / sizeof( *g_unitDescription ) );
   }

   ///////////////////////////////////////////////////////////////////////////////////
   namespace
   {
      SPrefixDescription g_prefixDescription[] =
         {
          { pNoPrefix, "", "no prefix" },
          { pMilli, "m", "milli" },
          { pKilo, "k", "kilo" }
         };
   }

   CDescriptionTableHandle< TPrefixDescriptionMap > prefixDescriptionMap()
   {
      return buildDescriptionTable< TPrefixDescriptionMap >( g_prefixDescription, sizeof( g_prefixDescription ) / sizeof( *g_prefixDescription ) );
   }

   ///////////////////////////////////////////////////////////////////////////////////
   namespace
   {
      SErrorDescription g_errorDescription[] =
         {
          { etMalf0, "Electronic Spark Timing. Firing point control" },
          { etMalf1, "High manifold pressure" },
          { etMalf2, "Exhaust Gas Recirculation Circuit" },
          { etMalf3, "No signal from speed sensor" },
          { etMalf4, "High manifold air temperature" },
          { etMalf5, "High signal from Trotle Position Sensor" },
          { etMalf6, "Low collant temperature" },
          { etMalf7, "Oxygen sensor circuit" },
          { etMalf8, "Electronic Spark Timing A shorted at BAT+" },
          { etMalf9, "Electronic Spark Timing A shorted at ground" },
          { etMalf10, "Immobilizer no connection" },
          { etMalf11, "Octane corrector" },
          { etMalf12, "Knock sensor" },
          { etMalf13, "ECU Error" },
          { etMalf14, "O2 Sensor reach" },
          { etMalf15, "O2 Sensor lean" },
          { etMalf16, "Low signal from Trotle Position Sensor" },
          { etMalf17, "High collant temperature" },
          { etMalf18, "Idle Air Control" },
          { etMalf19, "Crank position sensor" },
          { etMalf20, "ECU Error" },
          { etMalf21, "Injector" },
          { etMalf22, "Electronic Spark Timing A shorted at BAT+" },
          { etMalf23, "Electronic Spark Timing A shorted at ground" },
          { etMalf24, "A/C pressure sensor" },
          { etMalf25, "A/C pressure sensor" },
          { etMalf26, "Low manifold pressure" },
          { etMalf27, "Low manifold air temperature" }
         };
   }

   CDescriptionTableHandle< TErrorDescriptionMap > errorDescriptionMap()
   {
      return buildDescriptionTable< TErrorDescriptionMap >( g_errorDescription, sizeof( g_errorDescription ) / sizeof( *g_errorDescription ) );
   }

   ///////////////////////////////////////////////////////////////////////////////////
   namespace
   {
      SControlDescription g_controlDescription[] =
         {
          { ctCheckEngine, "Check Engine", "" },
          { ctFuelPump, "Fuel Pump", "" },
          { ctEGR, "Exhaus Gas Recirculation", "" },
          { ctFan, "Fan", "" },
          { ctAC, "A/C", "" },
          { ctCharPurgePulseWidth, "Charcoal Pulse Width", "" },
          { ctResetBLM, "Reset BLM", "" },
          { ctOpenLoop, "Open Loop", "" },
          { ctKnock, "Knock Sensor", "" },
          { ctResetIAC, "Reset IAC", "" },
          { ctCO, "CO", "" },
          { ctSA, "SA", "" },
          { ctRPM, "RPM", "" },
          { ctIAC, "IAC", "" },
          { ctAirFuel, "Air Fuel", "" },
          { ctSparcAdvance, "Spark Advance", "" },
          { ctInj1, "Injector 1", "" },
          { ctInj2, "Injector 2", "" },
          { ctInj3, "Injector 3", "" },
          { ctInj4, "Injector 4", "" }
         };
   }

   CDescriptionTableHandle< TControlDescriptionMap > controlDescriptionMap()
   {
      return buildDescriptionTable< TControlDescriptionMap >( g_controlDescription,
                                                              sizeof( g_controlDescription ) / sizeof( *g_controlDescription ) );
   }

   ///////////////////////////////////////////////////////////////////////////////////
   CUnitKey::CUnitKey()
   {
      _dataType = dtInvalid;
   }

   CUnitKey::CUnitKey( EDataType aType )
   {
      _dataType = aType;
   }

   EDataType CUnitKey::type()const
   {
      return _dataType;
   }

   bool CUnitKey::isValid()const
   {
      return _dataType != dtInvalid;
   }

   bool CUnitKey::operator < ( const CUnitKey& aKey )const
   {
      return _dataType < aKey._dataType;
   }

   bool CUnitKey::operator == ( const CUnitKey& aKey )const
   {
      return _dataType == aKey._dataType;
   }

   bool CUnitKey::operator != ( const CUnitKey& aKey )const
   {
      return !operator == ( aKey );
   }

   ///////////////////////////////////////////////////////////////////////////////////
   namespace
   {
      TGroup2UnitKeyMap g_group2UnitKeyMap;
      TUnitKey2GroupMap g_unitKey2GroupMap;
   }

   CDescriptionTableHandle< TGroup2UnitKeyMap > group2UnitKeyMap()
   {
      if( !g_group2UnitKeyMap.size() )
      {
         g_group2UnitKeyMap[ gtAirCondition ].insert( dtACPresure );
         g_group2UnitKeyMap[ gtAirCondition ].insert( dtACClutch );
         g_group2UnitKeyMap[ gtAirCondition ].insert( dtACSwitch );
         g_group2UnitKeyMap[ gtAirCondition ].insert( dtACRun );
         g_group2UnitKeyMap[ gtAirCondition ].insert( dtACReq );

         g_group2UnitKeyMap[ dtEngineParameters ].insert( dtCoolantTemp );
         g_group2UnitKeyMap[ dtEngineParameters ].insert( dtAirTemp );
         g_group2UnitKeyMap[ dtEngineParameters ].insert( dtMap );
         g_group2UnitKeyMap[ dtEngineParameters ].insert( dtTps );
         g_group2UnitKeyMap[ dtEngineParameters ].insert( dtPresentEngineSpeed );
         g_group2UnitKeyMap[ dtEngineParameters ].insert( dtO2 );
         g_group2UnitKeyMap[ dtEngineParameters ].insert( dtMat );
         g_group2UnitKeyMap[ dtEngineParameters ].insert( dtSparcAdvance );
         g_group2UnitKeyMap[ dtEngineParameters ].insert( dtIACPresentPosition );
         g_group2UnitKeyMap[ dtEngineParameters ].insert( dtBasePulseWidth );
         g_group2UnitKeyMap[ dtEngineParameters ].insert( dtAirFuel );
         g_group2UnitKeyMap[ dtEngineParameters ].insert( dtCoolantSw );
         g_group2UnitKeyMap[ dtEngineParameters ].insert( dtHiFan );
         g_group2UnitKeyMap[ dtEngineParameters ].insert( dtLoFan );
         g_group2UnitKeyMap[ dtEngineParameters ].insert( dtFuelCutOff );
         g_group2UnitKeyMap[ dtEngineParameters ].insert( dtFuelPump );
         g_group2UnitKeyMap[ dtEngineParameters ].insert( dtCharPurge );
         g_group2UnitKeyMap[ dtEngineParameters ].insert( dtVGIS );
         g_group2UnitKeyMap[ dtEngineParameters ].insert( dtTPSClosed );
         g_group2UnitKeyMap[ dtEngineParameters ].insert( dtDecelCutOff );
         g_group2UnitKeyMap[ dtEngineParameters ].insert( dtReachLean );
         g_group2UnitKeyMap[ dtEngineParameters ].insert( dtIdleRun );
         g_group2UnitKeyMap[ dtEngineParameters ].insert( dtIACRun );
         g_group2UnitKeyMap[ dtEngineParameters ].insert( dtFANReq );
         g_group2UnitKeyMap[ dtEngineParameters ].insert( dtCatConvOver );

         g_group2UnitKeyMap[ dtECUParameters ].insert( dtBlm );
         g_group2UnitKeyMap[ dtECUParameters ].insert( dtBlmCell );
         g_group2UnitKeyMap[ dtECUParameters ].insert( dtInt );
         g_group2UnitKeyMap[ dtECUParameters ].insert( dtFieldService );
         g_group2UnitKeyMap[ dtECUParameters ].insert( dtBlockLearn );
         g_group2UnitKeyMap[ dtECUParameters ].insert( dtClosedLoop );
         g_group2UnitKeyMap[ dtECUParameters ].insert( dtIACDesiredPosition );
         g_group2UnitKeyMap[ dtECUParameters ].insert( dtDesiredEngineSpeed );
         g_group2UnitKeyMap[ dtECUParameters ].insert( dtIntReset );
         g_group2UnitKeyMap[ dtECUParameters ].insert( dtRpmLow );
         g_group2UnitKeyMap[ dtECUParameters ].insert( dtIACReset );
         g_group2UnitKeyMap[ dtECUParameters ].insert( dtIACResetReq );
         g_group2UnitKeyMap[ dtECUParameters ].insert( dtO2Ready );

         g_group2UnitKeyMap[ gtOther ].insert( dtSpeed );
         g_group2UnitKeyMap[ gtOther ].insert( dtBat );
         g_group2UnitKeyMap[ gtOther ].insert( dtCO );
         g_group2UnitKeyMap[ gtOther ].insert( dtOctane );
         g_group2UnitKeyMap[ gtOther ].insert( dtParkNeutral );
      }

      return g_group2UnitKeyMap;
   }

   CDescriptionTableHandle< TUnitKey2GroupMap > unitKey2GroupMap()
   {
      if( !g_unitKey2GroupMap.size() )
      {
         for( TGroup2UnitKeyMap::const_iterator i( group2UnitKeyMap()->begin() ); i != group2UnitKeyMap()->end(); ++i )
         {
            for( TUnitKeys::const_iterator j( i->second.begin() ); j != i->second.end(); ++j )
            {
               g_unitKey2GroupMap[ *j ] = i->first;
            }
         }
      }

      return g_unitKey2GroupMap;
   }
}
