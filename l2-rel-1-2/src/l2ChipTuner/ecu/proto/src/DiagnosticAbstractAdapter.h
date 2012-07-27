/*
 * DiagnosticAbstractAdapter.h
 *
 *  Created on: 04.12.2008
 *      Author: mars
 */

#ifndef DIAGNOSTICABSTRACTADAPTER_H_
#define DIAGNOSTICABSTRACTADAPTER_H_

#include "DiagnosticTable.h"

#include <l2RefObject.h>

namespace Diagnostic
{
   class CAbstractAdapter : virtual public l2max::CRefObject
   {
   public:
      CAbstractAdapter();
      virtual ~CAbstractAdapter();

      virtual TUnitKeys keys()const = 0;

      bool hasKey( const CUnitKey& aKey )const;
   };
}

#endif /* DIAGNOSTICABSTRACTADAPTER_H_ */
