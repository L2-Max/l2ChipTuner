/*
 * MeterGraphManager.cpp
 *
 *  Created on: 01.10.2009
 *      Author: mars
 */

#include "MeterGraphManager.h"
#include "MeterGraphWindow.h"
#include "Meter2DGraphView.h"
#include "Meter2DGraphModel.h"

#include <AppContext.h>
#include <AppDefs.h>
#include <ECUDeviceView.h>
#include <ECUDeviceUnitSelectionModel.h>
#include <ECUDevice.h>
#include <ECUUnitsTable.h>
#include <ECUDataUnit.h>
#include <ECUDiagnosticUnitModelUtils.h>
#include <ECUDiagnosticGraphView.h>
#include <ECUDiagnosticGraphViewLayout.h>

#include <l2ExceptionThrower.h>
#include <l2Trace.h>

#include <QLayout>

CMeterGraphManager::CMeterGraphManager( CECUDeviceView* aView )
{
   TRACE_FUN( Routine, "CMeterGraphManager::CMeterGraphManager" );
   
   _deviceView = aView;
}

CMeterGraphManager::~CMeterGraphManager()
{
   TRACE_FUN( Routine, "CMeterGraphManager::~CMeterGraphManager" );
}

void CMeterGraphManager::OpenGraphsInView( const Diagnostic::TUnitKeys& aKeys )
{
   TRACE_FUN( Routine, "CMeterGraphManager::OpenGraphsInView" );
   
   for( Diagnostic::TUnitKeys::const_iterator I_key( aKeys.begin() ); I_key != aKeys.end(); ++I_key )
   {
      CMeter2DGraphView* graphView( view( *I_key ) );
      
      TGraphWindowContainer::iterator I_window( _graphWindowContainer.find( *I_key ) );

      if( I_window != _graphWindowContainer.end() )
      {
         I_window->second->SetView( 0 );
         
         delete I_window->second;

         _graphWindowContainer.erase( I_window );
      }
      
      if( !graphView )
      {
         graphView = new CMeter2DGraphView( *I_key, *_deviceView );
         
         _graphViewContainer.insert( graphView );
      }

      _deviceView->ecuDiagnosticGraphView()->InsertGraph( graphView );
   }
}

void CMeterGraphManager::OpenGraphsInWindow( const Diagnostic::TUnitKeys& aKeys )
{
   TRACE_FUN( Routine, "CMeterGraphManager::OpenGraphsInWindow" );
   
   for( Diagnostic::TUnitKeys::const_iterator I_key( aKeys.begin() ); I_key != aKeys.end(); ++I_key )
   {
      CMeterGraphWindow* graphWindow( 0 );
      CMeter2DGraphView* graphView( view( *I_key ) );
      
            
      TGraphWindowContainer::iterator I_window( _graphWindowContainer.find( *I_key ) );

      if( I_window != _graphWindowContainer.end() )
      {
         graphWindow = I_window->second;
      }
      
      if( !graphView )
      {
         graphView = new CMeter2DGraphView( *I_key, *_deviceView );
         
         _graphViewContainer.insert( graphView );
      }
      
      if( !graphWindow )
      {
         graphWindow = new CMeterGraphWindow( *_deviceView );
         
         graphWindow->SetView( graphView );
         
         graphWindow->show();
         
         _graphWindowContainer.insert( TGraphWindowContainer::value_type( graphView->key(), graphWindow ) );
      }
      
      graphWindow->activateWindow();
   }
}

void CMeterGraphManager::Restore()
{
   TRACE_FUN( Routine, "CMeterGraphManager::Restore" );
   
   l2max::CVariant openedGraphs;
   CAppContext::GetInstance()->MainRegistry().Read( S_METER_GRAPH + _deviceView->registryNamespace() + V_METER_GRAPH_OPENEDGRAPHS,
                                                    openedGraphs );

   if( openedGraphs.isValid() )
   {
      ECUDiagnosticModelUtils::TSynonyms synonyms( ECUDiagnosticModelUtils::Str2Synonyms( openedGraphs.toString() ) );

      Diagnostic::TUnitKeys keysInWindow;
      Diagnostic::TUnitKeys keysInView;

      for( ECUDiagnosticModelUtils::TSynonyms::const_iterator I_synonym( synonyms.begin() );
           I_synonym != synonyms.end(); ++I_synonym )
      {
         Diagnostic::CUnitKey key( _deviceView->ecuDevice().unitsTable().Synonym2Key( *I_synonym ) );

         if( key.isValid() )
         {
            l2max::CVariant openInWindow;
            CAppContext::GetInstance()->MainRegistry().Read( S_METER_GRAPH + _deviceView->registryNamespace() +
                                                             "::" + I_synonym->toStdString() + V_METER_GRAPH_OPENINWINDOW,
                                                             openInWindow );
            
            if( !openInWindow.isValid() || openInWindow.toBool() )
            {
               keysInWindow.insert( key );
            }
            else
            {
               keysInView.insert( key );
            }
         }
      }
      
      OpenGraphsInWindow( keysInWindow );
      
      if( _deviceView->ecuDiagnosticGraphView() )
      {
         OpenGraphsInView( keysInView );
      }
      else
      {
         _viewKeyContainer = keysInView;
      }
   }
}

void CMeterGraphManager::Close( const Diagnostic::CUnitKey& aKey )
{
   Diagnostic::TUnitKeys keys;
   keys.insert( aKey );
   
   Close( keys );
}

void CMeterGraphManager::Close( const Diagnostic::TUnitKeys& aKeys )
{
   TRACE_FUN( Routine, "CMeterGraphManager::Close" );
   
   for( Diagnostic::TUnitKeys::const_iterator I_key( aKeys.begin() ); I_key != aKeys.end(); ++I_key )
   {
      TGraphWindowContainer::iterator I_window( _graphWindowContainer.find( *I_key ) );
      
      if( I_window != _graphWindowContainer.end() )
      {
         CMeterGraphWindow* graphWindow( I_window->second );
         
         _graphWindowContainer.erase( I_window );
         
         delete graphWindow;
      }
      
      CMeter2DGraphView* graphView( view( *I_key ) );
      
      if( graphView )
      {
         graphView->setParent( 0 );
         
         _graphViewContainer.erase( graphView );
         
         delete graphView;
      }
   }
}

void CMeterGraphManager::CloseAll()
{
   TRACE_FUN( Routine, "CMeterGraphManager::CloseAll" );
   
   //////////////////////////////////////////////////////////////////////////////////////////////////////
   Diagnostic::TUnitKeys keys( _viewKeyContainer );
   
   _viewKeyContainer.clear();

   for( TGraphViewContainer::const_iterator I_view( _graphViewContainer.begin() );
         I_view != _graphViewContainer.end(); ++I_view )
   {
      Diagnostic::CUnitKey key( ( *I_view )->key() );
      
      keys.insert( key );
   }
   
   ///////////////////////////////////////////////////////////////////////////////////////////////////////
   ECUDiagnosticModelUtils::TSynonyms synonyms;
   
   for( Diagnostic::TUnitKeys::const_iterator I_key( keys.begin() ); I_key != keys.end(); ++I_key )
   {
      std::string synonym( _deviceView->ecuDevice().unitsTable().data( *I_key )->synonym() );

      synonyms.push_back( synonym );

      CAppContext::GetInstance()->MainRegistry().Write( S_METER_GRAPH + _deviceView->registryNamespace() +
                                                        "::" + synonym + V_METER_GRAPH_OPENINWINDOW,
                                                        IsWindow( *I_key ) );
   }
   
   CAppContext::GetInstance()->MainRegistry().Write( S_METER_GRAPH + _deviceView->registryNamespace() + V_METER_GRAPH_OPENEDGRAPHS,
                                                     ECUDiagnosticModelUtils::Synonyms2Str( synonyms ) );
      
   Close( keys );
}

void CMeterGraphManager::Update()
{
   TRACE_FUN( Routine, "CMeterGraphManager::Update" );
   
   for( TGraphViewContainer::iterator I_view( _graphViewContainer.begin() );
         I_view != _graphViewContainer.end(); ++I_view )
   {
      ( *I_view )->model()->update();
   }
}

bool CMeterGraphManager::IsView( const Diagnostic::CUnitKey& aKey )const
{
   CAbstractMeterView* theView( view( aKey ) );
   
   return theView && _graphWindowContainer.find( aKey ) == _graphWindowContainer.end();
}

bool CMeterGraphManager::IsWindow( const Diagnostic::CUnitKey& aKey )const
{
   CAbstractMeterView* theView( view( aKey ) );
   
   return theView && _graphWindowContainer.find( aKey ) != _graphWindowContainer.end();
}

CMeter2DGraphView* CMeterGraphManager::view( const Diagnostic::CUnitKey& aKey )const
{
   CMeter2DGraphView* ret( 0 );
   
   for( TGraphViewContainer::iterator I_view( _graphViewContainer.begin() );
         !ret && I_view != _graphViewContainer.end(); ++I_view )
   {
      if( ( *I_view )->key() == aKey )
      {
         ret = *I_view;
      }
   }
   
   return ret;
}

bool CMeterGraphManager::hasOpenedGraph()const
{
   return _graphViewContainer.size();
}

void CMeterGraphManager::CloseView()
{
   TRACE_FUN( Routine, "CMeterGraphManager::CloseView" );
   
   _viewKeyContainer.clear();
   
   for( TGraphViewContainer::const_iterator I_view( _graphViewContainer.begin() );
         I_view != _graphViewContainer.end(); ++I_view )
   {
      Diagnostic::CUnitKey key( ( *I_view )->key() );
      
      if( IsView( key ) )
      {
         _viewKeyContainer.insert( key );
      }
   }
   
   Close( _viewKeyContainer );
}

void CMeterGraphManager::RestoreView()
{
   TRACE_FUN( Routine, "CMeterGraphManager::RestoreView" );
   
   OpenGraphsInView( _viewKeyContainer );
}
