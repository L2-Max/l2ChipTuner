/*
 * l2LexScanerImpl.h
 *
 *  Created on: 29.03.2009
 *      Author: L2-Max
 */

#ifndef L2LEXSCANERIMPL_H_
#define L2LEXSCANERIMPL_H_

namespace l2max
{
   namespace Registry
   {
      namespace Driver
      {
         template int CLexScaner::yylex( parser::semantic_type*, parser::location_type*, CParserDriver& );
      }
   }
}

#endif /* L2LEXSCANERIMPL_H_ */
