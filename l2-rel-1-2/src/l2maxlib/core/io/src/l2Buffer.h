/*
 * l2Buffer.h
 *
 *  Created on: 27.10.2008
 *      Author: mars
 */

#ifndef L2BUFFER_H_
#define L2BUFFER_H_

#include "l2AbstractIODevice.h"

namespace l2max
{
   class CByteArray;

   class CBuffer : public CAbstractIODevice
   {
      CByteArray* _buffer;

      unsigned int _position;

   public:
      CBuffer( CByteArray* aBuffer );
      virtual ~CBuffer();

      virtual unsigned int position()const;
      virtual unsigned int size()const;

      virtual int read( void* aData, unsigned int aSize );
      virtual int write( const void* aData, unsigned int aSize );

      virtual bool open( TOpenMode aOpenMode );
      virtual bool seek( int offset, ESeekMode seekMode );
      virtual bool close();
   };

}

#endif /* L2BUFFER_H_ */
