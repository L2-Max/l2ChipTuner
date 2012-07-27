/*
 * KWP2kProto.cpp
 *
 *  Created on: 04.11.2008
 *      Author: mars
 */

#include "KWP2kProto.h"
#include "KWP2kDataLinkLayer.h"

#include "DiagnosticDataAdapter.h"
#include "DiagnosticErrorAdapter.h"
#include "DiagnosticControlAdapter.h"

#include <l2Trace.h>

namespace KWP2k
{
   CProto::CProto( CAbstractAdapter& anAdapter ) : CApplicationLayer( anAdapter )
   {
      SetAddressingType( atPhysical );
      SetTargetAddress( 0x10 );
      SetSourceAddress( 0xf0 );
   }

   CProto::~CProto(){}

   Diagnostic::HDataAdapter CProto::diagnosticDataAdapter()
   {
      TRACE_FUN( Routine, "KWP2k::CProto::diagnosticDataAdapter" );

      return Diagnostic::HDataAdapter();
   }

   Diagnostic::HControlAdapter CProto::diagnosticControlAdapter()
   {
      TRACE_FUN( Routine, "KWP2k::CProto::diagnosticControlAdapter" );

      return Diagnostic::HControlAdapter();
   }

   Diagnostic::HErrorAdapter CProto::diagnosticErrorAdapter()
   {
      TRACE_FUN( Routine, "KWP2k::CProto::diagnosticErrorAdapter" );

      return Diagnostic::HErrorAdapter();
   }

   std::string CProto::implementationErrorText()const
   {
      return "not implemented";
   }

   CAbstractProto::TErrorCode CProto::implementationErrorCode()const
   {
      return 0;
   }
}
