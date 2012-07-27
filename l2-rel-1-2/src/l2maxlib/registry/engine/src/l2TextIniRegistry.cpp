/*
 * l2TextIniRegistry.cpp
 *
 *  Created on: 07.01.2009
 *      Author: L2-Max
 */

#include "l2TextIniRegistry.h"

namespace l2max
{
   CTextIniRegistry::CTextIniRegistry(){}
   CTextIniRegistry::~CTextIniRegistry()throw(){}

   bool CTextIniRegistry::Open( const std::string& aFile )
   {
      return CRegistry::Open( aFile, Registry::Driver::tTextIni );
   }
}
