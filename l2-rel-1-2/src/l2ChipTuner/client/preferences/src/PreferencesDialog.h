/*
 * PreferencesDialog.h
 *
 *  Created on: 15.10.2008
 *      Author: L2-Max
 */

#ifndef PREFERENCESDIALOG_H_
#define PREFERENCESDIALOG_H_

#include "PreferencePageContainer.h"

#include <ui_PreferencesDialog.h>

#include <QDialog>

class CPreferencesDialog : public QDialog,
                           public CPreferencePageContainer,
                           public Ui_PreferenceDialog
{
   Q_OBJECT;

   CPreferencePage* _currPreferencePage;

   TPagesPath PagesReversedPath( QTreeWidgetItem* anItem );

   virtual void accept();

public:
   CPreferencesDialog( QWidget* aParent );
   virtual ~CPreferencesDialog();

   int exec();

private slots:
   void OnItemSelectionChanged();
};

#endif /* PREFERENCESDIALOG_H_ */
