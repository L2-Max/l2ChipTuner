/*
 * ECUControlUnit.h
 *
 *  Created on: 03.02.2009
 *      Author: mars
 */

#ifndef ECUCONTROLUNIT_H_
#define ECUCONTROLUNIT_H_

#include "ECUAbstractUnit.h"

#include <DiagnosticAdapterHandles.h>

namespace l2max
{
   class CVariant;
}

namespace ECU
{
   class CControlUnit : public CAbstractUnit
   {
      Diagnostic::HControlAdapter _controlAdapter;

   public:
      CControlUnit( Diagnostic::EDataType aType,
                    Diagnostic::HControlAdapter anAdapter );

      virtual ~CControlUnit();

      void SetValue( const l2max::CVariant& aValue, Diagnostic::EUnit anUnit );
   };
}

#endif /* ECUCONTROLUNIT_H_ */
