/*
 * ECURequestQueue.h
 *
 *  Created on: 25.01.2009
 *      Author: mars
 */

#ifndef ECUREQUESTQUEUE_H_
#define ECUREQUESTQUEUE_H_

#include "QueueThreadSafe.hxx"
#include "ECURequest.h"

namespace ECU
{
   class CRequestQueue : public CQueueThreadSafe< CRequest >
   {
   public:
      enum ESizeState
      {
         ssNoChange,
         ssInc,
         ssDec
      };
      
   public:
      CRequestQueue();
      virtual ~CRequestQueue();
      
      ESizeState sizeState()const;
   };
}

#endif /* ECUREQUESTQUEUE_H_ */
