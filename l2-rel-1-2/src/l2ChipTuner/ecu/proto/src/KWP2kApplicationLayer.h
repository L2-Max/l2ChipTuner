/*
 * KWP2kApplicationLayer.h
 *
 *  Created on: 06.11.2008
 *      Author: mars
 */

#ifndef KWP2KAPPLICATIONLAYER_H_
#define KWP2KAPPLICATIONLAYER_H_

#include "KWP2kDataLinkLayer.h"

namespace KWP2k
{
   class CApplicationLayer : public CDataLinkLayer
   {
      unsigned char _diagnosticSessionType;

      bool StartStandardDiagnosticSession();

   public:
      enum EApplicationLayerServiceId
      {
         alsiStartDiagnosticSessionReq = 0x10,
         alsiStartDiagnosticSessionResp = 0x50,

         alsiReadDataByLocalIdentifierReq = 0x21,
         alsiReadDataByLocalIdentifierResp = 0x61
      };

      enum EApplicationLayerDiagnosticSession
      {
         aldsStandardSession = 0x81,
         aldsPeriodicTransmission = 82,
         aldsProgrammingSession = 0x85,
         aldsDevelopmentSession = 0x86,
         aldsAjustementSession = 0x87
      };

      enum EApplicationLayerReacordLocalIdentifier
      {
         alrliLocalIdentifierScalingTable = 0x01
      };

      CApplicationLayer( CAbstractAdapter& anAdapter );
      virtual ~CApplicationLayer();

      virtual bool StartDiagnosticSession();
      virtual bool StopDiagnosticSession();
      virtual bool ReadDataByLocalIdentifier();
      virtual bool WriteDataByLocalIdentifier();
   };
}

#endif /* KWP2KAPPLICATIONLAYER_H_ */
