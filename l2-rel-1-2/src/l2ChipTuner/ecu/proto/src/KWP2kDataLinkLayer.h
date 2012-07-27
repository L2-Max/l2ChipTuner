/*
 * KWP2kDataLinkLayer.h
 *
 *  Created on: 06.11.2008
 *      Author: mars
 */

#ifndef KWP2KDATALINKLAYER_H_
#define KWP2KDATALINKLAYER_H_

#include "AbstractProto.h"
#include "KWP2kNegativeResponseHandler.h"

namespace l2max
{
   class CByteArray;
}

namespace KWP2k
{
   class CDataLinkLayer : public CAbstractProto,
                          public CNegativeResponseHandler
   {
      unsigned char _targetAddress;
      unsigned char _sourceAddress;

      unsigned char _keyByte1;
      unsigned char _keyByte2;

      unsigned char formatByte( unsigned char aServiceId, unsigned char aLength )const;
      unsigned char targetByte()const;
      unsigned char sourceByte()const;

      l2max::CByteArray reqHeader( unsigned char aServiceId, unsigned char aLength )const;

   public:
      enum EAddressingType
      {
         atPhysical = 0x80,
         atFunctional = 0xC0
      };

      enum EDataLinkServiceId
      {
         dlsiStartCommunicationReq = 0x81,
         dlsiStartCommunicationResp = 0xc1,

         dlsiStopCommunicationReq = 0x82,
         dlsiStopCommunicationResp = 0xc2
      };

      static unsigned char checkSum( const l2max::CByteArray& aData );

   private:
      EAddressingType _addressingType;

   protected:
      static const unsigned short _maxResponseLength;

      unsigned char _stopCommunicationRespCode;

      bool StartCommunication();
      bool StopCommunication();

      void SetAddressingType( EAddressingType anAddressingType );
      void SetTargetAddress( unsigned char aByte );
      void SetSourceAddress( unsigned char aByte );

      bool MakeTransaction( unsigned char aServiceId, const l2max::CByteArray& aParam, l2max::CByteArray& aResp );

   public:
      CDataLinkLayer( CAbstractAdapter& anAdapter );
      virtual ~CDataLinkLayer();
   };
}

#endif /* KWP2KDATALINKLAYER_H_ */
