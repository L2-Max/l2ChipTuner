/*
 * l2RegistryDriverException.h
 *
 *  Created on: 07.01.2009
 *      Author: L2-Max
 */

#ifndef L2REGISTRYDRIVEREXCEPTION_H_
#define L2REGISTRYDRIVEREXCEPTION_H_

#include <l2Exception.h>

namespace l2max
{
   namespace Registry
   {
      namespace Driver
      {
         class CException : public l2max::CException
         {
         public:
            CException( const std::string& aDesc ) : l2max::CException( aDesc ) { }
         };
      }
   }
}

#endif /* L2REGISTRYDRIVEREXCEPTION_H_ */
