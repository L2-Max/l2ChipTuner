/*
 * DiagnosticErrorAdapter.h
 *
 *  Created on: 07.12.2008
 *      Author: L2-Max
 */

#ifndef DIAGNOSTICERRORADAPTER_H_
#define DIAGNOSTICERRORADAPTER_H_

#include "DiagnosticTable.h"

#include <l2Mutex.h>
#include <l2RefObject.h>

#include <set>

namespace Diagnostic
{
   class CErrorAdapter : virtual public l2max::CRefObject
   {
      CErrorAdapter( const CErrorAdapter& );
      CErrorAdapter& operator = ( const CErrorAdapter& );

   protected:
      mutable l2max::CMutex _errorMutex;

      TErrorTypes _errorTypes;

   public:
      CErrorAdapter();
      virtual ~CErrorAdapter();

      TErrorTypes errors()const;
   };
}

#endif /* DIAGNOSTICERRORADAPTER_H_ */
