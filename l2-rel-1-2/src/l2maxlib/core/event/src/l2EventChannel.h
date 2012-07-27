/*
 * l2EventChannel.h
 *
 *  Created on: 13.08.2009
 *      Author: mars
 */

#ifndef L2EVENTCHANNEL_H_
#define L2EVENTCHANNEL_H_

#include <l2Thread.h>

namespace l2max
{
   struct IEventConsumer;
   
   class CEventChannel : public CThread
   {
   public:
      CEventChannel();
      virtual ~CEventChannel();
      
      bool Connect( IEventConsumer* aConsumer );
   };
}

#endif /* L2EVENTCHANNEL_H_ */
