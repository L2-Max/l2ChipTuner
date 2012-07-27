/*
 * l2TextTreeCategorySerializer.h
 *
 *  Created on: 31.03.2009
 *      Author: L2-Max
 */

#ifndef L2TEXTTREECATEGORYSERIALIZER_H_
#define L2TEXTTREECATEGORYSERIALIZER_H_

#include "l2BaseCategorySerializer.h"

namespace l2max
{
   namespace Registry
   {
      namespace Driver
      {
         class CTextTreeCategorySerializer : public CBaseCategorySerializer
         {
         public:
            CTextTreeCategorySerializer( const SCategoryMembers& aMembers, int anIndentation );
            virtual ~CTextTreeCategorySerializer();
         };
      }
   }
}

#endif /* L2TEXTTREECATEGORYSERIALIZER_H_ */
