/*
 * PreferencePageProto.h
 *
 *  Created on: 26.10.2008
 *      Author: L2-Max
 */

#ifndef PREFERENCEPAGEPROTO_H_
#define PREFERENCEPAGEPROTO_H_

#include "PreferencePage.h"

#include <ui_PreferencePageProto.h>

struct IPreferenceOptionalPage;

class CPreferencePageProto : public CPreferencePage,
                             public Ui_PreferencePageProto
{
   Q_OBJECT;

   IPreferenceOptionalPage* _protocolConfigurationPage;

public:
   CPreferencePageProto();
   virtual ~CPreferencePageProto();

   virtual void save();

private slots:
   void OnCommunicationProtocolChanged( int anIndex );
};

#endif /* PREFERENCEPAGEPROTO_H_ */
