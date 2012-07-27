/*
 * PreferencePageFlowMeterGas.h
 *
 *  Created on: 23.10.2009
 *      Author: mars
 */

#ifndef PREFERENCEPAGEFLOWMETERGAS_H_
#define PREFERENCEPAGEFLOWMETERGAS_H_

#include "IPreferenceOptionalPage.h"

#include <Ui_PreferencePageFlowMeterGas.h>

class CPreferencePageFlowMeterGas : public QWidget,
                                    public Ui_PreferencePageFlowMeterGas,
                                    public IPreferenceOptionalPage
{
public:
   CPreferencePageFlowMeterGas( QWidget* aParent );
   virtual ~CPreferencePageFlowMeterGas();
   
   virtual void save();
   virtual void show();
};

#endif /* PREFERENCEPAGEFLOWMETERGAS_H_ */
