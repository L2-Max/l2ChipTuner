/*
 * l2VariableSerializer.h
 *
 *  Created on: 31.03.2009
 *      Author: L2-Max
 */

#ifndef L2VARIABLESERIALIZER_H_
#define L2VARIABLESERIALIZER_H_

#include "l2BaseSerializer.h"
#include "l2CategoryParserDriver.h"

namespace l2max
{
   namespace Registry
   {
      namespace Driver
      {
         class CVariableSerializer : public CBaseSerializer
         {
            static const int _literalStringLength;
            static const int _literalBinaryLength;

            std::string _indentationColumn;

         public:
            CVariableSerializer( const TVariable& aVariable, int anIndentation );
            virtual ~CVariableSerializer();
         };

         class CVariablesSerializer : public CBaseSerializer
         {
         public:
            CVariablesSerializer( const TVariableSeq& aVariables, int anIndentation );
            virtual ~CVariablesSerializer();
         };
      }
   }
}

#endif /* L2VARIABLESERIALIZER_H_ */
