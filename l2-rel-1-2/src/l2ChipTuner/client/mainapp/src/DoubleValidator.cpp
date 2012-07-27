/*
 * DoubleValidator.cpp
 *
 *  Created on: 23.10.2009
 *      Author: mars
 */

#include "DoubleValidator.h"

CDoubleValidator::CDoubleValidator( double aMin, double aMax, int aDidgit, QObject* aParent ) :
   QDoubleValidator( aMin, aMax, aDidgit, aParent )
{
   setNotation( QDoubleValidator::StandardNotation );
}

QValidator::State CDoubleValidator::validate( QString& anInput, int& aPos )const
{
   QValidator::State ret( QDoubleValidator::validate( anInput, aPos ) );
   
   if( ret == QValidator::Intermediate )
   {
      ret = QValidator::Invalid;
   }
   
   return ret;
}
