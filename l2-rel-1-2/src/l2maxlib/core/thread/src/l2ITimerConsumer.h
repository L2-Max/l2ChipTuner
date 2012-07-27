/*
 * l2ITimerConsumer.h
 *
 *  Created on: 19.02.2009
 *      Author: mars
 */

#ifndef L2ITIMERCONSUMER_H_
#define L2ITIMERCONSUMER_H_

namespace l2max
{
   struct ITimerConsumer
   {
      virtual void timeout() = 0;
   };
}

#endif /* L2ITIMERCONSUMER_H_ */
