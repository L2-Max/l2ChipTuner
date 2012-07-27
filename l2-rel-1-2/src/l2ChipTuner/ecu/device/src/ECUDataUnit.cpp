/*
 * DataUnit.cpp
 *
 *  Created on: 12.10.2008
 *      Author: L2-Max
 */

#include "ECUDataUnit.h"

#include <AppDefs.h>
#include <AppContext.h>

#include <DiagnosticDataAdapter.h>

#include <l2ExceptionThrower.h>
#include <l2Trace.h>
#include <l2MutexLocker.h>

#define V_ECU_UNIT_MEASUREU "::measureUnit"

namespace ECU
{
   CDataUnit::CDataUnit( Diagnostic::EDataType aType,
                         Diagnostic::HDataAdapter anAdapter,
                         const std::string& aNamespace ) :
      CAbstractUnit( aType ),
      _diagnosticDataAdapter( anAdapter ),
      _namespace( aNamespace ),
      _synonym( Diagnostic::dataDescriptionMap()->find( key().type() )->second._synonym ),
      _name( Diagnostic::dataDescriptionMap()->find( key().type() )->second._name ),
      _description( Diagnostic::dataDescriptionMap()->find( key().type() )->second._description )
   {
      l2max::CVariant measureUnitValue;

      CAppContext::GetInstance()->MainRegistry().Read( registryKeyName() + V_ECU_UNIT_MEASUREU, measureUnitValue );

      if( measureUnitValue.isValid() )
      {
         SetMeasureUnit( static_cast< Diagnostic::EUnit >( measureUnitValue.toInt() ) );
      }
      else
      {
         Diagnostic::EUnit theUnit( Diagnostic::uCounts );

         Diagnostic::TMeasureUnitsMap meatureUnitsMap( measureUnits() );

         for( Diagnostic::TMeasureUnitsMap::iterator i( meatureUnitsMap.begin() ); i != meatureUnitsMap.end(); ++i )
         {
            if( i->first != theUnit )
            {
               theUnit = i->first;

               break;
            }
         }

         SetMeasureUnit( theUnit );
      }
   }

   CDataUnit::~CDataUnit()
   {
      if( measureUnits().size() > 1 )
      {
         l2max::CVariant val;

         CAppContext::GetInstance()->MainRegistry().Write( registryKeyName() + V_ECU_UNIT_MEASUREU, measureUnit() );
      }
   }

   std::string CDataUnit::registryKeyName()const
   {
      l2max::CMutexLocker locker( _mutex );
      
      return S_ECU_UNIT + _namespace + "::" + synonym();
   }

   const std::string& CDataUnit::synonym()const
   {
      l2max::CMutexLocker locker( _mutex );
      
      return _synonym;
   }

   const std::string& CDataUnit::name()const
   {
      l2max::CMutexLocker locker( _mutex );
      
      return _name;
   }

   const std::string& CDataUnit::description()const
   {
      l2max::CMutexLocker locker( _mutex );
      
      return _description;
   }

   std::string CDataUnit::text()const
   {
      l2max::CMutexLocker locker( _mutex );
      
      std::string ret( "nothing to display" );

      l2max::CVariant theValue( value() );
      
      if( theValue.isValid() )
      {
         ret = theValue.toString();

         std::string theMeasureUnitText( measureUnitSymbol() );

         if( theMeasureUnitText.size() )
         {
            ret += " " + theMeasureUnitText;
         }
      }
      
      return ret;
   }

   l2max::CVariant CDataUnit::value()const
   {
      l2max::CMutexLocker locker( _mutex );
      
      return value( measureUnit() );
   }

   l2max::CVariant CDataUnit::value( Diagnostic::EUnit anUnit )const
   {
      l2max::CMutexLocker locker( _mutex );
      
      return _diagnosticDataAdapter->value( key().type(), anUnit );
   }

   void CDataUnit::SetMeasureUnit( Diagnostic::EUnit anUnit )
   {
      l2max::CMutexLocker locker( _mutex );
      
      _defaultUnit = anUnit;
   }

   Diagnostic::EUnit CDataUnit::measureUnit()const
   {
      l2max::CMutexLocker locker( _mutex );
      
      return _defaultUnit;
   }

   Diagnostic::TMeasureUnitsMap CDataUnit::measureUnits()const
   {
      l2max::CMutexLocker locker( _mutex );
      
      Diagnostic::TMeasureUnitsMap ret( _diagnosticDataAdapter->measureUnits( key().type() ) );

      ret[ Diagnostic::uCounts ] = Diagnostic::pNoPrefix;

      return ret;
   }

   std::string CDataUnit::measureUnitName()const
   {
      l2max::CMutexLocker locker( _mutex );
      
      Diagnostic::TMeasureUnitsMap measureUnitsMap( measureUnits() );
      Diagnostic::TMeasureUnitsMap::iterator I_unit( measureUnitsMap.find( _defaultUnit ) );

      CHECK_OBJECT_STATE( I_unit != measureUnitsMap.end(), "Meausere unit should be registered in data adapter" );

      return Diagnostic::unitDescriptionMap()->find( I_unit->first )->second._description;
   }

   std::string CDataUnit::measureUnitSymbol()const
   {
      l2max::CMutexLocker locker( _mutex );
      
      std::string ret;

      Diagnostic::TMeasureUnitsMap measureUnitsMap( measureUnits() );

      Diagnostic::TMeasureUnitsMap::iterator i( measureUnitsMap.find( _defaultUnit ) );

      CHECK_OBJECT_STATE( i != measureUnitsMap.end(), "Meausere unit should be registered in data adapter" );

      if( i->first != Diagnostic::uCounts )
      {
         ret += Diagnostic::prefixDescriptionMap()->find( i->second )->second._symbol;
         ret += Diagnostic::unitDescriptionMap()->find( i->first )->second._symbol;
      }

      return ret;
   }
   
   bool CDataUnit::IsChanged()const
   {
      l2max::CMutexLocker locker( _mutex );
      
      return _diagnosticDataAdapter->IsChanged( key().type() );
   }
   
   Diagnostic::HAbstractDataFormula CDataUnit::formula()const
   {
      l2max::CMutexLocker locker( _mutex );
      
      return formula( measureUnit() );
   }
   
   Diagnostic::HAbstractDataFormula CDataUnit::formula( Diagnostic::EUnit anUnit )const
   {
      l2max::CMutexLocker locker( _mutex );
      
      return _diagnosticDataAdapter->formula( key().type(), anUnit );
   }
}
