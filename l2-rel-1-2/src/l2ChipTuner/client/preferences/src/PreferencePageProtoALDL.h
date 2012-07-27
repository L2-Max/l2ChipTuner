/*
 * PreferencePageProtoALDL.h
 *
 *  Created on: 25.03.2009
 *      Author: L2-Max
 */

#ifndef PREFERENCEPAGEPROTOALDL_H_
#define PREFERENCEPAGEPROTOALDL_H_

#include "IPreferenceOptionalPage.h"

#include <Ui_PreferencePageALDLProtocol.h>

class CPreferencePageProtoALDL : public QWidget,
                                 public Ui_PreferencePageALDLProtocol,
                                 public IPreferenceOptionalPage
{
   Q_OBJECT;

public:
   CPreferencePageProtoALDL( QWidget* aParent );
   virtual ~CPreferencePageProtoALDL();

   virtual void save();
   virtual void show();

private slots:
   void OnModelChanged( int anIndex );
   void OnEngineChanged( int anIndex );
};

#endif /* PREFERENCEPAGEPROTOALDL_H_ */
