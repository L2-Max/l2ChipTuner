/*
 * l2RegistryDriverTypes.h
 *
 *  Created on: 07.01.2009
 *      Author: L2-Max
 */

#ifndef L2REGISTRYDRIVERTYPES_H_
#define L2REGISTRYDRIVERTYPES_H_

#include <list>
#include <string>

namespace l2max
{
   namespace Registry
   {
      typedef std::list< std::string > TKeyList;
      typedef TKeyList::iterator TKeyListIterator;
      
      namespace Driver
      {
         enum EType
         {
            tTextIni,
            tTextTree
         };
      }
   }
}

#endif /* L2REGISTRYDRIVERTYPES_H_ */
