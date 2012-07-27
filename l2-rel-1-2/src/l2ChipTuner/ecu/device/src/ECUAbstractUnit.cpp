/*
 * ECUAbstractUnit.cpp
 *
 *  Created on: 23.11.2008
 *      Author: L2-Max
 */

#include "ECUAbstractUnit.h"

#include <string>

namespace ECU
{
   CAbstractUnit::CAbstractUnit( const Diagnostic::CUnitKey& aKey ) : _key( aKey ){}

   CAbstractUnit::~CAbstractUnit(){}

   const Diagnostic::CUnitKey& CAbstractUnit::key()const
   {
      return _key;
   }
}
