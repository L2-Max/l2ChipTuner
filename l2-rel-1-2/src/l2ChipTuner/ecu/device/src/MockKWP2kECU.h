/*
 * MockKWP2kECU.h
 *
 *  Created on: 07.11.2008
 *      Author: mars
 */

#ifndef MOCKKWP2KECU_H_
#define MOCKKWP2KECU_H_

#include "MockKLine.h"

namespace Mock
{
   class CKWP2kECU : public CKLine
   {
      unsigned char _state;

      unsigned char _format;
      unsigned char _target;
      unsigned char _source;

      unsigned char _diagSession;
      unsigned char _recordLocalIdentifier;

   public:
      CKWP2kECU( unsigned char aNumber );
      virtual ~CKWP2kECU();

      virtual int read( void* aData, unsigned int aSize );
      virtual int write( const void* aData, unsigned int aSize );
   };

}

#endif /* MOCKKWP2KECU_H_ */
