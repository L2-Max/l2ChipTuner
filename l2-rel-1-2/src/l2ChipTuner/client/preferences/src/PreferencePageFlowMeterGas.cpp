/*
 * PreferencePageFlowMeterGas.cpp
 *
 *  Created on: 23.10.2009
 *      Author: mars
 */

#include "PreferencePageFlowMeterGas.h"
#include "DoubleValidator.h"

#include <AppDefs.h>
#include <AppContext.h>

CPreferencePageFlowMeterGas::CPreferencePageFlowMeterGas( QWidget* aParent ) : QWidget( aParent )
{
   setupUi( this );
   
   parentWidget()->setMinimumSize( size() );
   
   l2max::CVariant literWeight;
   CAppContext::GetInstance()->MainRegistry().Read( V_ECU_FLOWMETER_GASLITERWEIGHT, literWeight );
   literWeight.SetPrecission( 3 );
   
   _literWeightLineEdit->setValidator( new CDoubleValidator( .1, .999, 3, _literWeightLineEdit ) );
   _literWeightLineEdit->setText( literWeight.toString().c_str() );
   
   
   l2max::CVariant lambda;
   CAppContext::GetInstance()->MainRegistry().Read( V_ECU_FLOWMETER_GASLAMBDA, lambda );
   lambda.SetPrecission( 1 );
   
   _lambdaLineEdit->setValidator( new CDoubleValidator( 1., 20., 1, _lambdaLineEdit ) );
   _lambdaLineEdit->setText( lambda.toString().c_str() );
}

CPreferencePageFlowMeterGas::~CPreferencePageFlowMeterGas(){}

void CPreferencePageFlowMeterGas::save()
{
   l2max::CVariant literWeight( _literWeightLineEdit->text().toStdString() );
   CAppContext::GetInstance()->MainRegistry().Write( V_ECU_FLOWMETER_GASLITERWEIGHT, literWeight.toDouble() );
   
   l2max::CVariant lambda( _lambdaLineEdit->text().toStdString() );
   CAppContext::GetInstance()->MainRegistry().Write( V_ECU_FLOWMETER_GASLAMBDA, lambda.toDouble() );
}

void CPreferencePageFlowMeterGas::show()
{
   QWidget::show();
}
