/*
 * l2TextIniCategorySerializer.h
 *
 *  Created on: 31.03.2009
 *      Author: L2-Max
 */

#ifndef L2TEXTINICATEGORYSERIALIZER_H_
#define L2TEXTINICATEGORYSERIALIZER_H_

#include "l2BaseCategorySerializer.h"

namespace l2max
{
   namespace Registry
   {
      namespace Driver
      {
         class CTextIniCategorySerializer : public CBaseCategorySerializer
         {
         public:
            CTextIniCategorySerializer( const SCategoryMembers& aMembers, int anIndentation );
            virtual ~CTextIniCategorySerializer();
         };
      }
   }
}

#endif /* L2TEXTINICATEGORYSERIALIZER_H_ */
