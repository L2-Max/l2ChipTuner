/*
 * l2BaseCategorySerializer.h
 *
 *  Created on: 31.03.2009
 *      Author: L2-Max
 */

#ifndef L2BASECATEGORYSERIALIZER_H_
#define L2BASECATEGORYSERIALIZER_H_

#include "l2BaseSerializer.h"
#include "l2CategoryParserDriver.h"

namespace l2max
{
   namespace Registry
   {
      namespace Driver
      {
         class CBaseCategorySerializer : public CBaseSerializer
         {
         public:
            CBaseCategorySerializer( const SCategoryMembers& aMembers, int anIndentation );
            virtual ~CBaseCategorySerializer();
         };
      }
   }
}

#endif /* L2BASECATEGORYSERIALIZER_H_ */
