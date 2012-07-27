/*
 * l2IRefObject.h
 *
 *  Created on: 05.01.2009
 *      Author: mars
 */

#ifndef L2IREFOBJECT_H_
#define L2IREFOBJECT_H_

namespace l2max
{
   struct IRefObject
   {
      virtual unsigned int AddRef() = 0;
      virtual unsigned int Release() = 0;
      virtual unsigned int refCount()const = 0;
   };
}

#endif /* L2IREFOBJECT_H_ */
