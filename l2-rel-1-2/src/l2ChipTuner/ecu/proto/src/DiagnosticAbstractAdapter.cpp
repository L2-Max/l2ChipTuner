/*
 * DiagnosticAbstractAdapter.cpp
 *
 *  Created on: 04.12.2008
 *      Author: mars
 */

#include "DiagnosticAbstractAdapter.h"

namespace Diagnostic
{
   CAbstractAdapter::CAbstractAdapter()
   {
   }

   CAbstractAdapter::~CAbstractAdapter()
   {
   }
   
   bool CAbstractAdapter::hasKey( const CUnitKey& aKey )const
   {
      TUnitKeys theKeys( keys() );
      
      return theKeys.find( aKey ) != theKeys.end();
   }
}
