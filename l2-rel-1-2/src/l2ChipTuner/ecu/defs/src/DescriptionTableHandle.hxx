/*
 * DescriptionTableHandle.h
 *
 *  Created on: 24.03.2009
 *      Author: L2-Max
 */

#ifndef DESCRIPTIONTABLEHANDLE_H_
#define DESCRIPTIONTABLEHANDLE_H_

#include <l2Mutex.h>

template< class TDescriptionTable >
   class CDescriptionTableHandle
   {
      static l2max::CMutex _mutex;

      const TDescriptionTable& _descriptionTable;

      CDescriptionTableHandle& operator = ( const CDescriptionTableHandle& );

   public:
      CDescriptionTableHandle( const CDescriptionTableHandle& aHandle );
      CDescriptionTableHandle( const TDescriptionTable& aTable );
      virtual ~CDescriptionTableHandle();

      const TDescriptionTable* operator -> ();
   };

#endif /* DESCRIPTIONTABLEHANDLE_H_ */
