#ifndef L2TEXTTREEDRV_H_
#define L2TEXTTREEDRV_H_

#include <l2RegistryDriverInfo.h>
#include <l2AbstractRegistryDriver.h>

#include <map>
#include <fstream>

namespace l2max
{
   namespace Registry
   {
      namespace Driver
      {
         extern "C" HInterface CreateInstance();

         class CTreeImpl : public CAbstractRegistryDriver
         {
            friend class CTreeParserDriver;

            SInfo _registryDriverInfo;
            
            CTreeImpl( const CTreeImpl& );
            CTreeImpl& operator = ( const CTreeImpl& );

            virtual void parse();

         public:
            CTreeImpl();
            virtual ~CTreeImpl();

            virtual bool Close();

            virtual const SInfo* driverInfo()const;

            virtual EType type()const;
         };
      }
   }
}

#endif /*L2TEXTTREEDRV_H_*/
