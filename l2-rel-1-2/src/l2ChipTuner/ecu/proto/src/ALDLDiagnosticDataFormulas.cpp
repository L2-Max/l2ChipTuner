/*
 * ALDLDiagnosticDataFormulas.cpp
 *
 *  Created on: 20.11.2008
 *      Author: mars
 */

#include "ALDLDiagnosticDataFormulas.h"

#include <l2Variant.h>
#include <l2Trace.h>

namespace ALDL
{
   ////////////////////////////////////////////////////////////////////////////////////
   CBasicFormula::CBasicFormula( double aScaling, unsigned char aPrecission, int anOffset, double aBase )
   {
      _scaling = aScaling;
      _precission = aPrecission;
      _offset = anOffset;
      _base = aBase;
   }

   CBasicFormula::~CBasicFormula(){}

   l2max::CVariant CBasicFormula::encode( const l2max::CVariant& aValue )const
   {
      return l2max::CVariant( ( aValue.toDouble() - _offset ) / _scaling - _base );
   }
   
   l2max::CVariant CBasicFormula::decode( const l2max::CVariant& aValue )const
   {
      l2max::CVariant ret( ( aValue.toDouble() + _base ) * _scaling + _offset );

      if( _precission )
      {
         ret.SetPrecission( _precission );
      }
      else
      {
         ret = ret.toInt();
      }
      
      return ret;
   }

   void CBasicFormula::SetScaling( double aScaling )
   {
      _scaling = aScaling;
   }

   void CBasicFormula::SetBase( double aBase )
   {
      _base = aBase;
   }
}
