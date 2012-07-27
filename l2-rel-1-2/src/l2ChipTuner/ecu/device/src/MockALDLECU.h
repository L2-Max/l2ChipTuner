/*
 * KLineALDLECUMock.h
 *
 *  Created on: 09.10.2008
 *      Author: mars
 */

#ifndef KLINEALDLECUMOCK_H_
#define KLINEALDLECUMOCK_H_

#include "MockKLine.h"

#include <l2File.h>
#include <l2ByteArray.h>

namespace Mock
{
   class CALDLECU : public CKLine
   {
      unsigned char _ecuId;
      unsigned char _mode;
      unsigned char _table;

      bool _firstModeDataReq;

      l2max::CFile _file;
      l2max::CByteArray _control;

   public:
      CALDLECU( unsigned char aNumber );
      virtual ~CALDLECU();

      virtual int read( void* aData, unsigned int aSize );
      virtual int write( const void* aData, unsigned int aSize );
   };
}

#endif /* KLINEALDLECUMOCK_H_ */
