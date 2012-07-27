/*
 * DiagnosticControlAdapter.cpp
 *
 *  Created on: 02.02.2009
 *      Author: mars
 */

#include "DiagnosticControlAdapter.h"
#include "DiagnosticAbstractDataFormula.h"
#include "DiagnosticDataAdapter.h"

#include <l2MutexLocker.h>
#include <l2ExceptionThrower.h>
#include <l2Variant.h>

namespace Diagnostic
{
   CControlAdapter::CControlAdapter( CDataAdapter& anAdapter ) : _dataAdapter( anAdapter ){}
   CControlAdapter::~CControlAdapter(){}

   void CControlAdapter::SetValue( const l2max::CVariant& aValue, EDataType aType, EUnit anUnit )
   {
      l2max::CMutexLocker locker( _controlMutex );

      _rawDataMap.clear();

      if( hasKey( aType ) )
      {
         HAbstractDataFormula theFormula( _dataAdapter.formula( aType, anUnit ) );

         l2max::CVariant rawValue( aValue );

         if( !theFormula.IsNull() )
         {
            rawValue = theFormula->encode( rawValue );
         }

         _rawDataMap[ aType ] = rawValue.toInt();
      }
      else
      {
         CHECK_OBJECT_STATE( false, "There no such key in adapter" );
      }
   }

   void CControlAdapter::Synchronize()
   {
      l2max::CMutexLocker locker( _controlMutex );

      _unitKeys.clear();

      Synchronize( dtHiFan );
      Synchronize( dtCharPurge );
      Synchronize( dtClosedLoop );
      Synchronize( dtIACResetReq );
      Synchronize( dtDesiredEngineSpeed );
      Synchronize( dtAirFuel );
      Synchronize( dtSparcAdvance );
   }

   l2max::CVariant CControlAdapter::value( Diagnostic::EDataType aType )const
   {
      l2max::CVariant ret;

      TRawDataMap::const_iterator i( _rawDataMap.find( aType ) );

      if( i != _rawDataMap.end() )
      {
         ret = i->second;
      }

      return ret;
   }

   Diagnostic::TUnitKeys CControlAdapter::keys()const
   {
      l2max::CMutexLocker locker( _controlMutex );

      return _unitKeys;
   }

   void CControlAdapter::Synchronize( EDataType aType )
   {
      if( _dataAdapter.hasKey( aType ) )
      {
         _unitKeys.insert( aType );
      }
   }

   void CControlAdapter::Clear()
   {
      l2max::CMutexLocker locker( _controlMutex );

      _rawDataMap.clear();
   }

   bool CControlAdapter::operator == ( const CControlAdapter& anAdapter )const
   {
      return _rawDataMap == anAdapter._rawDataMap;
   }

   bool CControlAdapter::operator != ( const CControlAdapter& anAdapter )const
   {
      return !CControlAdapter::operator == ( anAdapter );
   }
}
