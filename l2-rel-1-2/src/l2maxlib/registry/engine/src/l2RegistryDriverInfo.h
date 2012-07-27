/*
 * l2RegistryDriverInfo.h
 *
 *  Created on: 07.01.2009
 *      Author: L2-Max
 */

#ifndef L2REGISTRYDRIVERINFO_H_
#define L2REGISTRYDRIVERINFO_H_

#include <string>
#include <list>

namespace l2max
{
   namespace Registry
   {
      namespace Driver
      {
         typedef std::list< std::string > TErrorList;

         struct SInfo
         {
            std::string _name;
            std::string _description;
            std::string _version;
            std::string _fileName;
         };
      }
   }
}

#endif /* L2REGISTRYDRIVERINFO_H_ */
