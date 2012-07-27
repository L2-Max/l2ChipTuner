/*
 * l2ParserDriver.h
 *
 *  Created on: 27.03.2009
 *      Author: L2-Max
 */

#ifndef L2PARSERDRIVER_H_
#define L2PARSERDRIVER_H_

#include <string>
#include <fstream>
#include <sstream>

namespace l2max
{
   class location;

   namespace Registry
   {
      namespace Driver
      {
         class CLexScaner;

         class CParserDriver
         {
            std::string _file;

            CLexScaner& _lexer;

         public:
            CParserDriver( const std::string& aFile, CLexScaner& aLexer );
            virtual ~CParserDriver();

            virtual void RegisterError( const std::string& anError )const;

            void error( const location& aLocation, const std::string& aMessage )const;

            std::string& file();

            CLexScaner& lexer();
         };
      }
   }
}

#endif /* L2PARSERDRIVER_H_ */
