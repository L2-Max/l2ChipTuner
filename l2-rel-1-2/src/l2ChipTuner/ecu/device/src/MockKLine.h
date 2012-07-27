/*
 * MockKLine.h
 *
 *  Created on: 07.11.2008
 *      Author: mars
 */

#ifndef MOCKKLINE_H_
#define MOCKKLINE_H_

#include <l2SerialLine.h>
#include <l2ByteArray.h>

namespace Mock
{
   class CKLine : public l2max::CSerialLine
   {
      enum EKLineState
      {
         klsReady,
         klsEcho
      };

      EKLineState _klineState;

      l2max::CByteArray _echo;

   public:
      CKLine( unsigned char aNumber );
      virtual ~CKLine();

      virtual int read( void* aData, unsigned int aSize );
      virtual int write( const void* aData, unsigned int aSize );
   };
}

#endif /* MOCKKLINE_H_ */
