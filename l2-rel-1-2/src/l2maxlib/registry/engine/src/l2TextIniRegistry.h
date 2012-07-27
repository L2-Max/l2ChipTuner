/*
 * l2TextIniRegistry.h
 *
 *  Created on: 07.01.2009
 *      Author: L2-Max
 */

#ifndef L2TEXTINIREGISTRY_H_
#define L2TEXTINIREGISTRY_H_

#include "l2Registry.h"

namespace l2max
{
   class CTextIniRegistry : public CRegistry
   {
   public:
      CTextIniRegistry();
      virtual ~CTextIniRegistry()throw();

      bool Open( const std::string& aFile );
   };
}

#endif /* L2TEXTINIREGISTRY_H_ */
