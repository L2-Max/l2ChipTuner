/*
 * ProtocolTable.cpp
 *
 *  Created on: 24.03.2009
 *      Author: L2-Max
 */

#include "ProtocolTable.h"
#include "DescriptionTableBuilder.hxx"

namespace Protocol
{
   ///////////////////////////////////////////////////////////////////////////////////
   namespace
   {
      SProtocolDescription g_protocolDescription[] =
         {
          { ptALDL, "ALDL", "ALDL" },
          { ptKWP2k, "KWP2k", "Keyword Protocol 2000" }
         };
   }

   CDescriptionTableHandle< TProtocolDescriptionMap > protocolDescriptionMap()
   {
      return buildDescriptionTable< TProtocolDescriptionMap >( g_protocolDescription, sizeof( g_protocolDescription ) / sizeof( *g_protocolDescription ) );
   }

   namespace ALDL
   {
      ///////////////////////////////////////////////////////////////////////////////////
      namespace
      {
         SECUIdDescription g_ecuIdDescription[] =
            {
             { eiL15sohcZXJN, Vehicle::mtLanos, Vehicle::et15SOHC, "ZXJN", "ZXJN" },
             { eiL16dohcYAIH, Vehicle::mtLanos, Vehicle::et16DOHC, "YAIH", "YAIH" }
            };
      }

      CDescriptionTableHandle< TECUIdDescriptionMap > ecuIdDescriptionMap()
      {
         return buildDescriptionTable< TECUIdDescriptionMap >( g_ecuIdDescription,
                                                               sizeof( g_ecuIdDescription ) / sizeof( *g_ecuIdDescription ) );
      }

      ///////////////////////////////////////////////////////////////////////////////////
      TModelSet modelSet()
      {
         TModelSet ret;

         CDescriptionTableHandle< TECUIdDescriptionMap > ecuDescription( ecuIdDescriptionMap() );

         for( TECUIdDescriptionMap::const_iterator i( ecuDescription->begin() ); i != ecuDescription->end(); ++i )
         {
            ret.insert( i->second._model );
         }

         return ret;
      }

      ///////////////////////////////////////////////////////////////////////////////////
      TEngineSet engineSet( Vehicle::EModelType aModel )
      {
         TEngineSet ret;

         CDescriptionTableHandle< TECUIdDescriptionMap > ecuDescription( ecuIdDescriptionMap() );

         for( TECUIdDescriptionMap::const_iterator i( ecuDescription->begin() ); i != ecuDescription->end(); ++i )
         {
            if( i->second._model == aModel )
            {
               ret.insert( i->second._engine );
            }
         }

         return ret;
      }

      ///////////////////////////////////////////////////////////////////////////////////
      TEcuIdSet ecuIdSet( Vehicle::EModelType aModel, Vehicle::EEngineType anEngine )
      {
         TEcuIdSet ret;

         CDescriptionTableHandle< TECUIdDescriptionMap > ecuDescription( ecuIdDescriptionMap() );

         for( TECUIdDescriptionMap::const_iterator i( ecuDescription->begin() ); i != ecuDescription->end(); ++i )
         {
            if( i->second._model == aModel )
            {
               if( i->second._engine == anEngine )
               {
                  ret.insert( i->second._id );
               }
            }
         }

         return ret;
      }
   }
}
