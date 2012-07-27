/*
 * ECUUnitsTable.cpp
 *
 *  Created on: 25.10.2008
 *      Author: L2-Max
 */

#include "ECUUnitsTable.h"
#include "ECUControlUnit.h"

#include <DiagnosticDataAdapter.h>
#include <DiagnosticControlAdapter.h>

#include <l2ExceptionThrower.h>
#include <l2MutexLocker.h>

namespace ECU
{
   CUnitsTable::CUnitsTable( const std::string& aNamespace ) : _namespace( aNamespace ){}

   CUnitsTable::~CUnitsTable()
   {
      for( TControlContainer::iterator i( _controlsContainer.begin() ); i != _controlsContainer.end(); ++i )
      {
         delete i->second;
      }
   }

   void CUnitsTable::BuildDataTable( Diagnostic::HDataAdapter anAdapter )
   {
      l2max::CMutexLocker locker( _mutex );

      Diagnostic::TUnitKeys keys( anAdapter->keys() );

      for( Diagnostic::TUnitKeys::iterator i( keys.begin() ); i != keys.end(); ++i )
      {
         _unitsContainer[ *i ] = *new CDataUnit( i->type(), anAdapter, _namespace );
      }
   }

   Diagnostic::CUnitKey CUnitsTable::Synonym2Key( const std::string& aSynonym )
   {
      l2max::CMutexLocker locker( _mutex );

      Diagnostic::CUnitKey ret;

      Diagnostic::TUnitKeys unitKeys( dataKeys() );

      for( Diagnostic::TUnitKeys::iterator i( unitKeys.begin() ); i != unitKeys.end(); ++i )
      {
         if( data( *i )->synonym() == aSynonym )
         {
            ret = *i;

            break;
         }
      }

      return ret;
   }

   unsigned int CUnitsTable::dataCount()const
   {
      l2max::CMutexLocker locker( _mutex );

      return _unitsContainer.size();
   }

   HDataUnit CUnitsTable::data( const Diagnostic::CUnitKey& aKey )const
   {
      l2max::CMutexLocker locker( _mutex );
      
      TUnitContainer::const_iterator I_unit( _unitsContainer.find( aKey ) );

      CHECK_OBJECT_STATE( I_unit != _unitsContainer.end(), "Table has no such unit" );

      return I_unit->second;
   }

   Diagnostic::TUnitKeys CUnitsTable::dataKeys()const
   {
      l2max::CMutexLocker locker( _mutex );

      Diagnostic::TUnitKeys ret;

      for( TUnitContainer::const_iterator i( _unitsContainer.begin() ); i != _unitsContainer.end(); ++i )
      {
         ret.insert( i->first );
      }

      return ret;
   }

   bool CUnitsTable::hasData( const Diagnostic::CUnitKey& aKey )const
   {
      l2max::CMutexLocker locker( _mutex );

      return _unitsContainer.find( aKey ) != _unitsContainer.end();
   }

   void CUnitsTable::BuildControlTable( Diagnostic::HControlAdapter anAdapter )
   {
      l2max::CMutexLocker locker( _mutex );

      if( !anAdapter.IsNull() )
      {
         Diagnostic::TUnitKeys keys( anAdapter->keys() );

         for( Diagnostic::TUnitKeys::iterator i( keys.begin() ); i != keys.end(); ++i )
         {
            _controlsContainer[ *i ] = new CControlUnit( i->type(), anAdapter );
         }
      }
   }

   bool CUnitsTable::hasControl( const Diagnostic::CUnitKey& aKey )const
   {
      l2max::CMutexLocker locker( _mutex );

      return _controlsContainer.find( aKey ) != _controlsContainer.end();
   }

   unsigned int CUnitsTable::controlCount()const
   {
      l2max::CMutexLocker locker( _mutex );

      return _controlsContainer.size();
   }

   Diagnostic::TUnitKeys CUnitsTable::controlKeys()const
   {
      l2max::CMutexLocker locker( _mutex );

      Diagnostic::TUnitKeys ret;

      for( TControlContainer::const_iterator i( _controlsContainer.begin() ); i != _controlsContainer.end(); ++i )
      {
         ret.insert( i->first );
      }

      return ret;
   }

   CControlUnit& CUnitsTable::control( const Diagnostic::CUnitKey& aKey )
   {
      l2max::CMutexLocker locker( _mutex );

      CHECK_OBJECT_STATE( _controlsContainer.find( aKey ) != _controlsContainer.end(), "Table has no such unit" );

      return *_controlsContainer[ aKey ];
   }

   const std::string& CUnitsTable::registryNamespace()const
   {
      return _namespace;
   }
}
