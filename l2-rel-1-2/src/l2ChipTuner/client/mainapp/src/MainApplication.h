/*
 * MainApplication.h
 *
 *  Created on: 28.09.2009
 *      Author: mars
 */

#ifndef MAINAPPLICATION_H_
#define MAINAPPLICATION_H_

#include <QApplication>

class CMainApplication : public QApplication
{
public:
   CMainApplication( int anArgc, char* anArgv[] );
   virtual ~CMainApplication();
   
   bool notify( QObject* aReceiver, QEvent* anEvent );
};

#endif /* MAINAPPLICATION_H_ */
