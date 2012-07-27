/*
 * VehicleTable.cpp
 *
 *  Created on: 25.03.2009
 *      Author: L2-Max
 */

#include "VehicleTable.h"
#include "DescriptionTableBuilder.hxx"

namespace Vehicle
{
   ///////////////////////////////////////////////////////////////////////////////////
   namespace
   {
      SEngineDescription g_engineDescription[] =
         {
          { et15SOHC, "_15SOHC", "1.5 SOHC", "", 1500 },
          { et16DOHC, "_16DOHC", "1.6 DOHC", "", 1600 }
         };
   }

   CDescriptionTableHandle< TEngineDescriptionMap > engineDescriptionMap()
   {
      return buildDescriptionTable< TEngineDescriptionMap >( g_engineDescription, sizeof( g_engineDescription ) / sizeof( *g_engineDescription ) );
   }

   ///////////////////////////////////////////////////////////////////////////////////
   namespace
   {
      SModelDescription g_modelDescription[] =
         {
          { mtLanos, "LANOS", "Lanos" }
         };
   }

   CDescriptionTableHandle< TModelDescriptionMap > modelDescriptionMap()
   {
      return buildDescriptionTable< TModelDescriptionMap >( g_modelDescription, sizeof( g_modelDescription ) / sizeof( *g_modelDescription ) );
   }
}
