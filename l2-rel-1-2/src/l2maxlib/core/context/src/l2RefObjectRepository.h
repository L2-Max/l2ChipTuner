/*
 * l2RefObjectRepository.h
 *
 *  Created on: 25.02.2009
 *      Author: L2-Max
 */

#ifndef L2REFOBJECTREPOSITORY_H_
#define L2REFOBJECTREPOSITORY_H_

#include <l2Mutex.h>

#include <set>

namespace l2max
{
   struct IRefObject;

   class CRefObjectRepository
   {
      typedef std::set< const IRefObject* > TRefObjectSet;
      TRefObjectSet _refObjectSet;

      unsigned int _regCount;
      unsigned int _unRegCount;

      CMutex _mutex;

   public:
      CRefObjectRepository();
      virtual ~CRefObjectRepository();

      void Register( const IRefObject& anObject );
      void UnRegister( const IRefObject& anObject );

      void dump()const;
   };
}

#endif /* L2REFOBJECTREPOSITORY_H_ */
