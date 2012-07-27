#include "l2Library.h"
#include "l2Trace.h"

#include <l2ExceptionThrower.h>

#ifdef __WIN32__
	#include <windows.h>
#endif

namespace l2max
{
   CLibrary::CLibrary()
   {
      _module = 0;
   }
   
   CLibrary::CLibrary( const std::string& aFile )
   {
      _module = 0;
      
      SetFile( aFile );
   }

   CLibrary::~CLibrary()
   {
      TRACE_FUN( Routine, "CLibrary::~CLibrary" );

      UnLoad();
   }
   
   void CLibrary::SetFile( const std::string& aFile )
   {
      _file = aFile;
      
      if( IsLoaded() )
      {
         UnLoad();
      }
   }

   bool CLibrary::Load()
   {
      TRACE_FUN( Frequently, "CLibrary::Load" );

      if( !IsLoaded() )
      {
         CHECK_OBJECT_STATE( _file.size(), "File is not set" );
         
         _module = ::LoadLibrary( _file.c_str() );
      }

      return IsLoaded();
   }

   bool CLibrary::UnLoad()
   {
      TRACE_FUN( Frequently, "CLibrary::UnLoad" );

      if( IsLoaded() )
      {
         if( ::FreeLibrary( reinterpret_cast< HMODULE >( _module ) ) )
         {
            TraceRoutine << "!!! Library '" << _file << "' unloaded" << std::endl;

            ::CloseHandle( _module );

            _module = 0;
         }
      }

      return !IsLoaded();
   }

   bool CLibrary::IsLoaded()const
   {
      TRACE_FUN( Frequently, "CLibrary::isLoaded" );

      return _module;
   }

   pfnType CLibrary::Resolve( const std::string& anEntry )const
   {
      TRACE_FUN( Frequently, "CLibrary::resolve" );

      pfnType pfn( 0 );

      if( IsLoaded() )
      {
         pfn = reinterpret_cast< pfnType >( GetProcAddress( reinterpret_cast< HMODULE >( _module ), anEntry.c_str() ) );
      }

      return pfn;
   }
}
