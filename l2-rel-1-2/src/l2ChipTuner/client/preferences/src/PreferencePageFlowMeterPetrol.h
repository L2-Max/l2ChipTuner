/*
 * PreferencePageFlowMeterPetrol.h
 *
 *  Created on: 23.10.2009
 *      Author: mars
 */

#ifndef PREFERENCEPAGEFLOWMETERPETROL_H_
#define PREFERENCEPAGEFLOWMETERPETROL_H_

#include "IPreferenceOptionalPage.h"

#include <Ui_PreferencePageFlowMeterPetrol.h>

class CPreferencePageFlowMeterPetrol : public QWidget,
                                       public Ui_PreferencePageFlowMeterPetrol,
                                       public IPreferenceOptionalPage
{
public:
   CPreferencePageFlowMeterPetrol( QWidget* aParent );
   virtual ~CPreferencePageFlowMeterPetrol();
   
   virtual void save();
   virtual void show();
};

#endif /* PREFERENCEPAGEFLOWMETERPETROL_H_ */
