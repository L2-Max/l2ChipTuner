#ifndef L2REGISTRY_H_
#define L2REGISTRY_H_

#include "l2RegistryDriverInterface.h"

#include <l2Variant.h>
#include <l2Mutex.h>

#include <map>

namespace l2max
{
   class CVariant;

   struct SRegistryValue
   {
      const std::string _name;
      const CVariant _value;
   };

	class CRegistry
	{
	   Registry::Driver::HInterface _driver;

		typedef std::map< std::string, CVariant > TDefaultValueContainer;
		TDefaultValueContainer _defaultValueContainer;

		static CMutex _mutex;
		
		void Release();

	public:
		CRegistry();
		virtual ~CRegistry()throw();

		bool Open( const std::string& aFile, Registry::Driver::EType aType );
		bool Read( const std::string& aKey, CVariant& aValue )const;
		bool Write( const std::string& aKey, const CVariant& aValue );
		bool Close();

		void Clear();

		void DefineDefaultValue( const std::string& aKey, const CVariant& aValue );
		bool defaultValue( const std::string& aKey, CVariant& aValue )const;

		void Initialize( const SRegistryValue* aReagistryValue, int aCount );

		bool hasError()const;
		std::string errorString()const;
		
		Registry::TKeyListIterator CategoryList( const std::string& aKey, Registry::TKeyList& aList )const;
		Registry::TKeyListIterator VariableList( const std::string& aKey, Registry::TKeyList& aList )const;
	};
}

#endif /*L2REGISTRY_H_*/
