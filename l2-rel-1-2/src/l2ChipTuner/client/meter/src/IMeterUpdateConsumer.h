/*
 * IMeterUpdateConsumer.h
 *
 *  Created on: 04.11.2009
 *      Author: mars
 */

#ifndef IMETERUPDATECONSUMER_H_
#define IMETERUPDATECONSUMER_H_

struct IMeterUpdateConsumer
{
   virtual void modelUpdated() = 0;
};

#endif /* IMETERUPDATECONSUMER_H_ */
