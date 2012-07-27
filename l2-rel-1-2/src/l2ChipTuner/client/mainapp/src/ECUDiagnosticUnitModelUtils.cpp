/*
 * ECUDiagnosticUnitModelUtils.cpp
 *
 *  Created on: 03.12.2008
 *      Author: mars
 */

#include "ECUDiagnosticUnitModelUtils.h"

#include <l2Trace.h>

#define SYNONYMS_SEPARATOR ":"

namespace ECUDiagnosticModelUtils
{
   //////////////////////////////////////////////////////////////////////////////////////
   std::string Synonyms2Str( const TSynonyms& aSynonyms )
   {
      TRACE_FUN( Routine, "::Synonyms2Str" );
      
      return l2max::CString::List2String( aSynonyms, SYNONYMS_SEPARATOR );
   }

   TSynonyms Str2Synonyms( const std::string& aSynonyms )
   {
      TRACE_FUN( Routine, "::Synonyms2Str" );
      
      return l2max::CString::String2List( aSynonyms, SYNONYMS_SEPARATOR );
   }

   //////////////////////////////////////////////////////////////////////////////////////
   std::string mimeTypes()
   {
      return "l2ChipTuner/diagnosticDataUnitTypes";
   }
}
