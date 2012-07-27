/*
 * ALDLProto.h
 *
 *  Created on: 05.10.2008
 *      Author: L2-Max
 */

#ifndef ALDLPROTO_H_
#define ALDLPROTO_H_

#include "ALDLDataStreamLayer.h"
#include "ALDLAbstractDataStreamHandles.h"

#include <ProtocolTable.h>
#include <VehicleTable.h>

#include <l2TextIniRegistry.h>

class CALDLProtocolCreationStrategy;
class CProtocolDiagnosticDataStorage;

namespace ALDL
{
   class CProto : public CDataStreamLayer
   {
      CALDLProtocolCreationStrategy* _strategy;
      CProtocolDiagnosticDataStorage* _storage;

      HAbstractMode1Data0 _abstractMode1Data0;
      HAbstractMode4 _abstractMode4;

      CProto( const CProto& );
      CProto& operator = ( const CProto& );

   public:
      enum EErrorCode
      {
         ecNoError,
         ecMode1Data0Deserialization,
         ecMode4Deserialization
      };

   private:
      Protocol::ALDL::EECUId _ecuId;

      typedef std::map< EErrorCode, std::string > TErrorTextMap;
      TErrorTextMap _errorTextMap;

      EErrorCode _errorCode;
      
      bool MakeTransaction( unsigned char aMode, const l2max::CByteArray& aParam, l2max::CByteArray& aResp );

   public:
      CProto( CAbstractAdapter& anAdapter, CALDLProtocolCreationStrategy* aStrategy );
      virtual ~CProto();

      virtual Diagnostic::HDataAdapter diagnosticDataAdapter();
      virtual Diagnostic::HControlAdapter diagnosticControlAdapter();
      virtual Diagnostic::HErrorAdapter diagnosticErrorAdapter();

      virtual bool StartDiagnosticSession();
      virtual bool StopDiagnosticSession();
      virtual bool ReadDataByLocalIdentifier();
      virtual bool WriteDataByLocalIdentifier();

      virtual void ResetError();

      virtual std::string implementationErrorText()const;
      virtual CAbstractProto::TErrorCode implementationErrorCode()const;

      Protocol::ALDL::EECUId ecuId()const;

      ::CProtocolDiagnosticDataStorage* storage();
   };
}

#endif /* ALDLPROTO_H_ */
