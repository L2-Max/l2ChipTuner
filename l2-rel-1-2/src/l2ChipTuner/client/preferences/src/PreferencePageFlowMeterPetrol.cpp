/*
 * PreferencePageFlowMeterPetrol.cpp
 *
 *  Created on: 23.10.2009
 *      Author: mars
 */

#include "PreferencePageFlowMeterPetrol.h"
#include "DoubleValidator.h"

#include <AppDefs.h>
#include <AppContext.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CPreferencePageFlowMeterPetrol::CPreferencePageFlowMeterPetrol( QWidget* aParent ) : QWidget( aParent )
{
   setupUi( this );
   
   parentWidget()->setMinimumSize( size() );
   
   _injectorCalibrationLineEdit->setValidator( new CDoubleValidator( 1., 500., 1, _injectorCalibrationLineEdit ) );
   
   l2max::CVariant injCal;
   CAppContext::GetInstance()->MainRegistry().Read( V_ECU_FLOWMETER_INJCAL, injCal );
   
   injCal.SetPrecission( 1 );
   
   _injectorCalibrationLineEdit->setText( injCal.toString().c_str() );
}

CPreferencePageFlowMeterPetrol::~CPreferencePageFlowMeterPetrol(){}

void CPreferencePageFlowMeterPetrol::save()
{
   l2max::CVariant injCal( _injectorCalibrationLineEdit->text().toStdString() );
   CAppContext::GetInstance()->MainRegistry().Write( V_ECU_FLOWMETER_INJCAL, injCal.toDouble() );
}

void CPreferencePageFlowMeterPetrol::show()
{
   QWidget::show();
}
