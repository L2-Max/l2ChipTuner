/*
 * PreferencePageSession.h
 *
 *  Created on: 04.01.2009
 *      Author: L2-Max
 */

#ifndef PREFERENCEPAGESESSION_H_
#define PREFERENCEPAGESESSION_H_

#include "PreferencePage.h"

#include <ui_PreferencePageSession.h>

class CPreferencePageSession : public CPreferencePage,
                               public Ui_PreferencePageSession
{
   Q_OBJECT;

   virtual void save();

   void RefreshDirectory();

public:
   CPreferencePageSession();
   virtual ~CPreferencePageSession();

private slots:
   void OnDirectoryBrowse();
   void OnReadFrom( int aState );
   void OnWriteTo( int aState );
   void OnAuto( int aState );
};

#endif /* PREFERENCEPAGESESSION_H_ */
