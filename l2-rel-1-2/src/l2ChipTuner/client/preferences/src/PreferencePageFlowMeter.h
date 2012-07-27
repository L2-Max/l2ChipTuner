/*
 * PreferencePageDeviceFlowMeter.h
 *
 *  Created on: 20.10.2009
 *      Author: mars
 */

#ifndef PREFERENCEPAGEFLOWMETER_H_
#define PREFERENCEPAGEFLOWMETER_H_

#include "PreferencePage.h"

#include <FlowMeterDefs.h>
#include <ui_PreferencePageFlowMeter.h>

struct IPreferenceOptionalPage;

class CPreferencePageFlowMeter : public CPreferencePage,
                                 public Ui_PreferencePageFlowMeter
{
   Q_OBJECT;
   
   IPreferenceOptionalPage* _fuelConfigurationPage;
   
   void ShowConfigurationPage( FlowMeter::EFuelType aType );

public:
   CPreferencePageFlowMeter();
   virtual ~CPreferencePageFlowMeter();
   
   virtual void save();

private slots:
   void OnPetrolToggled( bool aChecked );
   void OnGasToggled( bool aChecked );
};

#endif /* PREFERENCEPAGEFLOWMETER_H_ */
