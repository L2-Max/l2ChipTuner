/*
 * MainApplication.cpp
 *
 *  Created on: 28.09.2009
 *      Author: mars
 */

#include "MainApplication.h"

#include <l2Exception.h>

#include <QMessageBox>

#include <sstream>

CMainApplication::CMainApplication( int anArgc, char* anArgv[] ) : QApplication( anArgc, anArgv ){}
CMainApplication::~CMainApplication(){}

bool CMainApplication::notify( QObject* aReceiver, QEvent* anEvent )
{
   bool ret( false );
   
   try
   {
      ret = QApplication::notify( aReceiver, anEvent );
   }
   catch( const l2max::CException& anException )
   {
      std::ostringstream what;

      what << " *** Exception caught in CMainApplication::notify *** " << std::endl
           << anException.what() << std::endl;

      QMessageBox::critical( 0, "Exception", what.str().c_str() );
   }
   
   return ret;
}
