/*
 * l2AbstractRegistryDriver.cpp
 *
 *  Created on: 04.09.2009
 *      Author: L2-Max
 */

#include "l2AbstractRegistryDriver.h"
#include "l2CategoryKeyParser.h"

#include <l2Trace.h>
#include <l2Variant.h>
#include <l2MutexLocker.h>

namespace l2max
{
   namespace Registry
   {
      namespace Driver
      {
         CAbstractRegistryDriver::CAbstractRegistryDriver() : _category( *new CCategory() )
         {
            _isModified = false;
         }

         CAbstractRegistryDriver::~CAbstractRegistryDriver(){}

         unsigned int CAbstractRegistryDriver::run()
         {
            unsigned int ret( 0 );

            try
            {
               parse();
            }
            catch( const CException& anException )
            {
               _errorRepository.RegisterError( anException.what() );
               
               TraceCritical << "Registry driver ERROR:" << std::endl;
               TraceCritical << _errorRepository.errorString() << std::endl;
            }
            catch( ... )
            {
               TraceCritical << "Unknown Exception" << std::endl;
            }

            synchronization().parserStopped();

            return ret;
         }

         bool CAbstractRegistryDriver::isRun()const
         {
            return CThread::isRun() && synchronization().isRun() && synchronization().WaitForData();
         }

         bool CAbstractRegistryDriver::hasError()const
         {
            return _errorRepository.hasError();
         }

         std::string CAbstractRegistryDriver::errorString()const
         {
            return _errorRepository.errorString();
         }

         bool CAbstractRegistryDriver::Open( const std::string& aFile )
         {
            TRACE_FUN( Routine, "CAbstractRegistryDriver::Open" );

            bool ret( false );

            Close();

            CMutexLocker locker( synchronization().mutex() );
            
            synchronization().ResetTerminate();

            _file = aFile;
            _fstream.open( _file.c_str(), std::ios_base::in );
            
            if( ret = _fstream.is_open() )
            {
               _errorRepository.SetParsedFileName( _file );

               Start();
            }

            return ret && !hasError();
         }

         bool CAbstractRegistryDriver::Read( const std::string& aKey, CVariant& aValue )const
         {
            TRACE_FUN( Frequently, "CAbstractRegistryDriver::Read" );

            CMutexLocker locker( synchronization().mutex() );

            bool ret( false );

            do
            {
               CCategoryKeyParser keyParser( aKey );

               ret = category()->Read( keyParser, aValue );
               
               synchronization().continueRead();
            }
            while( !ret && isRun() );

            return ret;
         }

         bool CAbstractRegistryDriver::Write( const std::string& aKey, const CVariant& aValue )
         {
            TRACE_FUN( Frequently, "CAbstractRegistryDriver::Write" );

            CMutexLocker locker( synchronization().mutex() );
            
            bool ret( false );

            CCategoryKeyParser keyParser( aKey );
            
            if( ret = category()->Write( keyParser, aValue ) )
            {
               _isModified = true;
            }
            
            return ret;
         }
         
         void CAbstractRegistryDriver::Clear()
         {
            TRACE_FUN( Routine, "CAbstractRegistryDriver::Clear" );
            
            WaitForDriverStop();
            
            CMutexLocker locker( synchronization().mutex() );
            
            category()->Clear();
            
            _isModified = false;
         }

         const std::string& CAbstractRegistryDriver::file()const
         {
            return _file;
         }

         std::ifstream& CAbstractRegistryDriver::stream()
         {
            return _fstream;
         }

         const std::string& CAbstractRegistryDriver::parsedFileName()const
         {
            return file();
         }

         void CAbstractRegistryDriver::WaitForDriverStop()const
         {
            while( CThread::isRun() && !Wait( 0 ) )
            {
               synchronization().continueRead();
            }
         }

         HCategory CAbstractRegistryDriver::category()const
         {
            return _category;
         }

         TKeyListIterator CAbstractRegistryDriver::CategoryList( const std::string& aKey, TKeyList& aList )const
         {
            CMutexLocker locker( synchronization().mutex() );

            TKeyListIterator ret( aList.end() );

            do
            {
               CCategoryKeyParser keyParser( aKey );

               ret = category()->CategoryList( keyParser, aList );
               
               synchronization().continueRead();
            }
            while( ret == aList.end() && isRun() );

            return ret;
         }

         TKeyListIterator CAbstractRegistryDriver::VariableList( const std::string& aKey, TKeyList& aList )const
         {
            CMutexLocker locker( synchronization().mutex() );

            TKeyListIterator ret( aList.end() );

            do
            {
               CCategoryKeyParser keyParser( aKey );

               ret = category()->VariableList( keyParser, aList );
               
               synchronization().continueRead();
            }
            while( ret == aList.end() && isRun() );

            return ret;
         }

         const CParserDriverSynchronization& CAbstractRegistryDriver::synchronization()const
         {
            return _synchronization;
         }

         CParserErrorRepository& CAbstractRegistryDriver::errorRepository()
         {
            return _errorRepository;
         }
         
         bool CAbstractRegistryDriver::isModified()const
         {
            CMutexLocker locker( synchronization().mutex() );
            
            return _isModified;
         }
      }
   }
}
