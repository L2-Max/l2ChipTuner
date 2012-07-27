/*
 * PreferencePageFlowMeter.cpp
 *
 *  Created on: 20.10.2009
 *      Author: mars
 */

#include "PreferencePageFlowMeter.h"
#include "PreferencePageFlowMeterPetrol.h"
#include "PreferencePageFlowMeterGas.h"

#include <AppDefs.h>
#include <AppContext.h>

#include <l2Trace.h>

CPreferencePageFlowMeter::CPreferencePageFlowMeter() : CPreferencePage( "Flow Meter" )
{
   _fuelConfigurationPage = 0;
   
   setupUi( this );
   
   connect( _petrolRadioButton, SIGNAL( toggled( bool ) ), this, SLOT( OnPetrolToggled( bool ) ) );
   connect( _gasRadioButton, SIGNAL( toggled( bool ) ), this, SLOT( OnGasToggled( bool ) ) );
   
   l2max::CVariant fuelType;
   CAppContext::GetInstance()->MainRegistry().Read( V_ECU_FLOWMETER_FUELTYPE, fuelType );
   
   if( fuelType.toInt() == FlowMeter::ftGas )
   {
      _gasRadioButton->toggle();
   }
   else
   {
      _petrolRadioButton->toggle();
   }
}

CPreferencePageFlowMeter::~CPreferencePageFlowMeter()
{
   delete _fuelConfigurationPage;
}

void CPreferencePageFlowMeter::save()
{
   l2max::CVariant fuelType;
   
   if( _petrolRadioButton->isChecked() )
   {
      fuelType.SetInt( FlowMeter::ftPetrol );
   }
   else if( _gasRadioButton->isChecked() )
   {
      fuelType.SetInt( FlowMeter::ftGas );
   }
   
   CAppContext::GetInstance()->MainRegistry().Write( V_ECU_FLOWMETER_FUELTYPE, fuelType );
   
   if( _fuelConfigurationPage )
   {
      _fuelConfigurationPage->save();
   }
   
   CPreferencePage::save();
}

void CPreferencePageFlowMeter::ShowConfigurationPage( FlowMeter::EFuelType aType )
{
   if( _fuelConfigurationPage )
   {
      _fuelConfigurationPage->save();
      
      delete _fuelConfigurationPage;
      _fuelConfigurationPage = 0;
   }
   
   switch( aType )
   {
   case FlowMeter::ftPetrol:
      _fuelConfigurationPage = new CPreferencePageFlowMeterPetrol( _fuelOptionsGroupBox );
      break;
   case FlowMeter::ftGas:
      _fuelConfigurationPage = new CPreferencePageFlowMeterGas( _fuelOptionsGroupBox );
      break;
   }
   
   if( _fuelConfigurationPage )
   {
      _fuelConfigurationPage->show();
   }
}

void CPreferencePageFlowMeter::OnPetrolToggled( bool aChecked )
{
   TRACE_FUN( Routine, "CPreferencePageFlowMeter::OnPetrolToggled" );
   
   if( aChecked )
   {
      ShowConfigurationPage( FlowMeter::ftPetrol );
   }
}

void CPreferencePageFlowMeter::OnGasToggled( bool aChecked )
{
   TRACE_FUN( Routine, "CPreferencePageFlowMeter::OnGasToggled" );
   
   if( aChecked )
   {
      ShowConfigurationPage( FlowMeter::ftGas );
   }
}
