/*
 * ALDLProtocolDiagnosticDataStorage.cpp
 *
 *  Created on: 03.09.2009
 *      Author: L2-Max
 */

#include "ALDLProtocolDiagnosticDataStorage.h"
#include "ALDLProto.h"

#include <AppDefs.h>
#include <AppContext.h>

namespace ALDL
{
   CProtocolDiagnosticDataStorage::CProtocolDiagnosticDataStorage( CProto* aProto, const l2max::CRegistry& aRegistry ) :
      ::CProtocolDiagnosticDataStorage( aRegistry )
   {
      if( IsWriteEnabled() )
      {
         writeRegistry().Write( V_DIAGF_CONF_PROTOTYPE, aProto->type() );
         writeRegistry().Write( V_DIAGF_CONF_ALDLECUID, aProto->ecuId() );
      }
   }

   CProtocolDiagnosticDataStorage::~CProtocolDiagnosticDataStorage(){}
}
