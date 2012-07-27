/*
 * MenuBarManager.cpp
 *
 *  Created on: 11.12.2008
 *      Author: mars
 */

#include "MenuBarManager.h"

#include <l2Trace.h>
#include <l2ExceptionThrower.h>

#include <QMenuBar>

const CMenuBarManager::TSessionId CMenuBarManager::_invalidSessionId( static_cast< TSessionId >( -1 ) );

CMenuBarManager::CMenuBarManager( QMenuBar& aMenuBar ) : _menuBar( aMenuBar )
{
   TRACE_FUN( Routine, "CMenuBarManager::CMenuBarManager" );
   
   //////////////////////////////////////////////////////////////////////////////////////////////////////
   _menuActionMap[ mMain ] = TMenuRelationship( _menuBar.addMenu( "Main" )->menuAction(), &_menuBar );
   _menuActionMap[ mMainPreferences ] = TMenuRelationship( _menuActionMap[ mMain ].first->menu()->
                                                              addAction( QIcon( ":/Menu/Main/menuMainPreferences.png" ),
                                                                         "Preferences" ),
                                                           _menuActionMap[ mMain ].first );
   _menuActionMap[ mMain ].first->menu()->addSeparator();
   _menuActionMap[ mMainExit ] = TMenuRelationship( _menuActionMap[ mMain ].first->menu()->addAction( "Exit" ),
                                                    _menuActionMap[ mMain ].first );

   //////////////////////////////////////////////////////////////////////////////////////////////////////
   _menuActionMap[ mView ] = TMenuRelationship( _menuBar.addMenu( "View" )->menuAction(), &_menuBar );

   _menuActionMap[ mViewUnits ] = TMenuRelationship( _menuActionMap[ mView ].first->menu()->
                                                        addAction( QIcon( ":/Menu/View/menuViewMonitor.png" ),
                                                                   "ECU Units" ),
                                                     _menuActionMap[ mView ].first );
   
   _menuActionMap[ mViewError ] = TMenuRelationship( _menuActionMap[ mView ].first->menu()->
                                                        addAction( QIcon( ":/Menu/View/menuViewError.png" ),
                                                                   "ECU Error" ),
                                                     _menuActionMap[ mView ].first );

   _menuActionMap[ mViewFlowMeter ] = TMenuRelationship( _menuActionMap[ mView ].first->menu()->
                                                            addAction( QIcon( ":/Menu/View/menuViewFlowMeter.png" ),
                                                                       "Flow Meter" ),
                                                         _menuActionMap[ mView ].first );
   
   //////////////////////////////////////////////////////////////////////////////////////////////////////
   _menuActionMap[ mViewGraph ] = TMenuRelationship( _menuActionMap[ mView ].first->menu()->
                                                        addAction( QIcon( ":/Menu/Meter/menuMeterGraphView.png" ),
                                                                   "Graph" ),
                                                     _menuActionMap[ mView ].first );
   
   //////////////////////////////////////////////////////////////////////////////////////////////////////
   _menuActionMap[ mViewPacketSelection ] = TMenuRelationship( _menuActionMap[ mView ].first->menu()->
                                                                    addAction( QIcon( ":/Menu/View/menuViewPacketSelection.png" ),
                                                                               "Packet selection" ),
                                                               _menuActionMap[ mView ].first );

   _menuActionMap[ mView ].first->menu()->addSeparator();
   _menuActionMap[ mViewDataUnits ] = TMenuRelationship( _menuActionMap[ mView ].first->menu()->addAction( "Data Units View" ),
                                                         _menuActionMap[ mView ].first );

   //////////////////////////////////////////////////////////////////////////////////////////////////////
   _menuActionMap[ mDevice ] = TMenuRelationship( _menuBar.addMenu( "Device" )->menuAction(), &_menuBar );
   _menuActionMap[ mDeviceStartStopDiagnostic ] = TMenuRelationship( _menuActionMap[ mDevice ].first->menu()->
                                                                        addAction( QIcon( ":/Menu/Device/menuDeviceStartDiagnosticSession.png" ),
                                                                                   "Start Diagnostic Session" ),
                                                                     _menuActionMap[ mDevice ].first );

   _menuActionMap[ mDevice ].first->menu()->addSeparator();

   _menuActionMap[ mDevicePeriodicTrasmission ] = TMenuRelationship( _menuActionMap[ mDevice ].first->menu()->
                                                                        addAction( QIcon( ":/Menu/Device/menuDevicePeriodicalTransmission.png" ),
                                                                                   "Periodic Transmission" ),
                                                                     _menuActionMap[ mDevice ].first );
   _menuActionMap[ mDeviceSnapshot ] = TMenuRelationship( _menuActionMap[ mDevice ].first->menu()->
                                                             addAction( QIcon( ":/Menu/Device/menuDeviceShapshot.png" ),
                                                                        "Snapshot" ),
                                                          _menuActionMap[ mDevice ].first );

   //////////////////////////////////////////////////////////////////////////////////////////////////////
   _menuActionMap[ mMeter ] = TMenuRelationship( _menuBar.addMenu( "Meter" )->menuAction(), &_menuBar );
   _menuActionMap[ mMeterMeasureUnits ] = TMenuRelationship( _menuActionMap[ mMeter ].first->menu()->
                                                                addMenu( QIcon( ":/Menu/Meter/menuMeterMeasureUnits.png" ),
                                                                                "Measure units" )->menuAction(),
                                                             _menuActionMap[ mMeter ].first );

   _menuActionMap[ mMeterGraph ] = TMenuRelationship( _menuActionMap[ mMeter ].first->menu()->
                                                         addMenu( QIcon( ":/Menu/Meter/menuMeterGraph.png" ),
                                                                  "Graph" )->menuAction(),
                                                      _menuActionMap[ mMeter ].first );
   
   _menuActionMap[ mMeterGraphWindow ] = TMenuRelationship( _menuActionMap[ mMeterGraph ].first->menu()->
                                                               addAction( QIcon( ":/Menu/Meter/menuMeterGraphWindow.png" ),
                                                                         "In Window" ),
                                                            _menuActionMap[ mMeterGraph ].first );
   
   _menuActionMap[ mMeterGraphView ] = TMenuRelationship( _menuActionMap[ mMeterGraph ].first->menu()->
                                                             addAction( QIcon( ":/Menu/Meter/menuMeterGraphView.png" ),
                                                                       "In View" ),
                                                          _menuActionMap[ mMeterGraph ].first );
   
   _menuActionMap[ mMeterGraph ].first->menu()->addSeparator();
   
   _menuActionMap[ mMeterGraphClose ] = TMenuRelationship( _menuActionMap[ mMeterGraph ].first->menu()->
                                                              addAction( QIcon( ":/Menu/Meter/menuMeterGraphClose.png" ),
                                                                         "Close" ),
                                                           _menuActionMap[ mMeterGraph ].first );
   
   _menuActionMap[ mMeterGraphCloseAll ] = TMenuRelationship( _menuActionMap[ mMeterGraph ].first->menu()->
                                                                 addAction( QIcon( ":/Menu/Meter/menuMeterGraphCloseAll.png" ),
                                                                            "Close All" ),
                                                              _menuActionMap[ mMeterGraph ].first );

   //////////////////////////////////////////////////////////////////////////////////////////////////////
   _menuActionMap[ mHelp ] = TMenuRelationship( _menuBar.addMenu( "Help" )->menuAction(), &_menuBar );

   _menuActionMap[ mHelpAbout ] = TMenuRelationship( _menuActionMap[ mHelp ].first->menu()->addAction( "About" ),
                                                     _menuActionMap[ mHelp ].first );

   //////////////////////////////////////////////////////////////////////////////////////////////////////
   for( TMenuActionMap::iterator i( _menuActionMap.begin() ); i != _menuActionMap.end(); ++i )
   {
      menu( i->first )->setVisible( false );
   }
}

CMenuBarManager::~CMenuBarManager()
{
   TRACE_FUN( Routine, "CMenuBarManager::~CMenuBarManager" );
}

QAction* CMenuBarManager::menu( EMenu aMenu )const
{
   return _menuActionMap.find( aMenu )->second.first;
}

void CMenuBarManager::ShowMenu( EMenu aMenu )const
{
   menu( aMenu )->setVisible( true );

   menuShowHideEvent();

   EMenu parent( parentMenu( aMenu ) );

   if( parent != mNoMenu )
   {
      if( !menu( parent )->isVisible() )
      {
         ShowMenu( parent );
      }
   }
}

void CMenuBarManager::HideMenu( EMenu aMenu )const
{
   menu( aMenu )->setVisible( false );

   menuShowHideEvent();

   EMenu parent( parentMenu( aMenu ) );

   if( parent != mNoMenu )
   {
      QList< QAction* > actionList( menu( parent )->menu()->actions() );

      bool hasVisible( false );

      for( QList< QAction* >::iterator i( actionList.begin() ); i != actionList.end() && !hasVisible; ++i )
      {
         hasVisible = !( *i )->isSeparator() && ( *i )->isVisible();
      }

      if( !hasVisible )
      {
         HideMenu( parent );
      }
   }
}

void CMenuBarManager::ShowMenu( EMenu aMenu, const TSessionId& anId )const
{
   TRACE_FUN( Routine, "CMenuBarManager::ShowMenu" );
   
   TMenuSessionContainer::iterator I_menu( _menuSessionContainer.find( aMenu ) );
   
   if( I_menu == _menuSessionContainer.end() )
   {
      I_menu = _menuSessionContainer.insert( TMenuSessionContainer::value_type( aMenu, TSessionContainer() ) ).first;
   }
   
   TSessionContainer::iterator I_session( I_menu->second.find( anId ) );

   if( I_session == I_menu->second.end() )
   {
      I_menu->second.insert( anId );
      
      ShowMenu( aMenu );
   }
}

void CMenuBarManager::HideMenu( EMenu aMenu, const TSessionId& anId )const
{
   TRACE_FUN( Routine, "CMenuBarManager::HideMenu" );
   
   TMenuSessionContainer::iterator I_menu( _menuSessionContainer.find( aMenu ) );
   
   if( I_menu != _menuSessionContainer.end() )
   {
      TSessionContainer::iterator I_session( I_menu->second.find( anId ) );
      
      if( I_session != I_menu->second.end() )
      {
         I_menu->second.erase( I_session );
         
         if( !I_menu->second.size() )
         {
            _menuSessionContainer.erase( I_menu );
            
            HideMenu( aMenu );
         }
      }
   }
}

void CMenuBarManager::Connect( EMenu aMenu, QObject* anObject, const char* aSlot, const char* aSignal )
{
   QObject::connect( menu( aMenu ), aSignal, anObject, aSlot );
}

CMenuBarManager::EMenu CMenuBarManager::parentMenu( EMenu aMenu )const
{
   EMenu ret( mNoMenu );

   TMenuRelationship relationship( _menuActionMap.find( aMenu )->second );

   if( relationship.second != &_menuBar )
   {
      for( TMenuActionMap::const_iterator i( _menuActionMap.begin() ); i != _menuActionMap.end(); ++i )
      {
         if( i->second.first == relationship.second )
         {
            ret = i->first;

            break;
         }
      }
   }

   return ret;
}

template< class TParent >
   void CMenuBarManager::Replace( EMenu aMenu, QAction* anAction, TParent& aParent )
   {
      aParent.insertAction( _menuActionMap[ aMenu ].first, anAction );
      aParent.removeAction( _menuActionMap[ aMenu ].first );

      _menuActionMap[ aMenu ].first = anAction;
   }

void CMenuBarManager::Replace( EMenu aMenu, QAction* anAction )
{
   EMenu parent( parentMenu( aMenu ) );

   if( parent == mNoMenu )
   {
      Replace( aMenu, anAction, _menuBar );
   }
   else
   {
      Replace( aMenu, anAction, *menu( parent )->menu() );
   }
}

CMenuBarManager::TSessionId CMenuBarManager::OpenSession()
{
   TSessionId ret( CMenuBarManager::_invalidSessionId );
   
   for( TSessionId i( 0 ); i < CMenuBarManager::_invalidSessionId; ++i )
   {
      if( _sessionsContainer.find( i ) == _sessionsContainer.end() )
      {
         ret = i;
         
         _sessionsContainer.insert( ret );
         
         break;
      }
   }
   
   return ret;
}

void CMenuBarManager::CloseSession( const TSessionId& anId )
{
   TSessionsContainer::iterator I_session( _sessionsContainer.find( anId ) );
   
   CHECK_OBJECT_STATE( I_session != _sessionsContainer.end(), "Session is not registered" );
   
   typedef std::set< EMenu > TMenuContainer;
   TMenuContainer menuContainer;
   
   for( TMenuSessionContainer::const_iterator I_menu( _menuSessionContainer.begin() );
         I_menu != _menuSessionContainer.end(); ++I_menu )
   {
      if( I_menu->second.find( anId ) != I_menu->second.end() )
      {
         menuContainer.insert( I_menu->first );
      }
   }
   
   for( TMenuContainer::const_iterator I_menu( menuContainer.begin() );
         I_menu != menuContainer.end(); ++I_menu )
   {
      HideMenu( *I_menu, *I_session );
   }
   
   _sessionsContainer.erase( I_session );
}
