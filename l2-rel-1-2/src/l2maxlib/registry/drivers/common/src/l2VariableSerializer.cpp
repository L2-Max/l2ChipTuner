/*
 * l2VariableSerializer.cpp
 *
 *  Created on: 31.03.2009
 *      Author: L2-Max
 */

#include "l2VariableSerializer.h"

#include <L2Variant.h>

namespace l2max
{
   namespace Registry
   {
      namespace Driver
      {
         ////////////////////////////////////////////////////////////////////////////////////////////////////////////
         const int CVariableSerializer::_literalStringLength( 32 );
         const int CVariableSerializer::_literalBinaryLength( 32 );

         CVariableSerializer::CVariableSerializer( const TVariable& aVariable, int anIndentation ) : CBaseSerializer( anIndentation )
         {
            _serialized += _indentation;

            std::string variableDefinition( aVariable.first + " = " );

            _indentationColumn = std::string( variableDefinition.size(), ' ' );

            _serialized += variableDefinition;


            if( aVariable.second->type() == CVariant::tBinary )
            {
               std::string binaryBuf( aVariable.second->toString() );

               for( int i( 0 ); i < binaryBuf.size(); i += 8 )
               {
                  if( i && !( i % CVariableSerializer::_literalBinaryLength ) )
                  {
                     _serialized += CBaseSerializer::cr();
                     _serialized += _indentation;
                     _serialized += _indentationColumn;
                  }

                  _serialized += " 0x" + binaryBuf.substr( i, 8 );
               }
            }
            else if( aVariable.second->type() == CVariant::tString )
            {
               std::string stringBuf( aVariable.second->toString() );

               const char* escSeq[][ 2 ] =
               {
                     { "\\n", "\n" },
                     { "\\t", "\t" },
                     { "\\r", "\r" }
               };

               for( int i( 0 ); i < sizeof( escSeq ) / sizeof( *escSeq ); ++i )
               {
                  int pos( 0 );

                  while( pos != std::string::npos )
                  {
                     pos = stringBuf.find( escSeq[ i ][ 1 ], pos );

                     if( pos != std::string::npos )
                     {
                        stringBuf.replace( pos, 1, escSeq[ i ][ 0 ] );
                     }
                  }
               }

               _serialized += "\"";

               for( int i( 0 ); i < stringBuf.size(); i += CVariableSerializer::_literalStringLength )
               {
                  if( i && !( i % CVariableSerializer::_literalStringLength ) )
                  {
                     _serialized += "\"" + CBaseSerializer::cr();
                     _serialized += _indentation;
                     _serialized += _indentationColumn;
                     _serialized += "\"";
                  }

                  _serialized += stringBuf.substr( i, CVariableSerializer::_literalStringLength );
               }

               _serialized += "\"";
            }
            else
            {
               _serialized += aVariable.second->toString();
            }

            _serialized += ";" + CBaseSerializer::cr();
         }

         CVariableSerializer::~CVariableSerializer(){}

         ////////////////////////////////////////////////////////////////////////////////////////////////////////////
         CVariablesSerializer::CVariablesSerializer( const TVariableSeq& aVariables, int anIndentation ) : CBaseSerializer( anIndentation )
         {
            for( TVariableSeq::const_iterator i( aVariables.begin() ); i != aVariables.end(); ++i )
            {
               _serialized += CVariableSerializer( i->second, anIndentation ).serialized();
            }
         }

         CVariablesSerializer::~CVariablesSerializer(){}
      }
   }
}
