/*
 * PreferencePageAdapter.h
 *
 *  Created on: 16.10.2008
 *      Author: mars
 */

#ifndef PREFERENCEPAGEADAPTER_H_
#define PREFERENCEPAGEADAPTER_H_

#include "PreferencePage.h"

#include <ui_PreferencePageAdapter.h>

struct IPreferenceOptionalPage;

class CPreferencePageAdapter : public CPreferencePage,
                               public Ui_PreferencePageAdapter
{
   Q_OBJECT;

   IPreferenceOptionalPage* _deviceConfigurationPage;

public:
   CPreferencePageAdapter();
   virtual ~CPreferencePageAdapter();

   virtual void save();

private slots:
   void OnCommunicationDeviceChanged( int anIndex );
};

#endif /* PREFERENCEPAGEADAPTER_H_ */
