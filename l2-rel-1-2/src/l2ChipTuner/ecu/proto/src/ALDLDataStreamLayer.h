/*
 * ALDLDataStreamLayer.h
 *
 *  Created on: 14.11.2008
 *      Author: mars
 */

#ifndef ALDLDATASTREAMLAYER_H_
#define ALDLDATASTREAMLAYER_H_

#include "AbstractProto.h"

namespace l2max
{
   class CByteArray;
}

namespace ALDL
{
   class CDataStreamLayer : public CAbstractProto
   {
      static const unsigned char _lengthBase;
      static const unsigned char _maxResponseLength;

      unsigned char _ecuId;
      
      unsigned char ecuId()const;

      l2max::CByteArray reqHeader( unsigned char aLength )const;
      
   protected:
      bool MakeTransaction( unsigned char aMode, const l2max::CByteArray& aParam, l2max::CByteArray& aResp );

   public:
      enum EModes
      {
         mReturnToNormal = 0x00,
         mTransmitData   = 0x01,
         mDumpMemory     = 0x02,
         mCheckMemory    = 0x03,
         mECUFunc        = 0x04,
         mLoadRAM        = 0x05,
         mLoadRAMAndRum  = 0x06,
         mResetMalf      = 0x0A
      };

      CDataStreamLayer( CAbstractAdapter& anAdapter );
      virtual ~CDataStreamLayer();

      static unsigned char encLength( unsigned char aLength );
      static unsigned char decLength( unsigned char aLength );
      static unsigned char checkSum( const l2max::CByteArray& aByteArray );
      
      void SetECUId( unsigned char anECUId );
   };
}

#endif /* ALDLDATASTREAMLAYER_H_ */
