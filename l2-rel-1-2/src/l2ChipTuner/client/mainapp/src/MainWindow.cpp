/*
 * MainWindow.cpp
 *
 *  Created on: 03.10.2008
 *      Author: mars
 */

#include "MainWindow.h"
#include "MenuBarManager.h"
#include "ToolBarManager.h"
#include "ECUDeviceView.h"
#include "HelpAboutDialog.h"

#include <PreferencesDialog.h>
#include <PreferencePageAdapter.h>
#include <PreferencePageProto.h>
#include <PreferencePageSession.h>
#include <PreferencePageFlowMeter.h>
#include <PreferencePageStartUp.h>
#include <AppDefs.h>
#include <AppContext.h>

#include <l2Trace.h>

#include <QMenuBar>
#include <QStatusBar>
#include <QLabel>
#include <QCloseEvent>

CMainWindow::CMainWindow()
{
   _closeRequired = false;

   _menuBarManager = new CMenuBarManager( *menuBar() );
   _toolBarManager = new CToolBarManager( this );

   RestorePosition();

   _deviceStatus = new QLabel( "No communication", this );
   _deviceStatus->setContentsMargins( 1, 0, 1, 0 );

   _communicationStatus = new QLabel( this );
   _communicationStatus->setContentsMargins( 1, 0, 1, 0 );

   statusBar()->addPermanentWidget( _communicationStatus );
   statusBar()->addPermanentWidget( _deviceStatus );

   _menuSessionId = menuBarManager().OpenSession();
   
   CreateMenu();
   CreateECUDeviceView();
}

CMainWindow::~CMainWindow()
{
   TRACE_FUN( Routine, "CMainWindow::~CMainWindow" );

   menuBarManager().CloseSession( _menuSessionId );
   
   delete _toolBarManager;
   delete _menuBarManager;
}

void CMainWindow::closeEvent( QCloseEvent* anEvent )
{
   TRACE_FUN( Routine, "CMainWindow::closeEvent" );

   bool allViewsDestroyed( true );

   _closeRequired = true;

   if( _ecuDeviceView->IsReadyToClose() )
   {
      delete _ecuDeviceView;
      _ecuDeviceView = 0;
   }
   else
   {
      allViewsDestroyed = false;

      _ecuDeviceView->close();
   }

   if( allViewsDestroyed )
   {
      SavePosition();

      QMainWindow::closeEvent( anEvent );
   }
   else
   {
      anEvent->ignore();
   }
}

void CMainWindow::RestorePosition()
{
   l2max::CVariant windowX;
   l2max::CVariant windowY;
   l2max::CVariant windowWidth;
   l2max::CVariant windowHeight;
   l2max::CVariant isMaximized;
   l2max::CVariant isMinimized;

   CAppContext::GetInstance()->MainRegistry().Read( V_MAINWND_X, windowX );
   CAppContext::GetInstance()->MainRegistry().Read( V_MAINWND_Y, windowY );
   CAppContext::GetInstance()->MainRegistry().Read( V_MAINWND_WIDTH, windowWidth );
   CAppContext::GetInstance()->MainRegistry().Read( V_MAINWND_HEIGHT, windowHeight );
   CAppContext::GetInstance()->MainRegistry().Read( V_MAINWND_ISMAXI, isMaximized );
   CAppContext::GetInstance()->MainRegistry().Read( V_MAINWND_ISMINI, isMinimized );

   if( windowX.toInt() != -1 && windowY.toInt() != -1 )
   {
      move( windowX.toInt(), windowY.toInt() );
   }

   if( windowWidth.toInt() != -1 && windowHeight.toInt() != -1 )
   {
      resize( windowWidth.toInt(), windowHeight.toInt() );
   }

   if( isMaximized.toInt() )
   {
      showMaximized();
   }
   else if( isMinimized.toInt() )
   {
      showMinimized();
   }
}

void CMainWindow::SavePosition()const
{
   if( !isMaximized() && !isMinimized() )
   {
      CAppContext::GetInstance()->MainRegistry().Write( V_MAINWND_X, x() );
      CAppContext::GetInstance()->MainRegistry().Write( V_MAINWND_Y, y() );
      CAppContext::GetInstance()->MainRegistry().Write( V_MAINWND_WIDTH, width() );
      CAppContext::GetInstance()->MainRegistry().Write( V_MAINWND_HEIGHT, height() );
   }

   CAppContext::GetInstance()->MainRegistry().Write( V_MAINWND_ISMAXI, isMaximized() );
   CAppContext::GetInstance()->MainRegistry().Write( V_MAINWND_ISMINI, isMinimized() );
}

void CMainWindow::CreateMenu()
{
   ///////////////////////////////////////////////////////////////////////////////////////////////
   _menuBarManager->ShowMenu( CMenuBarManager::mMainPreferences, _menuSessionId );
   _menuBarManager->ShowMenu( CMenuBarManager::mMainExit, _menuSessionId );

   _menuBarManager->Connect( CMenuBarManager::mMainPreferences, this, SLOT( OnPreferences() ) );
   _menuBarManager->Connect( CMenuBarManager::mMainExit, this, SLOT( close() ) );

   ///////////////////////////////////////////////////////////////////////////////////////////////
   _menuBarManager->ShowMenu( CMenuBarManager::mHelpAbout, _menuSessionId );

   _menuBarManager->Connect( CMenuBarManager::mHelpAbout, this, SLOT( OnAbout() ) );
}

void CMainWindow::CreateECUDeviceView()
{
   _ecuDeviceView = new CECUDeviceView( this );

   setCentralWidget( _ecuDeviceView );
}

void CMainWindow::OnPreferences()
{
   CPreferencesDialog preferencesDialog( this );

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
   CPreferencePageContainer* communicationsPageContainer( preferencesDialog.AddPage( "Communications" ) );
   communicationsPageContainer->AddPage( new CPreferencePageAdapter() );
   communicationsPageContainer->AddPage( new CPreferencePageProto() );

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
   CPreferencePageContainer* ecuPageContainer( preferencesDialog.AddPage( "ECU" ) );
   CPreferencePageContainer* diagnosticPageContainer( ecuPageContainer->AddPage( "Diagnostic" ) );
   
   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
   diagnosticPageContainer->AddPage( new CPreferencePageSession() );
   
   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
   ecuPageContainer->AddPage( new CPreferencePageFlowMeter() );
   
   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
   preferencesDialog.AddPage( new CPreferencePageStartUp() );

   preferencesDialog.exec();
}

QLabel& CMainWindow::deviceStatus()const
{
   return *_deviceStatus;
}

QLabel& CMainWindow::communicationStatus()const
{
   return *_communicationStatus;
}

CMenuBarManager& CMainWindow::menuBarManager()const
{
   return *_menuBarManager;
}

void CMainWindow::OnAbout()
{
   CHelpAboutDialog( this ).exec();
}

void CMainWindow::CloseIfRequired()
{
   TRACE_FUN( Routine, "CMainWindow::CloseIfRequired" );

   if( _closeRequired )
   {
      close();
   }
}
