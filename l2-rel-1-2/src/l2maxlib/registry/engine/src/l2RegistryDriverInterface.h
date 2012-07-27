#ifndef L2REGISTRYDRIVERINTERFACE_H_
#define L2REGISTRYDRIVERINTERFACE_H_

#include "l2RegistryDriverTypes.h"

#include <l2IRefObject.h>
#include <l2RefHandle.hxx>

#include <string>

#define REGISTRY_DRIVER_CREATE_INSTANCE_ENTRY_NAME "CreateInstance"

namespace l2max
{
	class CVariant;

	namespace Registry
   {
      namespace Driver
      {
         struct SInfo;
         struct IInterface;
         
         typedef CRefHandle< IInterface > HInterface;
         typedef HInterface ( *pfnCreateInstance )();
         
         struct IInterface : virtual public IRefObject
         {
            virtual ~IInterface(){}
            
            virtual bool Open( const std::string& aFile ) = 0;
            virtual bool Read( const std::string& aKey, CVariant& aValue )const = 0;
            virtual bool Write( const std::string& aKey, const CVariant& aValue ) = 0;
            virtual bool Close() = 0;
            virtual void Clear() = 0;

            virtual const SInfo* driverInfo()const = 0;
            virtual EType type()const = 0;
            
            virtual bool hasError()const = 0;
            virtual std::string errorString()const = 0;
            
            virtual TKeyListIterator CategoryList( const std::string& aKey, Registry::TKeyList& aList )const = 0;
            virtual TKeyListIterator VariableList( const std::string& aKey, Registry::TKeyList& aList )const = 0;
         };
      }
   }
}

#endif /*L2DRIVERINTERFACE_H_*/
