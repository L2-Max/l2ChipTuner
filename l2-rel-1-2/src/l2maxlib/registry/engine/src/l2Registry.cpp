#include "l2Registry.h"
#include "l2RegistryFactory.h"
#include "l2RegistryDriverException.h"

#include <l2Trace.h>
#include <l2MutexLocker.h>
#include <l2ExceptionThrower.h>

namespace l2max
{
   CMutex CRegistry::_mutex;

	CRegistry::CRegistry()
	{
		TRACE_FUN( Routine, "CRegistry::CRegistry" );
	}

	CRegistry::~CRegistry()throw()
	{
		TRACE_FUN( Routine, "CRegistry::~CRegistry" );

		Close();
		
		Release();
	}
	
	void CRegistry::Release()
	{
	   if( !_driver.IsNull() )
	   {
	      Registry::Driver::EType type( _driver->type() );

	      _driver = Registry::Driver::HInterface();

	      Registry::CFactory::GetInstance()->Release( type );
	   }
	}

	bool CRegistry::Open( const std::string& aFile, Registry::Driver::EType aType )
	{
	   TRACE_FUN( Frequently, "CRegistry::Open" );

	   l2max::CMutexLocker locker( _mutex );

	   Registry::Driver::HInterface newDriver( Registry::CFactory::GetInstance()->QueryInterface( aType ) );
	   
	   Release();
	   
	   _driver = newDriver;

	   return !_driver.IsNull() && ( _driver->Open( aFile ) || !_driver->hasError() );
	}

	bool CRegistry::Read( const std::string& aKey, CVariant& aValue )const
	{
		TRACE_FUN( Frequently, "CRegistry::Read" );

		CMutexLocker locker( _mutex );

		bool ret( false );

		CHECK_OBJECT_STATE( !_driver.IsNull(), "There is no registry driver open" );

		if( !( ret = _driver->Read( aKey, aValue ) ) )
		{
			defaultValue( aKey, aValue );
		}

		return ret;
	}

	bool CRegistry::Write( const std::string& aKey, const CVariant& aValue )
	{
		TRACE_FUN( Frequently, "CRegistry::Write" );

		CMutexLocker locker( _mutex );

		CHECK_OBJECT_STATE( !_driver.IsNull(), "There is no registry driver open" );

		return _driver->Write( aKey, aValue );
	}

	bool CRegistry::Close()
	{
		TRACE_FUN( Frequently, "CRegistry::Close" );

		CMutexLocker locker( _mutex );

		return !_driver.IsNull() && _driver->Close();
	}

	void CRegistry::Clear()
	{
	   TRACE_FUN( Frequently, "CRegistry::Clear" );

	   CHECK_OBJECT_STATE( !_driver.IsNull(), "There is no registry driver open" );

	   _driver->Clear();
	}

	void CRegistry::DefineDefaultValue( const std::string& aKey, const CVariant& aValue )
	{
		TRACE_FUN( Frequently, "CRegistry::defineDefaultValue" );

		CMutexLocker locker( _mutex );

		_defaultValueContainer[ aKey ] = aValue;
	}

	bool CRegistry::defaultValue( const std::string& aKey, CVariant& aValue )const
	{
		TRACE_FUN( Frequently, "CRegistry::defaultValue" );

		CMutexLocker locker( _mutex );

		bool ret( true );

		TDefaultValueContainer::const_iterator itr( _defaultValueContainer.find( aKey ) );

		if( itr != _defaultValueContainer.end() )
		{
			aValue = itr->second;
		}
		else
		{
			ret = false;
		}

		return ret;
	}

	void CRegistry::Initialize( const SRegistryValue* aReagistryValue, int aCount )
	{
	   TRACE_FUN( Frequently, "CRegistry::initialise" );

	   CMutexLocker locker( _mutex );

	   for( int i( 0 ); i < aCount; ++i )
	   {
	      DefineDefaultValue( aReagistryValue[ i ]._name, aReagistryValue[ i ]._value );
	   }
	}

	bool CRegistry::hasError()const
	{
	   CHECK_OBJECT_STATE( !_driver.IsNull(), "There is no registry driver open" );

	   return _driver->hasError();
	}

	std::string CRegistry::errorString()const
	{
	   TRACE_FUN( Frequently, "CRegistry::errorString" );

	   CMutexLocker locker( _mutex );

	   std::string ret( "Driver not loaded" );

	   if( !_driver.IsNull() )
	   {
	      ret = _driver->errorString();
	   }

	   return ret;
	}
	
	Registry::TKeyListIterator CRegistry::CategoryList( const std::string& aKey, Registry::TKeyList& aList )const
   {
      CMutexLocker locker( _mutex );
      
      CHECK_OBJECT_STATE( !_driver.IsNull(), "There is no registry driver open" );
      
      return _driver->CategoryList( aKey, aList );
   }
   
	Registry::TKeyListIterator CRegistry::VariableList( const std::string& aKey, Registry::TKeyList& aList )const
   {
      CMutexLocker locker( _mutex );
      
      CHECK_OBJECT_STATE( !_driver.IsNull(), "There is no registry driver open" );
      
      return _driver->VariableList( aKey, aList );
   }
}
