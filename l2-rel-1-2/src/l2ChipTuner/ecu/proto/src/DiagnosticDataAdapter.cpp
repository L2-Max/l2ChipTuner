/*
 * DiagnosticDataAdapter.cpp
 *
 *  Created on: 25.10.2008
 *      Author: L2-Max
 */

#include "DiagnosticDataAdapter.h"

#include <l2Variant.h>
#include <l2MutexLocker.h>

namespace Diagnostic
{
   CDataAdapter::CDataAdapter(){}
   CDataAdapter::~CDataAdapter(){}

   l2max::CVariant CDataAdapter::value( EDataType aType, EUnit anUnit )const
   {
      l2max::CMutexLocker locker( _dataMutex );

      l2max::CVariant ret;
      
      if( anUnit == Diagnostic::uCounts )
      {
         ret = _rawDataMap.find( aType )->second;
      }
      else
      {
         TMeasureUnitsMap theMeasureUnitsMap( measureUnits( aType ) );
         
         if( theMeasureUnitsMap.find( anUnit ) != theMeasureUnitsMap.end() )
         {
            ret = _rawDataMap.find( aType )->second;
   
            HAbstractDataFormula theFormula( formula( aType, anUnit ) );
      
            if( !theFormula.IsNull() )
            {
               ret = theFormula->decode( ret );
            }
            else if( anUnit == Diagnostic::uBinary )
            {
               ret.SetType( l2max::CVariant::tBool );
            }
         }
      }
      
      return ret;
   }

   TMeasureUnitsMap CDataAdapter::measureUnits( EDataType aType )const
   {
      l2max::CMutexLocker locker( _dataMutex );

      TMeasureUnitsMap ret;
      TDataMeasureUnitsMap::const_iterator res( _dataMeasureUnitsMap.find( aType ) );

      if( res != _dataMeasureUnitsMap.end() )
      {
         ret = res->second;
      }

      return ret;
   }

   TUnitKeys CDataAdapter::keys()const
   {
      l2max::CMutexLocker locker( _dataMutex );

      TUnitKeys ret;

      for( TRawDataMap::const_iterator i( _rawDataMap.begin() ); i != _rawDataMap.end(); ++i )
      {
         ret.insert( i->first );
      }

      return ret;
   }

   HAbstractDataFormula CDataAdapter::formula( EDataType aType, EUnit anUnit )const
   {
      l2max::CMutexLocker locker( _dataMutex );

      HAbstractDataFormula ret;

      TFormulaMap::const_iterator res( _formulaMap.find( aType ) );

      if( res != _formulaMap.end() )
      {
         const TFormulaUnitMap& formulaUnitMap( res->second );

         TFormulaUnitMap::const_iterator res( formulaUnitMap.find( anUnit ) );

         if( res != formulaUnitMap.end() )
         {
            ret = res->second;
         }
      }

      return ret;
   }
   
   void CDataAdapter::DataChanged( const TRawDataMap& aData )
   {
      l2max::CMutexLocker locker( _dataMutex );

      _dataChangeContainer.clear();
      
      for( TRawDataMap::const_iterator I_data( aData.begin() ); I_data != aData.end(); ++I_data )
      {
         TRawDataMap::iterator I_raw( _rawDataMap.insert( *I_data ).first );
         
         if( I_raw->second != I_data->second )
         {
            _dataChangeContainer.insert( I_raw->first );
            
            I_raw->second = I_data->second;
         }
      }
   }
   
   bool CDataAdapter::IsChanged( EDataType aType )const
   {
      l2max::CMutexLocker locker( _dataMutex );

      return _dataChangeContainer.find( aType ) != _dataChangeContainer.end();
   }
}
