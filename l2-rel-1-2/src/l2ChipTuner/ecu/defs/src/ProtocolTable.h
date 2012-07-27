/*
 * ProtocolTable.h
 *
 *  Created on: 24.03.2009
 *      Author: L2-Max
 */

#ifndef PROTOCOLTABLE_H_
#define PROTOCOLTABLE_H_

#include "DescriptionTableHandle.hxx"
#include "VehicleTable.h"

#include <map>
#include <set>

namespace Protocol
{
   ////////////////////////////////////////////////////////////////////////////////////////
   enum EProtocolType
   {
      ptALDL,
      ptKWP2k,

      ptInvalid = -1
   };

   struct SProtocolDescription
   {
      EProtocolType _id;

      const char* _synonym;
      const char* _name;
      const char* _description;
   };

   typedef std::map< EProtocolType, SProtocolDescription > TProtocolDescriptionMap;

   CDescriptionTableHandle< TProtocolDescriptionMap > protocolDescriptionMap();

   namespace ALDL
   {
      ////////////////////////////////////////////////////////////////////////////////////////
      enum EECUId
      {
         eiL15sohcZXJN,
         eiL16dohcYAIH
      };

      struct SECUIdDescription
      {
         EECUId _id;

         Vehicle::EModelType _model;
         Vehicle::EEngineType _engine;

         const char* _synonym;
         const char* _name;
      };

      typedef std::map< EECUId, SECUIdDescription > TECUIdDescriptionMap;

      CDescriptionTableHandle< TECUIdDescriptionMap > ecuIdDescriptionMap();

      ////////////////////////////////////////////////////////////////////////////////////////
      typedef std::set< Vehicle::EModelType > TModelSet;
      TModelSet modelSet();

      ////////////////////////////////////////////////////////////////////////////////////////
      typedef std::set< Vehicle::EEngineType > TEngineSet;
      TEngineSet engineSet( Vehicle::EModelType aModel );

      ////////////////////////////////////////////////////////////////////////////////////////
      typedef std::set< EECUId > TEcuIdSet;
      TEcuIdSet ecuIdSet( Vehicle::EModelType aModel, Vehicle::EEngineType anEngine );
   }
}

#endif /* PROTOCOLTABLE_H_ */
