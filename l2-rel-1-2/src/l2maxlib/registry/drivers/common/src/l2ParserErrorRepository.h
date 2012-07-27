/*
 * l2ParserErrorRepository.h
 *
 *  Created on: 04.02.2009
 *      Author: mars
 */

#ifndef L2PARSERERRORREPOSITORY_H_
#define L2PARSERERRORREPOSITORY_H_

#include <l2RegistryDriverInfo.h>

namespace l2max
{
   namespace Registry
   {
      namespace Driver
      {
         class CParserErrorRepository
         {
            TErrorList _errorList;
            
            std::string _parsedFileName;

         public:
            CParserErrorRepository();
            virtual ~CParserErrorRepository();
            
            void SetParsedFileName( const std::string aFile );
            const std::string& parsedFileName()const;
            
            void RegisterError( const std::string& anError );
            
            bool hasError()const;
            std::string errorString()const;
         };
      }
   }
}

#endif /* L2PARSERERRORREPOSITORY_H_ */
