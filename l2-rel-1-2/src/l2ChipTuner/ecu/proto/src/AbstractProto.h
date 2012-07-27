/*
 * AbstractProto.h
 *
 *  Created on: 05.10.2008
 *      Author: L2-Max
 */

#ifndef ABSTRACTPROTO_H_
#define ABSTRACTPROTO_H_

#include "AbstractProtoError.h"
#include "DiagnosticAdapterHandles.h"

#include <ProtocolTable.h>

#include <vector>

namespace l2max
{
   struct IIODevice;

   class CByteArray;
}

class CAbstractAdapter;

class CAbstractProto : public CAbstractProtoError
{
protected:
   CAbstractAdapter& _adapter;

   std::string _namespace;

public:
   enum EWriteResultCode
   {
      wrcNotExecuted,
      wrcAccept,
      wrcReject
   };

protected:
   const Protocol::EProtocolType _type;

   EWriteResultCode _writeResultCode;

public:
   CAbstractProto( CAbstractAdapter& anAdapter, Protocol::EProtocolType aType );
   virtual ~CAbstractProto();

   virtual Diagnostic::HDataAdapter diagnosticDataAdapter() = 0;
   virtual Diagnostic::HControlAdapter diagnosticControlAdapter() = 0;
   virtual Diagnostic::HErrorAdapter diagnosticErrorAdapter() = 0;

   virtual bool StartDiagnosticSession() = 0;
   virtual bool StopDiagnosticSession() = 0;
   virtual bool ReadDataByLocalIdentifier() = 0;
   virtual bool WriteDataByLocalIdentifier() = 0;

   EWriteResultCode writeResultCode()const;

   Protocol::EProtocolType type()const;

   const std::string& registryNamespace()const;
};

#endif /* ABSTRACTPROTO_H_ */
