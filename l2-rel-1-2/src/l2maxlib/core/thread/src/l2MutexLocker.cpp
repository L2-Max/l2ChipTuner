/*
 * l2MutexLocker.cpp
 *
 *  Created on: 08.10.2008
 *      Author: mars
 */

#include "l2MutexLocker.h"
#include "l2Mutex.h"

namespace l2max
{
   CMutexLocker::CMutexLocker( CMutex& aMutex ) : _mutex( aMutex )
   {
      _mutex.Lock();
   }

   CMutexLocker::~CMutexLocker()
   {
      _mutex.UnLock();
   }
}
