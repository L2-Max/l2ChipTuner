/*
 * l2BaseCategorySerializer.cpp
 *
 *  Created on: 31.03.2009
 *      Author: L2-Max
 */

#include "l2BaseCategorySerializer.h"
#include "l2VariableSerializer.h"

namespace l2max
{
   namespace Registry
   {
      namespace Driver
      {
         CBaseCategorySerializer::CBaseCategorySerializer( const SCategoryMembers& aMembers, int anIndentation ) : CBaseSerializer( anIndentation )
         {
            _serialized += CVariablesSerializer( aMembers._variables, anIndentation ).serialized();
         }

         CBaseCategorySerializer::~CBaseCategorySerializer(){}
      }
   }
}
