/*
 * l2BaseSerializer.cpp
 *
 *  Created on: 31.03.2009
 *      Author: L2-Max
 */

#include "l2BaseSerializer.h"

namespace l2max
{
   namespace Registry
   {
      namespace Driver
      {
         const int CBaseSerializer::_indentSize( 3 );

         CBaseSerializer::CBaseSerializer( int anIndentation ) :
            _indentation( CBaseSerializer::_indentSize * anIndentation, ' ' ){}

         CBaseSerializer::~CBaseSerializer(){}

         const std::string& CBaseSerializer::serialized()const
         {
            return _serialized;
         }
         
         std::string CBaseSerializer::cr()
         {
#ifdef __WIN32__
            return "\r\n";
#else
            return "\n";
#endif
         }
      }
   }
}
