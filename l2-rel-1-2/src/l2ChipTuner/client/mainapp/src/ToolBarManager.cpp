/*
 * ToolBarManager.cpp
 *
 *  Created on: 25.02.2009
 *      Author: L2-Max
 */

#include "ToolBarManager.h"
#include "MainWindow.h"
#include "MenuBarManager.h"

#include <QToolBar>
#include <QAction>

CToolBarManager::CToolBarManager( CMainWindow* aParent ) : _mainWindow( *aParent )
{
   CMenuBarManager& menuBarManager( aParent->menuBarManager() );

   ///////////////////////////////////////////////////////////////////////////////
   QToolBar* mainToolBar( new QToolBar( "Main Tool Bar", &_mainWindow ) );

   mainToolBar->addAction( menuBarManager.menu( CMenuBarManager::mMainPreferences ) );

   _toolBarMap[ tbMain ] = mainToolBar;

   ///////////////////////////////////////////////////////////////////////////////
   QToolBar* viewToolBar( new QToolBar( "View Tool Bar", &_mainWindow ) );

   viewToolBar->addAction( menuBarManager.menu( CMenuBarManager::mViewUnits ) );
   viewToolBar->addAction( menuBarManager.menu( CMenuBarManager::mViewError ) );
   viewToolBar->addAction( menuBarManager.menu( CMenuBarManager::mViewFlowMeter ) );
   viewToolBar->addAction( menuBarManager.menu( CMenuBarManager::mViewGraph ) );

   _toolBarMap[ tbView ] = viewToolBar;

   ///////////////////////////////////////////////////////////////////////////////
   QToolBar* deviceToolBar( new QToolBar( "Device Tool Bar", &_mainWindow ) );

   deviceToolBar->addAction( menuBarManager.menu( CMenuBarManager::mDeviceStartStopDiagnostic ) );
   deviceToolBar->addAction( menuBarManager.menu( CMenuBarManager::mDevicePeriodicTrasmission ) );
   deviceToolBar->addAction( menuBarManager.menu( CMenuBarManager::mDeviceSnapshot ) );

   _toolBarMap[ tbDevice ] = deviceToolBar;

   for( TToolBarMap::iterator i( _toolBarMap.begin() ); i != _toolBarMap.end(); ++i )
   {
      i->second->setIconSize( QSize( 20, 20 ) );
      i->second->hide();
   }

   connect( &menuBarManager, SIGNAL( menuShowHideEvent() ), SLOT( OnMenuShowHide() ) );

   OnMenuShowHide();
}

CToolBarManager::~CToolBarManager(){}

void CToolBarManager::OnMenuShowHide()
{
   QToolBar* prevToolBar( 0 );

   for( TToolBarMap::reverse_iterator i( _toolBarMap.rbegin() ); i != _toolBarMap.rend(); ++i )
   {
      QList< QAction* > actions( i->second->actions() );

      bool hasVisibleActions( false );

      for( QList< QAction* >::iterator j( actions.begin() ); j != actions.end(); ++j )
      {
         if( ( *j )->isVisible() )
         {
            hasVisibleActions = true;

            break;
         }
      }

      i->second->setVisible( hasVisibleActions );

      if( !hasVisibleActions )
      {
         _mainWindow.removeToolBar( i->second );
      }
      else
      {
         if( prevToolBar )
         {
            _mainWindow.insertToolBar( prevToolBar, i->second );
         }
         else
         {
            _mainWindow.addToolBar( i->second );
         }

         prevToolBar = i->second;
      }
   }
}
