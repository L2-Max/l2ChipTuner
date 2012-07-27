/*
 * DiagnosticTable.h
 *
 *  Created on: 18.11.2008
 *      Author: L2-Max
 */

#ifndef DIAGNOSTICTABLE_H_
#define DIAGNOSTICTABLE_H_

#include "DescriptionTableHandle.hxx"

#include <map>
#include <set>
#include <vector>

namespace Diagnostic
{
   ///////////////////////////////////////////////////////////////////
   enum EDataType
   {
      dtCoolantTemp,
      dtMat,
      dtMap,
      dtTps,
      dtPresentEngineSpeed,
      dtO2,
      dtSpeed,
      dtACPresure,
      dtAirTemp,
      dtBat,
      dtCO,
      dtSparcAdvance,
      dtIACDesiredPosition,
      dtIACPresentPosition,
      dtDesiredEngineSpeed,
      dtBasePulseWidth,
      dtAirFuel,
      dtBlm,
      dtBlmCell,
      dtInt,
      dtOctane,

      dtACClutch,
      dtACSwitch,
      dtCoolantSw,
      dtHiFan,
      dtLoFan,
      dtFuelCutOff,
      dtFuelPump,
      dtFieldService,
      dtCharPurge,
      dtVGIS,
      dtTPSClosed,
      dtDecelCutOff,
      dtBlockLearn,
      dtClosedLoop,
      dtReachLean,
      dtCatConvOver,
      dtIdleRun,
      dtIntReset,
      dtRpmLow,
      dtIACReset,
      dtIACResetReq,
      dtO2Ready,
      dtACRun,
      dtIACRun,
      dtParkNeutral,
      dtACReq,
      dtFANReq,

      dtInvalid = -1
   };

   struct SDataDescription
   {
      EDataType _id;

      const char* _synonym;
      const char* _name;
      const char* _description;
   };

   typedef std::map< EDataType, SDataDescription > TDataDescriptionMap;

   CDescriptionTableHandle< TDataDescriptionMap > dataDescriptionMap();

   ///////////////////////////////////////////////////////////////////
   enum EGroupType
   {
      gtAirCondition,
      dtEngineParameters,
      dtECUParameters,
      gtOther
   };

   struct SGroupDescription
   {
      EGroupType _id;

      const char* _name;
      const char* _description;
   };

   typedef std::map< EGroupType, SGroupDescription > TGroupDescriptionMap;

   CDescriptionTableHandle< TGroupDescriptionMap > groupDescriptionMap();

   ///////////////////////////////////////////////////////////////////
   enum EUnit
   {
      uCounts,
      uVolt,
      uPercent,
      uDegree,
      uCelsius,
      uPascal,
      uKilometerPerHour,
      uRevolutionsPerMinute,
      uSecond,
      uBinary
   };

   struct SUnitDescription
   {
      EUnit _id;

      const char* _symbol;
      const char* _name;
      const char* _description;
   };

   typedef std::map< EUnit, SUnitDescription > TUnitDescriptionMap;

   CDescriptionTableHandle< TUnitDescriptionMap > unitDescriptionMap();

   ///////////////////////////////////////////////////////////////////
   enum EPrefix
   {
      pNoPrefix,
      pMilli,
      pKilo
   };

   struct SPrefixDescription
   {
      EPrefix _id;

      const char* _symbol;
      const char* _name;
   };

   typedef std::pair< EUnit, EPrefix > TMeasureUnitPair;
   typedef std::map< EUnit, EPrefix > TMeasureUnitsMap;
   typedef std::map< EPrefix, SPrefixDescription > TPrefixDescriptionMap;

   CDescriptionTableHandle< TPrefixDescriptionMap > prefixDescriptionMap();

   ///////////////////////////////////////////////////////////////////
   enum EErrorType
   {
      etMalf0,
      etMalf1,
      etMalf2,
      etMalf3,
      etMalf4,
      etMalf5,
      etMalf6,
      etMalf7,
      etMalf8,
      etMalf9,
      etMalf10,
      etMalf11,
      etMalf12,
      etMalf13,
      etMalf14,
      etMalf15,
      etMalf16,
      etMalf17,
      etMalf18,
      etMalf19,
      etMalf20,
      etMalf21,
      etMalf22,
      etMalf23,
      etMalf24,
      etMalf25,
      etMalf26,
      etMalf27
   };

   struct SErrorDescription
   {
      EErrorType _id;

      const char* _description;
   };

   typedef std::set< EErrorType > TErrorTypes;
   typedef std::map< EErrorType, SErrorDescription > TErrorDescriptionMap;

   CDescriptionTableHandle< TErrorDescriptionMap > errorDescriptionMap();

   ///////////////////////////////////////////////////////////////////
   enum EControlType
   {
      ctCheckEngine,
      ctFuelPump,
      ctEGR,
      ctFan,
      ctAC,
      ctCharPurgePulseWidth,
      ctResetBLM,
      ctOpenLoop,
      ctKnock,
      ctResetIAC,
      ctCO,
      ctSA,
      ctRPM,
      ctIAC,
      ctAirFuel,
      ctSparcAdvance,
      ctInj1,
      ctInj2,
      ctInj3,
      ctInj4
   };

   struct SControlDescription
   {
      EControlType _id;

      const char* _name;
      const char* _description;
   };

   typedef std::map< EControlType, SControlDescription > TControlDescriptionMap;

   CDescriptionTableHandle< TControlDescriptionMap > controlDescriptionMap();

   ///////////////////////////////////////////////////////////////////
   class CUnitKey
   {
      EDataType _dataType;

   public:
      CUnitKey();
      CUnitKey( EDataType aType );

      EDataType type()const;

      bool isValid()const;

      bool operator < ( const CUnitKey& aKey )const;
      bool operator == ( const CUnitKey& aKey )const;
      bool operator != ( const CUnitKey& aKey )const;
   };

   typedef std::set< CUnitKey > TUnitKeys;

   ///////////////////////////////////////////////////////////////////
   typedef std::map< EGroupType, TUnitKeys > TGroup2UnitKeyMap;
   typedef std::map< CUnitKey, EGroupType > TUnitKey2GroupMap;

   CDescriptionTableHandle< TGroup2UnitKeyMap > group2UnitKeyMap();
   CDescriptionTableHandle< TUnitKey2GroupMap > unitKey2GroupMap();
}

#endif /* DIAGNOSTICTABLE_H_ */
