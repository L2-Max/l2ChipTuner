/*
 * IMeter2DGraphUpdateConsumer.h
 *
 *  Created on: 04.11.2009
 *      Author: mars
 */

#ifndef IMETER2DGRAPHUPDATECONSUMER_H_
#define IMETER2DGRAPHUPDATECONSUMER_H_

#include "IMeterUpdateConsumer.h"

struct IMeter2DGraphUpdateConsumer : virtual public IMeterUpdateConsumer
{
   virtual void timeFrameChanged() = 0;
   virtual void valueFrameChanged() = 0;
   virtual void valueChanged() = 0;
};

#endif /* IMETER2DGRAPHUPDATECONSUMER_H_ */
