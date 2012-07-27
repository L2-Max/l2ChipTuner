#ifndef L2TEXTINIDRV_H_
#define L2TEXTINIDRV_H_

#include <l2RegistryDriverInfo.h>
#include <l2AbstractRegistryDriver.h>

#include <map>

namespace l2max
{
   namespace Registry
   {
      namespace Driver
      {
         extern "C" HInterface CreateInstance();

         typedef std::map< std::string, CVariant > TOperatorList;
         typedef std::map< std::string, TOperatorList > TGroupList;

         class CIniImpl : public CAbstractRegistryDriver
         {
            SInfo _registryDriverInfo;

            CIniImpl( const CIniImpl& );
            CIniImpl& operator = ( const CIniImpl& );

            virtual void parse();

         public:
            CIniImpl();
            virtual ~CIniImpl();

            virtual bool Close();

            virtual const SInfo* driverInfo()const;

            virtual EType type()const;
         };
      }
   }
}

#endif /*L2TEXTINIDRV_H_*/
