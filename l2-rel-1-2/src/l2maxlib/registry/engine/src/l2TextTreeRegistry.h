/*
 * l2TextTreeRegistry.h
 *
 *  Created on: 07.01.2009
 *      Author: L2-Max
 */

#ifndef L2TEXTTREEREGISTRY_H_
#define L2TEXTTREEREGISTRY_H_

#include "l2Registry.h"

namespace l2max
{
   class CTextTreeRegistry : public CRegistry
   {
   public:
      CTextTreeRegistry();
      virtual ~CTextTreeRegistry()throw();

      bool Open( const std::string& aFile );
   };
}

#endif /* L2TEXTTREEREGISTRY_H_ */
