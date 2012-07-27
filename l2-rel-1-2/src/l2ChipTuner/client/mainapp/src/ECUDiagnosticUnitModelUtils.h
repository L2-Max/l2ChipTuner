/*
 * ECUDiagnosticUnitModelUtils.h
 *
 *  Created on: 03.12.2008
 *      Author: mars
 */

#ifndef ECUDIAGNOSTICUNITMODELUTILS_H_
#define ECUDIAGNOSTICUNITMODELUTILS_H_

#include <DiagnosticTable.h>

#include <l2String.h>

#include <list>
#include <string>
#include <map>

namespace ECUDiagnosticModelUtils
{
   //////////////////////////////////////////////////////////////////////////////////////
   typedef l2max::TStringList TSynonyms;
   typedef std::vector< Diagnostic::CUnitKey > TKeyVector;

   std::string Synonyms2Str( const TSynonyms& aSynonyms );
   TSynonyms Str2Synonyms( const std::string& aSynonyms );

   //////////////////////////////////////////////////////////////////////////////////////
   std::string mimeTypes();
}

#endif /* ECUDIAGNOSTICUNITMODELUTILS_H_ */
