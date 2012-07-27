/*
 * ECUAbstractUnit.h
 *
 *  Created on: 23.11.2008
 *      Author: L2-Max
 */

#ifndef ECUABSTRACTUNIT_H_
#define ECUABSTRACTUNIT_H_

#include <DiagnosticTable.h>

namespace ECU
{
   class CAbstractUnit
   {
      Diagnostic::CUnitKey _key;

   public:
      CAbstractUnit( const Diagnostic::CUnitKey& aKey );
      virtual ~CAbstractUnit();

      const Diagnostic::CUnitKey& key()const;
   };
}

#endif /* ECUABSTRACTUNIT_H_ */
