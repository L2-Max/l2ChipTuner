/*
 * l2Semaphore.h
 *
 *  Created on: 08.10.2008
 *      Author: mars
 */

#ifndef L2SEMAPHORE_H_
#define L2SEMAPHORE_H_

namespace l2max
{
   class CSemaphore
   {
      struct SSemaphorePrivateHandle;
      
      SSemaphorePrivateHandle* _semaphorePrivateHandle;
      
   public:
      CSemaphore( int aCount );
      virtual ~CSemaphore();
      
      bool Acquire( int aTimeout = -1 );
      void Release();
   };
}

#endif /* L2SEMAPHORE_H_ */
