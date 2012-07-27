/*
 * VehicleTable.h
 *
 *  Created on: 25.03.2009
 *      Author: L2-Max
 */

#ifndef VEHICLETABLE_H_
#define VEHICLETABLE_H_

#include "DescriptionTableHandle.hxx"

#include <map>

namespace Vehicle
{
   ////////////////////////////////////////////////////////////////////////////////////////
   enum EEngineType
   {
      et15SOHC,
      et16DOHC
   };

   struct SEngineDescription
   {
      EEngineType _id;

      const char* _synonym;
      const char* _name;
      const char* _description;
      int _engine;
   };

   typedef std::map< EEngineType, SEngineDescription > TEngineDescriptionMap;

   CDescriptionTableHandle< TEngineDescriptionMap > engineDescriptionMap();

   ////////////////////////////////////////////////////////////////////////////////////////
   enum EModelType
   {
      mtLanos
   };

   struct SModelDescription
   {
      EModelType _id;

      const char* _synonym;
      const char* _name;
   };

   typedef std::map< EModelType, SModelDescription > TModelDescriptionMap;

   CDescriptionTableHandle< TModelDescriptionMap > modelDescriptionMap();
}

#endif /* VEHICLETABLE_H_ */
