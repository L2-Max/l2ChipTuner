/*
 * PreferencePageStartUp.cpp
 *
 *  Created on: 20.10.2009
 *      Author: mars
 */

#include "PreferencePageStartUp.h"

#include <AppDefs.h>
#include <AppContext.h>

CPreferencePageStartUp::CPreferencePageStartUp() : CPreferencePage( "Start Up" )
{
   setupUi( this );
   
   l2max::CVariant startDiag;
   CAppContext::GetInstance()->MainRegistry().Read( V_STARTUP_STARTDIAGNOSTICSESSION, startDiag );
   
   _startDiagnosticSession->setCheckState( startDiag.toBool() ? Qt::Checked : Qt::Unchecked );
}

CPreferencePageStartUp::~CPreferencePageStartUp(){}

void CPreferencePageStartUp::save()
{
   CAppContext::GetInstance()->MainRegistry().Write( V_STARTUP_STARTDIAGNOSTICSESSION, _startDiagnosticSession->checkState() == Qt::Checked );
   
   CPreferencePage::save();
}
