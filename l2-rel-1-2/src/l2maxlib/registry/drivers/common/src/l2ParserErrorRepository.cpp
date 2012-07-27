/*
 * l2ParserErrorRepository.cpp
 *
 *  Created on: 04.02.2009
 *      Author: mars
 */

#include "l2ParserErrorRepository.h"

#include <sstream>

namespace l2max
{
   namespace Registry
   {
      namespace Driver
      {
         CParserErrorRepository::CParserErrorRepository(){}

         CParserErrorRepository::~CParserErrorRepository(){}

         void CParserErrorRepository::RegisterError( const std::string& anError )
         {
            _errorList.push_back( anError );
         }

         void CParserErrorRepository::SetParsedFileName( const std::string aFile )
         {
            _parsedFileName = aFile;
         }

         const std::string& CParserErrorRepository::parsedFileName()const
         {
            return _parsedFileName;
         }

         std::string CParserErrorRepository::errorString()const
         {
            std::string ret;

            if( _errorList.size() )
            {
               std::ostringstream what;

               what << "Registry driver found " << _errorList.size() << " error(s) while parsing file: " << parsedFileName() << std::endl;

               for( TErrorList::const_iterator i( _errorList.begin() ); i != _errorList.end(); ++i )
               {
                  what << *i << std::endl;
               }

               ret = what.str();
            }

            return ret;
         }

         bool CParserErrorRepository::hasError()const
         {
            return _errorList.size();
         }
      }
   }
}
