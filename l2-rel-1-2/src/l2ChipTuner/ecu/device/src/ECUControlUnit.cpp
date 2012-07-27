/*
 * ECUControlUnit.cpp
 *
 *  Created on: 03.02.2009
 *      Author: mars
 */

#include "ECUControlUnit.h"

#include <DiagnosticControlAdapter.h>

#include <l2ExceptionThrower.h>

namespace ECU
{
   CControlUnit::CControlUnit( Diagnostic::EDataType aType,
                               Diagnostic::HControlAdapter anAdapter ) :
      CAbstractUnit( aType ),
      _controlAdapter( anAdapter ){}

   CControlUnit::~CControlUnit(){}

   void CControlUnit::SetValue( const l2max::CVariant& aValue, Diagnostic::EUnit anUnit )
   {
      _controlAdapter->SetValue( aValue, key().type(), anUnit );
   }
}
