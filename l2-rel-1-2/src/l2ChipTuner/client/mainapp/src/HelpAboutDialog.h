/*
 * HelpAboutDialog.h
 *
 *  Created on: 22.02.2009
 *      Author: L2-Max
 */

#ifndef HELPABOUTDIALOG_H_
#define HELPABOUTDIALOG_H_

#include <ui_HelpAbout.h>

#include <QDialog>

class CHelpAboutDialog : public QDialog,
                         public Ui_HelpAbout
{
public:
   CHelpAboutDialog( QWidget* aParent );
   virtual ~CHelpAboutDialog();
};

#endif /* HELPABOUTDIALOG_H_ */
