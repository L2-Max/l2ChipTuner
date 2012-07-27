/*
 * l2ParserDriver.cpp
 *
 *  Created on: 27.03.2009
 *      Author: L2-Max
 */

#include "l2ParserDriver.h"

#include <iostream>

namespace l2max
{
   namespace Registry
   {
      namespace Driver
      {
         CParserDriver::CParserDriver( const std::string& aFile, CLexScaner& aLexer ) :
            _file( aFile ),
            _lexer( aLexer ){}

         CParserDriver::~CParserDriver(){}

         void CParserDriver::RegisterError( const std::string& anError )const
         {
            std::cout << "CParserDriver::error: " << anError << std::endl;
         }

         std::string& CParserDriver::file()
         {
            return _file;
         }

         CLexScaner& CParserDriver::lexer()
         {
            return _lexer;
         }
      }
   }
}
