/*
 * l2TextTreeRegistry.cpp
 *
 *  Created on: 07.01.2009
 *      Author: L2-Max
 */

#include "l2TextTreeRegistry.h"

namespace l2max
{
   CTextTreeRegistry::CTextTreeRegistry(){}
   CTextTreeRegistry::~CTextTreeRegistry()throw(){}

   bool CTextTreeRegistry::Open( const std::string& aFile )
   {
      return CRegistry::Open( aFile, Registry::Driver::tTextTree );
   }
}
