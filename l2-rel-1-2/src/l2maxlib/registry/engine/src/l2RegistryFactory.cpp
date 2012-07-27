#include "l2RegistryFactory.h"

#include <l2Trace.h>
#include <l2Library.h>

#include <l2RefObject.h>

#include <map>

#include <sys/stat.h>

#ifdef __WIN32__
	#define DRV_PREFIX ""
	#define DRV_SUFFIX ".dll"

#elif __LINUX__
	#define DRV_PREFIX "lib"
	#define DRV_SUFFIX ".a"

#endif

namespace l2max
{
   namespace Registry
   {
      namespace
      {
         typedef std::map< Driver::EType, std::string > TDriversMap;
         TDriversMap g_driversMap;

         const TDriversMap& driversMap()
         {
            if( !g_driversMap.size() )
            {
               g_driversMap[ Driver::tTextIni ] = DRV_PREFIX"l2maxRegIniDrv"DRV_SUFFIX;
               g_driversMap[ Driver::tTextTree ] = DRV_PREFIX"l2maxRegTreeDrv"DRV_SUFFIX;
            }

            return g_driversMap;
         }
      }

      ///////////////////////////////////////////////////////////////////////////
      struct CFactory::SDriverInstance : public CRefObject
      {
         CLibrary _library;
         Driver::pfnCreateInstance _createInstance;
         
         int _refCount;
         
         SDriverInstance()
         {
            _refCount = 0;
         }
      };
      
      CFactory::CFactory()
      {
         TRACE_FUN( Routine, "CFactory::CFactory");
      }

      CFactory::~CFactory()
      {
         TRACE_FUN( Routine, "CFactory::~CFactory");
      }

      Driver::HInterface CFactory::QueryInterface( Driver::EType aType )
      {
         TRACE_FUN( Routine, "CFactory::QueryInterface" );

         Driver::HInterface ret;

         TDriverInstanceMap::iterator I_instance( _driverInstanceMap.find( aType ) );

         if( I_instance == _driverInstanceMap.end() )
         {
            std::string driverLibrary( driversMap().find( aType )->second );

            struct stat st;
            if( ::stat( driverLibrary.c_str(), &st ) != ENOENT )
            {
               HDriverInstance driverInstance( *new SDriverInstance() );
               
               driverInstance->_library.SetFile( driverLibrary );

               if( driverInstance->_library.Load() )
               {
                  driverInstance->_createInstance =
                     reinterpret_cast< Driver::pfnCreateInstance >( driverInstance->_library.Resolve( REGISTRY_DRIVER_CREATE_INSTANCE_ENTRY_NAME ) );

                  if( driverInstance->_createInstance )
                  {
                     I_instance = _driverInstanceMap.insert( TDriverInstanceMap::value_type( aType, driverInstance ) ).first;
                  }
               }
            }
         }

         if( I_instance != _driverInstanceMap.end() )
         {
            ++( I_instance->second->_refCount );
            
            ret = I_instance->second->_createInstance();
         }

         return ret;
      }
      
      void CFactory::Release( Driver::EType aType )
      {
         TRACE_FUN( Routine, "CFactory::Release" );
         
         TDriverInstanceMap::iterator I_instance( _driverInstanceMap.find( aType ) );
         
         if( I_instance != _driverInstanceMap.end() )
         {
            if( !--( I_instance->second->_refCount ) )
            {
               _driverInstanceMap.erase( I_instance );
            }
         }
      }
   }
}
