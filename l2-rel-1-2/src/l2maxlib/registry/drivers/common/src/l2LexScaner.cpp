/*
 * l2LexScaner.cpp
 *
 *  Created on: 29.03.2009
 *      Author: L2-Max
 */
#include "l2LexScaner.h"

namespace l2max
{
   namespace Registry
   {
      namespace Driver
      {
         int CL2FlexLexer::yylex(){ return -1; }

         CLexScaner::CLexScaner( std::istream* aYyin, std::ostream* aYyout ) : CL2FlexLexer( aYyin, aYyout ){}
      }
   }
}
