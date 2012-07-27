/*
 * ALDLProtocolDiagnosticDataStorage.h
 *
 *  Created on: 03.09.2009
 *      Author: L2-Max
 */

#ifndef ALDLPROTOCOLDIAGNOSTICDATASTORAGE_H_
#define ALDLPROTOCOLDIAGNOSTICDATASTORAGE_H_

#include "ProtocolDiagnosticDataStorage.h"

namespace ALDL
{
   class CProto;

   class CProtocolDiagnosticDataStorage : public ::CProtocolDiagnosticDataStorage
   {
   public:
      CProtocolDiagnosticDataStorage( CProto* aProto, const l2max::CRegistry& aRegistry );
      virtual ~CProtocolDiagnosticDataStorage();
   };
}

#endif /* ALDLPROTOCOLDIAGNOSTICDATASTORAGE_H_ */
