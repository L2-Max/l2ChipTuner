/*
 * ECUErrorsTable.cpp
 *
 *  Created on: 07.12.2008
 *      Author: L2-Max
 */

#include "ECUErrorsTable.h"

#include <DiagnosticErrorAdapter.h>

#include <l2ExceptionThrower.h>

namespace ECU
{
   CErrorsTable::CErrorsTable()
   {
   }

   CErrorsTable::~CErrorsTable()
   {
   }

   void CErrorsTable::BuildErrorsTable( Diagnostic::HErrorAdapter anAdapter )
   {
      CHECK_OBJECT_STATE( !anAdapter.IsNull(), "Adapter is null" );

      _errors = anAdapter->errors();
   }

   Diagnostic::TErrorTypes CErrorsTable::errors()const
   {
      return _errors;
   }

   int CErrorsTable::count()const
   {
      return _errors.size();
   }
}
