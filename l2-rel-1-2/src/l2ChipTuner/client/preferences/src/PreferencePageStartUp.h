/*
 * PreferencePageStartUp.h
 *
 *  Created on: 20.10.2009
 *      Author: mars
 */

#ifndef PREFERENCEPAGESTARTUP_H_
#define PREFERENCEPAGESTARTUP_H_

#include "PreferencePage.h"

#include <ui_PreferencePageStartUp.h>

class CPreferencePageStartUp : public CPreferencePage,
                               public Ui_PreferencePageStartUp
{
public:
   CPreferencePageStartUp();
   virtual ~CPreferencePageStartUp();
   
   virtual void save();
};

#endif /* PREFERENCEPAGESTARTUP_H_ */
