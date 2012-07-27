/*
 * ProtocolDiagnosticDataStorage
 *
 *  Created on: 02.09.2009
 *      Author: mars
 */

#include "ProtocolDiagnosticDataStorage.h"
#include "IProtocolDiagnosticDataStorageEventSupplier.h"

#include <AppDefs.h>
#include <AppContext.h>

#include <l2Thread.h>
#include <l2Trace.h>
#include <l2MutexLocker.h>
#include <l2Timer.h>

CProtocolDiagnosticDataStorage::CProtocolDiagnosticDataStorage( const l2max::CRegistry& aRegistry ) :
   _readRegistry( aRegistry )
{
   TRACE_FUN( Routine, "CProtocolDiagnosticDataStorage::CProtocolDiagnosticDataStorage" );
   
   _readPacketTimeo = 0;
   
   _writePacketNo = 0;
   _writePacketTimeo = 0;
   
   _supplier = 0;

   _currentKey = _keys.end();
   _lastReadKey = _keys.end();
   
   l2max::CVariant isReadFromFile;
   CAppContext::GetInstance()->MainRegistry().Read( V_ECU_DIAG_SESS_DATAFILER_E, isReadFromFile );
   
   _isReadEnabled = isReadFromFile.toBool();


   l2max::CVariant wrtiteToEnabled;
   CAppContext::GetInstance()->MainRegistry().Read( V_ECU_DIAG_SESS_DATAFILEW_E, wrtiteToEnabled );

   _isWriteEnabled = wrtiteToEnabled.toBool();

   if( IsWriteEnabled() )
   {
      l2max::CVariant dataDir;
      CAppContext::GetInstance()->MainRegistry().Read( V_ECU_DIAG_SESS_DATADIR, dataDir );

      l2max::CVariant writeToFile;
      CAppContext::GetInstance()->MainRegistry().Read( V_ECU_DIAG_SESS_DATAFILEW, writeToFile );

      writeRegistry().Open( dataDir.toString() + "/" + writeToFile.toString() );
      writeRegistry().Clear();
   }
}

CProtocolDiagnosticDataStorage::~CProtocolDiagnosticDataStorage()
{
   TRACE_FUN( Routine, "CProtocolDiagnosticDataStorage::~CProtocolDiagnosticDataStorage" );
}

std::string CProtocolDiagnosticDataStorage::buildKey( int aNum )
{
   std::string packetKey( S_DIAGF_DIAGNOSTICDATA_PACKET );
   packetKey += "_";

   std::string number( l2max::CVariant( aNum ).toString() );
   number.insert( number.begin(), 10 - number.size(), '0' );

   return packetKey + number;
}

bool CProtocolDiagnosticDataStorage::Read( l2max::CByteArray& aPacket )const
{
   TRACE_FUN( Routine, "CProtocolDiagnosticDataStorage::Read" );

   l2max::CMutexLocker locker( _mutex );
   
   bool ret( false );
   
   unsigned int timeo( l2max::CTimer::clock() );
   
   if( !_readPacketTimeo )
   {
      _readPacketTimeo = timeo;
   }
   
   int readTimeo( timeo - _readPacketTimeo );
   
   do
   {
      if( _currentKey == _keys.end() )
      {
         _currentKey = readRegistry().CategoryList( S_DIAGF_DIAGNOSTICDATA, _keys );
         _lastReadKey = _currentKey;
      }
      else
      {
         if( _lastReadKey != _keys.end() )
         {
            _lastReadKey = readRegistry().CategoryList( S_DIAGF_DIAGNOSTICDATA, _keys );
         }
      }
   
      if( _currentKey != _keys.end() )
      {
         l2max::CVariant dataValue;
         l2max::CVariant timeoValue;
   
         if( ret = ( readRegistry().Read( S_DIAGF_DIAGNOSTICDATA"::" + *_currentKey + V_DIAGF_DIAGNOSTICDATA_DATA, dataValue ) &&
                     readRegistry().Read( S_DIAGF_DIAGNOSTICDATA"::" + *_currentKey + V_DIAGF_DIAGNOSTICDATA_TIMEO, timeoValue ) &&
                     dataValue.isValid() &&
                     timeoValue.isValid() ) )
         {
            if( readTimeo > timeoValue.toInt() )
            {
               ++_currentKey;
               
               if( _readPacketTimeo != timeo )
               {
                  _readPacketTimeo = timeo;
               }
            }
            else
            {
               aPacket = dataValue.toByteArray();
               
               if( _supplier )
               {
                  _supplier->packetRead();
               }
            }
            
            readTimeo -= timeoValue.toInt();
         }
      }
      else
      {
         break;
      }
   }
   while( readTimeo > 0 && ret );

   return ret;
}

bool CProtocolDiagnosticDataStorage::Write( const l2max::CByteArray& aPacket )
{
   TRACE_FUN( Routine, "CProtocolDiagnosticDataStorage::Write" );

   l2max::CMutexLocker locker( _mutex );
   
   bool ret( false );

   if( IsWriteEnabled() )
   {
      unsigned int timeo( l2max::CTimer::clock() );
      
      if( !_writePacketTimeo )
      {
         _writePacketTimeo = timeo;
      }
      
      unsigned int packetTimeo( timeo - _writePacketTimeo );
      
      _writePacketTimeo = timeo;
      
      ( ret = writeRegistry().Write( CProtocolDiagnosticDataStorage::buildKey( _writePacketNo ) + V_DIAGF_DIAGNOSTICDATA_DATA, aPacket ) ) &&
      ( ret = writeRegistry().Write( CProtocolDiagnosticDataStorage::buildKey( _writePacketNo ) + V_DIAGF_DIAGNOSTICDATA_TIMEO, packetTimeo ) );

      ++_writePacketNo;
   }

   return ret;
}

const l2max::CRegistry& CProtocolDiagnosticDataStorage::readRegistry()const
{
   return _readRegistry;
}

l2max::CTextTreeRegistry& CProtocolDiagnosticDataStorage::writeRegistry()
{
   return _writeRegistry;
}

bool CProtocolDiagnosticDataStorage::IsWriteEnabled()const
{
   return _isWriteEnabled;
}

bool CProtocolDiagnosticDataStorage::IsReadEnabled()const
{
   return _isReadEnabled;
}

void CProtocolDiagnosticDataStorage::SetSupplier( IProtocolDiagnosticDataStorageEventSupplier* aSupplier )
{
   TRACE_FUN( Routine, "CProtocolDiagnosticDataStorage::SetSupplier" );
   
   l2max::CMutexLocker locker( _mutex );
   
   _supplier = aSupplier;
}

unsigned int CProtocolDiagnosticDataStorage::packetCount()const
{
   l2max::CMutexLocker locker( _mutex );
   
   return _keys.size();
}

unsigned int CProtocolDiagnosticDataStorage::packetNo()const
{
   l2max::CMutexLocker locker( _mutex );
   
   unsigned int ret( 0 );
   
   for( l2max::Registry::TKeyListIterator I_keys( _keys.begin() );
        _currentKey != I_keys && I_keys != _keys.end(); ++I_keys )
   {
      ++ret;
   }
   
   return ret;
}

void CProtocolDiagnosticDataStorage::SetPacketNo( unsigned int aPacketNo )
{
   TRACE_FUN( Routine, "CProtocolDiagnosticDataStorage::SetPacketNo" );
   
   l2max::CMutexLocker locker( _mutex );
   
   _currentKey = _keys.begin();
   
   for( int i( 0 ); i < aPacketNo; ++i )
   {
      ++_currentKey;
   }
}
