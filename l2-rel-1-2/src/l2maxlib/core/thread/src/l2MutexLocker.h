/*
 * l2MutexLocker.h
 *
 *  Created on: 08.10.2008
 *      Author: mars
 */

#ifndef L2MUTEXLOCKER_H_
#define L2MUTEXLOCKER_H_

namespace l2max
{
   class CMutex;

   class CMutexLocker
   {
      CMutex& _mutex;

      CMutexLocker( const CMutexLocker& );
      CMutexLocker& operator = ( const CMutexLocker& );

   public:
      CMutexLocker( CMutex& aMutex );
      virtual ~CMutexLocker();
   };

}

#endif /* L2MUTEXLOCKER_H_ */
