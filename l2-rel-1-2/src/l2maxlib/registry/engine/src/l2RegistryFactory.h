#ifndef L2REGISTRYFACTORY_H_
#define L2REGISTRYFACTORY_H_

#include "l2RegistryDriverInterface.h"

#include <l2Singleton.hxx>

#include <map>

namespace l2max
{
   namespace Registry
   {
      class CFactory : public CSingleton< CFactory >
      {
         friend class CSingleton< CFactory >;
         
         struct SDriverInstance;

         typedef CRefHandle< SDriverInstance > HDriverInstance;
         
         typedef std::map< Driver::EType, HDriverInstance > TDriverInstanceMap;
         TDriverInstanceMap _driverInstanceMap;

         CFactory();
         virtual ~CFactory();
         
      public:
         Driver::HInterface QueryInterface( Driver::EType aType );
         void Release( Driver::EType aType );
      };
   }
}
#endif /*L2REGISTRYDRIVERFACTORY_H_*/
