/*
 * l2RefObject.h
 *
 *  Created on: 05.01.2009
 *      Author: mars
 */

#ifndef L2REFOBJECT_H_
#define L2REFOBJECT_H_

#include "l2IRefObject.h"

namespace l2max
{
   class CRefObject : virtual public IRefObject
   {
      mutable unsigned int _count;

      CRefObject( const CRefObject& );
      CRefObject& operator = ( const CRefObject& );

   protected:
      CRefObject();
      virtual ~CRefObject();

   public:
      virtual unsigned int AddRef();
      virtual unsigned int Release();
      virtual unsigned int refCount()const;
   };
}

#endif /* L2REFOBJECT_H_ */
