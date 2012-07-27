/*
 * l2TextTreeCategorySerializer.cpp
 *
 *  Created on: 31.03.2009
 *      Author: L2-Max
 */

#include "l2TextTreeCategorySerializer.h"

namespace l2max
{
   namespace Registry
   {
      namespace Driver
      {
         CTextTreeCategorySerializer::CTextTreeCategorySerializer( const SCategoryMembers& aMembers, int anIndentation ) :
            CBaseCategorySerializer( aMembers, anIndentation )
         {
            for( TCategorySeq::const_iterator i( aMembers._categories.begin() ); i != aMembers._categories.end(); ++i )
            {
               if( aMembers._variables.size() || i != aMembers._categories.begin() )
               {
                  _serialized += CBaseSerializer::cr();
               }

               _serialized += _indentation;

               _serialized += i->first + CBaseSerializer::cr();
               _serialized += _indentation + "{" + CBaseSerializer::cr();

               _serialized += CTextTreeCategorySerializer( i->second.second->members(), anIndentation + 1 ).serialized();

               _serialized += _indentation + "}" + CBaseSerializer::cr();
            }
         }

         CTextTreeCategorySerializer::~CTextTreeCategorySerializer(){}
      }
   }
}
