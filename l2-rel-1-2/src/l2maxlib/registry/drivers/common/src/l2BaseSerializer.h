/*
 * l2BaseSerializer.h
 *
 *  Created on: 31.03.2009
 *      Author: L2-Max
 */

#ifndef L2BASESERIALIZER_H_
#define L2BASESERIALIZER_H_

#include <string>

namespace l2max
{
   namespace Registry
   {
      namespace Driver
      {
         class CBaseSerializer
         {
            static const int _indentSize;

         protected:
            const std::string _indentation;

            std::string _serialized;

         public:
            CBaseSerializer( int anIndentation );
            virtual ~CBaseSerializer();
            
            static std::string cr();

            const std::string& serialized()const;
         };
      }
   }
}

#endif /* L2BASESERIALIZER_H_ */
