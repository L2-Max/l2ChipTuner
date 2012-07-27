/*
 * main.cpp
 *
 *  Created on: 02.10.2008
 *      Author: L2-Max
 */

#include "AppContext.h"
#include "MainWindow.h"
#include "MainApplication.h"

#include <QPlastiqueStyle>
#include <QMessageBox>

#include <l2RegistryDriverException.h>
#include <l2Trace.h>

#include <sstream>

int main( int anArgc, char* anArgv[] )
{
   int ret( 0 );

   CAppContext ctx( anArgc, anArgv );

   CMainApplication a( anArgc, anArgv );

   CMainApplication::setStyle( new QPlastiqueStyle() );
   CMainApplication::setWindowIcon( QIcon( ":/Main/logo_l2ChipTuner.png" ) );

   try
   {
      CMainWindow mainWindow;
      mainWindow.show();

      ret = a.exec();
   }
   catch( const l2max::CException& anException )
   {
      std::ostringstream what;

      what << " *** Exception *** " << std::endl
           << anException.what() << std::endl;

      QMessageBox::critical( 0, "Exception", what.str().c_str() );
   }
   catch( ... )
   {
      TraceCritical << " *** UNKNOWN EXCEPTION *** " << std::endl;
   }

   return ret;
}
