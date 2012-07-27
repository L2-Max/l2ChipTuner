/*
 * l2LexScaner.h
 *
 *  Created on: 27.03.2009
 *      Author: L2-Max
 */

#ifndef L2LEXSCANER_H_
#define L2LEXSCANER_H_

#include <l2Variant.h>

#undef yylex

#ifndef YY_DECL
   #define YY_DECL                                             \
      template< class TSemanticType, class TLocationType >     \
         int l2max::Registry::Driver::CLexScaner::yylex( TSemanticType* yylval,  \
                                                         TLocationType* yylloc,  \
                                                         l2max::Registry::Driver::CParserDriver& aDriver )
#endif

#undef yyFlexLexer
#define yyFlexLexer CL2FlexLexer
#include <FlexLexer.h>

namespace l2max
{
   namespace Registry
   {
      namespace Driver
      {
         class CParserDriver;

         class CLexScaner : public CL2FlexLexer
         {
         public:
            CLexScaner( std::istream* aYyin = 0, std::ostream* aYyout = 0 );
            virtual ~CLexScaner(){};

            template< class TSemanticType, class TLocationType >
               int yylex( TSemanticType* yylval, TLocationType* yylloc, CParserDriver& aDriver );
         };
      }
   }
}

#endif /* L2LEXSCANER_H_ */
