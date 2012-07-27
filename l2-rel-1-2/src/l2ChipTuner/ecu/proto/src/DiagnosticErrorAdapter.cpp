/*
 * DiagnosticErrorAdapter.cpp
 *
 *  Created on: 07.12.2008
 *      Author: L2-Max
 */

#include "DiagnosticErrorAdapter.h"

#include <l2MutexLocker.h>

namespace Diagnostic
{
   CErrorAdapter::CErrorAdapter(){}

   CErrorAdapter::~CErrorAdapter(){}

   TErrorTypes CErrorAdapter::errors()const
   {
      l2max::CMutexLocker locker( _errorMutex );

      return _errorTypes;
   }
}
