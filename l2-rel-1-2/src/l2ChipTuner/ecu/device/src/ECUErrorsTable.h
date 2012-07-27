/*
 * ECUErrorsTable.h
 *
 *  Created on: 07.12.2008
 *      Author: L2-Max
 */

#ifndef ECUERRORSTABLE_H_
#define ECUERRORSTABLE_H_

#include <DiagnosticTable.h>
#include <DiagnosticAdapterHandles.h>

namespace Diagnostic
{
   class CErrorAdapter;
}

namespace ECU
{
   class CErrorsTable
   {
      Diagnostic::TErrorTypes _errors;

   public:
      CErrorsTable();
      virtual ~CErrorsTable();

      void BuildErrorsTable( Diagnostic::HErrorAdapter anAdapter );

      Diagnostic::TErrorTypes errors()const;
      int count()const;
   };
}

#endif /* ECUERRORSTABLE_H_ */
