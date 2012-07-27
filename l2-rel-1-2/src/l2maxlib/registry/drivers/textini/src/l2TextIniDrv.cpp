#include "l2TextIniDrv.h"
#include "l2TextIniCategorySerializer.h"

#include <l2TextIniParser.tab.hh>
#include <l2RegistryDriverException.h>
#include <l2Trace.h>
#include <l2Variant.h>
#include <l2CategoryKeyParser.h>
#include <l2File.h>
#include <l2MutexLocker.h>
#include <l2Config.h>

namespace l2max
{
   namespace Registry
   {
      namespace Driver
      {
         HInterface CreateInstance()
         {
            TRACE_FUN( Routine, "CreateInstance" );

            return *new CIniImpl();
         }

         CIniImpl::CIniImpl()
         {
            TRACE_FUN( Routine, "CIniImpl::CIniImpl" );

            _registryDriverInfo._name = "TextIni";
            _registryDriverInfo._description = "Group settings into ini-format";
            _registryDriverInfo._version = config::g_version;
            _registryDriverInfo._fileName = "l2maxRegIniDrv";
         }

         CIniImpl::~CIniImpl()
         {
            TRACE_FUN( Routine, "CIniImpl::~CIniImpl" );
         }

         bool CIniImpl::Close()
         {
            TRACE_FUN( Routine, "CIniImpl::Close" );

            if( !isModified() && CThread::isRun() )
            {
               synchronization().terminate();
            }
            
            WaitForDriverStop();

            bool ret( false );
            
            CMutexLocker locker( synchronization().mutex() );

            if( !( ret = !isModified() ) )
            {
               CFile theFile( file() );
               if( !hasError() && theFile.open( CFile::omWriteOnly | CFile::omTruncate ) )
               {
                  std::string buf;
   
                  buf += "/*********************************************************" + CBaseSerializer::cr() +
                         " * The file was generated by l2Registry driver" + CBaseSerializer::cr() +
                         " * Name: " + _registryDriverInfo._name + CBaseSerializer::cr() +
                         " * Description: " + _registryDriverInfo._description + CBaseSerializer::cr() +
                         " * Version: " + _registryDriverInfo._version + CBaseSerializer::cr() +
                         " *********************************************************/" + CBaseSerializer::cr();
   
                  buf += CTextIniCategorySerializer( category()->members(), 0 ).serialized();
   
                  ret = theFile.write( buf.c_str(), buf.size() ) == buf.size();
               }
            }
            
            return ret;
         }

         const SInfo* CIniImpl::driverInfo()const
         {
            return &_registryDriverInfo;
         }

         EType CIniImpl::type()const
         {
            return tTextIni;
         }

         void CIniImpl::parse()
         {
            TRACE_FUN( Routine, "CIniImpl::parse" );

            CLexScaner scaner( &stream() );
            CCategoryParserDriver driver( file(),
                                          scaner,
                                          category(),
                                          synchronization(),
                                          errorRepository() );

            parser theParser( driver );
            theParser.parse();
         }
      }
   }
}
