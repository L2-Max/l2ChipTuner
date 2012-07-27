/*
 * ECUDeviceView.cpp
 *
 *  Created on: 05.10.2008
 *      Author: L2-Max
 */

#include "ECUDeviceView.h"
#include "ECUDiagnosticUnitsMonitorView.h"
#include "ECUDiagnosticErrorsView.h"
#include "ECUFlowMeterDeviceView.h"
#include "ECUDiagnosticGraphView.h"
#include "MainWindow.h"
#include "FileProtocolPacketSelectionView.h"
#include "ECUDeviceUnitSelectionModel.h"

#include <AppDefs.h>
#include <AppContext.h>
#include <MeterGraphManager.h>

#include <ECUDevice.h>
#include <ECUErrorsTable.h>
#include <ECUFlowMeterDevice.h>
#include <ECUDeviceStatistic.h>
#include <ECUUnitsTable.h>
#include <ECUDataUnit.h>

#include <ALDLProto.h>
#include <ProtocolDiagnosticDataStorage.h>

#include <l2Trace.h>
#include <l2Thread.h>
#include <l2String.h>
#include <l2ExceptionThrower.h>

#include <QMenu>
#include <QTabBar>
#include <QPainter>
#include <QStatusBar>
#include <QScrollArea>
#include <QCloseEvent>
#include <QDockWidget>

#define PATH_SEPARATOR ":"

CECUDeviceView::CECUDeviceView( CMainWindow* aMainWindow ) : QTabWidget( aMainWindow ),
   _ecuNotifyHandler( this )
{
   _readyToClose = true;

   _noErrorIcon = QIcon( ":/Device/View/deviceViewErrorNoError.png" );
   _errorIcon = QIcon( ":/Device/View/deviceViewErrorHasError.png" );

   _mainWindow = aMainWindow;
   
   _fileProtocolPacketSelectionView = 0;
   _selectionModel = 0;
   _meterGraphManager = 0;

   _ecuDevice = new ECU::CDevice( &_ecuNotifyHandler );
   
   _menuSessionId = mainWindow().menuBarManager().OpenSession();

   CreateMenu();
   
   mainWindow().communicationStatus().hide();
   
   ///////////////////////////////////////////////////////////////////////////////////////////////
   l2max::CVariant startDiag;
   CAppContext::GetInstance()->MainRegistry().Read( V_STARTUP_STARTDIAGNOSTICSESSION, startDiag );
   
   if( startDiag.toBool() )
   {
      OnStartStopDiagnosticSession();
   }
}

CECUDeviceView::~CECUDeviceView()
{
   TRACE_FUN( Routine, "CECUDeviceView::~CECUDeviceView" );
   
   mainWindow().menuBarManager().CloseSession( _menuSessionId );

   delete _selectionModel;
   delete _ecuDevice;
}

void CECUDeviceView::CreateMenu()
{
   ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   mainWindow().menuBarManager().menu( CMenuBarManager::mViewUnits )->setCheckable( true );
   mainWindow().menuBarManager().Connect( CMenuBarManager::mViewUnits, this, SLOT( OnShowHideUnitsMonitorView() ) );
   
   mainWindow().menuBarManager().menu( CMenuBarManager::mViewError )->setCheckable( true );
   mainWindow().menuBarManager().Connect( CMenuBarManager::mViewError, this, SLOT( OnShowHideErrorView() ) );

   mainWindow().menuBarManager().menu( CMenuBarManager::mViewFlowMeter )->setCheckable( true );
   mainWindow().menuBarManager().Connect( CMenuBarManager::mViewFlowMeter, this, SLOT( OnShowHideFlowMeterView() ) );

   mainWindow().menuBarManager().menu( CMenuBarManager::mViewGraph )->setCheckable( true );
   mainWindow().menuBarManager().Connect( CMenuBarManager::mViewGraph, this, SLOT( OnShowHideGraphView() ) );
   
   mainWindow().menuBarManager().menu( CMenuBarManager::mViewPacketSelection )->setCheckable( true );
   mainWindow().menuBarManager().Connect( CMenuBarManager::mViewPacketSelection, this, SLOT( OnShowHidePacketSelectionView() ) );

   mainWindow().menuBarManager().menu( CMenuBarManager::mDeviceStartStopDiagnostic )->setShortcut( Qt::Key_F5 );
   mainWindow().menuBarManager().Connect( CMenuBarManager::mDeviceStartStopDiagnostic, this, SLOT( OnStartStopDiagnosticSession() ) );

   mainWindow().menuBarManager().ShowMenu( CMenuBarManager::mDeviceStartStopDiagnostic, _menuSessionId );

   connect( mainWindow().menuBarManager().menu( CMenuBarManager::mDevicePeriodicTrasmission ), SIGNAL( toggled( bool ) ),
                                                                                               SLOT( OnPeriodicTransmission( bool ) ) );

   mainWindow().menuBarManager().Connect( CMenuBarManager::mDeviceSnapshot, this, SLOT( OnSnapshot() ) );
   
   ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   mainWindow().menuBarManager().Connect( CMenuBarManager::mMeterGraphWindow, this, SLOT( OnOpenGraphInWindow() ) );
   mainWindow().menuBarManager().Connect( CMenuBarManager::mMeterGraphView, this, SLOT( OnOpenGraphInView() ) );
   mainWindow().menuBarManager().Connect( CMenuBarManager::mMeterGraphClose, this, SLOT( OnCloseGraph() ) );
   mainWindow().menuBarManager().Connect( CMenuBarManager::mMeterGraphCloseAll, this, SLOT( OnCloseAllGraph() ) );
}

void CECUDeviceView::RemoveViews()
{
   TRACE_FUN( Routine, "CECUDeviceView::RemoveViews" );

   //////////////////////////////////////////////////////////////////////////////////////////////////////
   if( _fileProtocolPacketSelectionView )
   {
      CAppContext::GetInstance()->MainRegistry().Write( V_FILE_PROTO_PACKSEL_DLG_ISVISIBLE, _fileProtocolPacketSelectionView->isVisible() );

      delete _fileProtocolPacketSelectionView;
      _fileProtocolPacketSelectionView = 0;

      mainWindow().menuBarManager().HideMenu( CMenuBarManager::mViewPacketSelection, _menuSessionId );
   }
   
   //////////////////////////////////////////////////////////////////////////////////////////////////////
   if( meterGraphManager() )
   {
      meterGraphManager()->CloseAll();
   }
   
   //////////////////////////////////////////////////////////////////////////////////////////////////////
   l2max::TStringList openedViews;

   for( int i( 0 ); i != count(); ++i )
   {
      openedViews.push_back( l2max::CVariant( Index2PageId( i ) ).toString() );
   }

   CAppContext::GetInstance()->MainRegistry().Write( V_DDV_OPENEDVIEWS, l2max::CString::List2String( openedViews, PATH_SEPARATOR ).toStdString() );
   CAppContext::GetInstance()->MainRegistry().Write( V_DDV_SELECTEVIEW, currentIndex() );

   //////////////////////////////////////////////////////////////////////////////////////////////////////
   RemoveView( piGraph );
   mainWindow().menuBarManager().HideMenu( CMenuBarManager::mViewGraph, _menuSessionId );
   
   //////////////////////////////////////////////////////////////////////////////////////////////////////
   RemoveView( piFlowMeter );
   mainWindow().menuBarManager().HideMenu( CMenuBarManager::mViewFlowMeter, _menuSessionId );
   
   //////////////////////////////////////////////////////////////////////////////////////////////////////
   RemoveView( piErrors );
   mainWindow().menuBarManager().HideMenu( CMenuBarManager::mViewError, _menuSessionId );
   
   //////////////////////////////////////////////////////////////////////////////////////////////////////
   RemoveView( piUnitsMonitor );
   mainWindow().menuBarManager().HideMenu( CMenuBarManager::mViewUnits, _menuSessionId );

   if( mainWindow().menuBarManager().menu( CMenuBarManager::mDevicePeriodicTrasmission )->isVisible() )
   {
      CAppContext::GetInstance()->MainRegistry().Write( V_ECU_DIAG_SESS_PERIODICTRANSM,
                                                        mainWindow().menuBarManager().menu( CMenuBarManager::mDevicePeriodicTrasmission )->isChecked() );
   }
}

void CECUDeviceView::OnStartStopDiagnosticSession()
{
   TRACE_FUN( Routine, "CECUDeviceView::OnStartStopDiagnosticSession" );

   bool res( false );

   if( !_ecuDevice->isInitialized() )
   {
      if( _ecuDevice->Initialize() )
      {
         mainWindow().deviceStatus().setText( "Attempt to estamilish communication with ECU" );

          res = _ecuDevice->StartDiagnosticSession();
      }

      if( _ecuDevice->errorCode() != ECU::CDevice::ecNoError )
      {
         mainWindow().deviceStatus().setText( _ecuDevice->errorText().c_str() );

         _ecuDevice->UnInitialize();
      }
   }
   else
   {
      mainWindow().deviceStatus().setText( "Attempt to cancel communication with ECU" );

      _ecuDevice->StopDiagnosticSession();

      res = true;
   }

   mainWindow().menuBarManager().menu( CMenuBarManager::mDeviceStartStopDiagnostic )->setEnabled( !res );
}

void CECUDeviceView::OnDiagnosticSessionStarted()
{
   TRACE_FUN( Routine, "CECUDeviceView::OnDiagnosticSessionStarted" );

   _initialDataRead = true;
   _readyToClose = false;
   
   mainWindow().deviceStatus().setText( "Communication with ECU estabilished." );

   mainWindow().menuBarManager().menu( CMenuBarManager::mDeviceStartStopDiagnostic )->setText( "Stop Diagnostic Session" );
   mainWindow().menuBarManager().menu( CMenuBarManager::mDeviceStartStopDiagnostic )->setIcon( QIcon( ":/Menu/Device/menuDeviceStopDiagnosticSession.png" ) );

   mainWindow().menuBarManager().menu( CMenuBarManager::mDeviceStartStopDiagnostic )->setEnabled( true );

   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   CHECK_OBJECT_STATE( !_selectionModel, "Selection model is not null" );
   
   _selectionModel = new CECUDeviceUnitSelectionModel();
   
   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   l2max::CVariant selectedView;
   CAppContext::GetInstance()->MainRegistry().Read( V_DDV_SELECTEVIEW, selectedView );

   l2max::CVariant openedViews;
   CAppContext::GetInstance()->MainRegistry().Read( V_DDV_OPENEDVIEWS, openedViews );

   l2max::TStringList path( l2max::CString::String2List( openedViews.toString(), PATH_SEPARATOR ) );

   for( l2max::TStringList::iterator i( path.begin() ); i != path.end(); ++i )
   {
      EPageId pageId( static_cast< EPageId >( l2max::CVariant( i->toStdString() ).toInt() ) );

      if( pageId == piUnitsMonitor )
      {
         if( !mainWindow().menuBarManager().menu( CMenuBarManager::mViewUnits )->isChecked() )
         {
            mainWindow().menuBarManager().menu( CMenuBarManager::mViewUnits )->trigger();
         }
         else
         {
            OnShowHideUnitsMonitorView();
         }

      }
      else if( pageId == piErrors )
      {
         if( !mainWindow().menuBarManager().menu( CMenuBarManager::mViewError )->isChecked() )
         {
            mainWindow().menuBarManager().menu( CMenuBarManager::mViewError )->trigger();
         }
         else
         {
            OnShowHideErrorView();
         }

      }
      else if( pageId == piFlowMeter )
      {
         if( !mainWindow().menuBarManager().menu( CMenuBarManager::mViewFlowMeter )->isChecked() )
         {
            mainWindow().menuBarManager().menu( CMenuBarManager::mViewFlowMeter )->trigger();
         }
         else
         {
            OnShowHideFlowMeterView();
         }
      }
      else if( pageId == piGraph )
      {
         if( !mainWindow().menuBarManager().menu( CMenuBarManager::mViewGraph )->isChecked() )
         {
            mainWindow().menuBarManager().menu( CMenuBarManager::mViewGraph )->trigger();
         }
         else
         {
            OnShowHideGraphView();
         }
      }
   }

   if( path.size() )
   {
      setCurrentIndex( selectedView.toInt() );
   }
   
   mainWindow().menuBarManager().ShowMenu( CMenuBarManager::mViewUnits, _menuSessionId );
   mainWindow().menuBarManager().ShowMenu( CMenuBarManager::mViewError, _menuSessionId );
   mainWindow().menuBarManager().ShowMenu( CMenuBarManager::mViewFlowMeter, _menuSessionId );
   mainWindow().menuBarManager().ShowMenu( CMenuBarManager::mViewGraph, _menuSessionId );
   
   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   mainWindow().menuBarManager().menu( CMenuBarManager::mDevicePeriodicTrasmission )->setCheckable( true );

   l2max::CVariant periodicTransm;
   CAppContext::GetInstance()->MainRegistry().Read( V_ECU_DIAG_SESS_PERIODICTRANSM, periodicTransm );

   if( periodicTransm.toBool() )
   {
      if( !mainWindow().menuBarManager().menu( CMenuBarManager::mDevicePeriodicTrasmission )->isChecked() )
      {
         mainWindow().menuBarManager().menu( CMenuBarManager::mDevicePeriodicTrasmission )->trigger();
      }
      else
      {
         OnPeriodicTransmission( true );
      }
   }
   else
   {
      mainWindow().menuBarManager().menu( CMenuBarManager::mDeviceSnapshot )->setEnabled( true );
   }

   mainWindow().menuBarManager().ShowMenu( CMenuBarManager::mDevicePeriodicTrasmission, _menuSessionId );
   mainWindow().menuBarManager().ShowMenu( CMenuBarManager::mDeviceSnapshot, _menuSessionId );

   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   CreatePacketSelectionView();
   
   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   _meterGraphManager = new CMeterGraphManager( this );

   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   mainWindow().communicationStatus().show();
   
   OnSnapshot();
   OnSnapshot();
}

void CECUDeviceView::OnDiagnosticSessionStoped()
{
   TRACE_FUN( Routine, "CECUDeviceView::OnDiagnosticSessionStoped" );

   mainWindow().deviceStatus().setText( "Communication with ECU cancelled." );
}

void CECUDeviceView::InitialDataRead()
{
   meterGraphManager()->Restore();
}

void CECUDeviceView::OnDataRead()
{
   TRACE_FUN( Routine, "CECUDeviceView::OnDataRead" );
   
   if( _initialDataRead )
   {
      _initialDataRead = false;
      
      InitialDataRead();
   }

   /////////////////////////////////////////////////////////////////////////////////////////////////
   CECUDiagnosticUnitsMonitorView* monitorView( ecuDiagnosticUnitsMonitorView() );

   if( monitorView && monitorView->isVisible() )
   {
      monitorView->UpdateDiagnosticDataTable( _ecuDevice->unitsTable() );
   }

   CECUDiagnosticErrorsView* errorView( ecuDiagnosticErrorsView() );

   if( errorView )
   {
      ECU::CErrorsTable& errorTable( _ecuDevice->errorsTable() );

      setTabIcon( PageId2Index( piErrors ), errorTable.count() ? _errorIcon : _noErrorIcon );

      errorView->UpdateDiagnosticErrorsTable( errorTable );
   }

   CECUFlowMeterDeviceView* flowMeterView( ecuFlowMeterDeviceView() );

   if( flowMeterView )
   {
      flowMeterView->InstallFlowMeterDevice( _ecuDevice->flowMeter() );
   }

   if( !_ecuDevice->IsPeriodicTransmissionEnabled() )
   {
      mainWindow().menuBarManager().menu( CMenuBarManager::mDeviceSnapshot )->setEnabled( true );
   }
   
   /////////////////////////////////////////////////////////////////////////////////////////////////
   meterGraphManager()->Update();
   
   /////////////////////////////////////////////////////////////////////////////////////////////////
   mainWindow().deviceStatus().setText( "Data read" );

   static int i( 0 );

   if( i++ == 1000 )
   {
//      mainWindow().close();
   }
}

void CECUDeviceView::OnDataWriten( int aCode )
{
   TRACE_FUN( Routine, "CECUDeviceView::OnDataWriten" );

   if( aCode == CAbstractProto::wrcAccept )
   {
      mainWindow().deviceStatus().setText( "ECU accept control" );
   }
   else if( aCode == CAbstractProto::wrcReject )
   {
      mainWindow().deviceStatus().setText( "ECU reject control" );
   }
}

void CECUDeviceView::OnStopped()
{
   TRACE_FUN( Routine, "CECUDeviceView::OnStopped" );

   mainWindow().deviceStatus().setText( std::string( "Scaner stopped. " + _ecuDevice->errorText() ).c_str() );

   RemoveViews();
   
   delete _meterGraphManager;
   _meterGraphManager = 0;
   
   delete _selectionModel;
   _selectionModel = 0;

   _ecuDevice->UnInitialize();

   mainWindow().menuBarManager().menu( CMenuBarManager::mDeviceStartStopDiagnostic )->setText( "Start Diagnostic Session" );
   mainWindow().menuBarManager().menu( CMenuBarManager::mDeviceStartStopDiagnostic )->
      setIcon( QIcon( ":/Menu/Device/menuDeviceStartDiagnosticSession.png" ) );

   mainWindow().menuBarManager().menu( CMenuBarManager::mDeviceStartStopDiagnostic )->setEnabled( true );

   mainWindow().menuBarManager().HideMenu( CMenuBarManager::mDevicePeriodicTrasmission, _menuSessionId );
   mainWindow().menuBarManager().HideMenu( CMenuBarManager::mDeviceSnapshot, _menuSessionId );

   mainWindow().communicationStatus().hide();

   ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   _readyToClose = true;

   mainWindow().CloseIfRequired();
}

void CECUDeviceView::paintEvent( QPaintEvent* anEvent )
{
   if( !count() )
   {
      std::string message;
      
      if( _ecuDevice && _ecuDevice->isInitialized() )
      {
         message = "There are nothing to display. No views are opened.";
      }
      else
      {
         message = "There are nothing to display. Start communication with ECU first.";
      }
      
      QFont f( font() );
      f.setPointSize( 10 );

      QPainter painter( this );
      painter.setFont( f );
      painter.drawText( rect(), Qt::AlignCenter | Qt::TextWordWrap, message.c_str() );
   }
   else
   {
      QTabWidget::paintEvent( anEvent );
   }
}

void CECUDeviceView::OnShowHideUnitsMonitorView()
{
   TRACE_FUN( Routine, "CECUDeviceView::OnShowHideUnitsMonitorView" );
   
   CECUDiagnosticUnitsMonitorView* unitsView( ecuDiagnosticUnitsMonitorView() );
   
   if( !unitsView )
   {
      unitsView = new CECUDiagnosticUnitsMonitorView( this );
      connect( unitsView, SIGNAL( controlChangedEvent() ), SLOT( OnControlChanged() ) );

      _pageId2Widget[ piUnitsMonitor ] = unitsView;

      insertTab( PageId2Index( piUnitsMonitor ), unitsView, QIcon( ":/Menu/View/menuViewMonitor.png" ), "Diagnostic Units Monitor" );
      setCurrentIndex( PageId2Index( piUnitsMonitor ) );
   }
   else
   {
      RemoveView( piUnitsMonitor );
   }
}

void CECUDeviceView::OnShowHideErrorView()
{
   TRACE_FUN( Routine, "CECUDeviceView::OnShowHideErrorView" );

   CECUDiagnosticErrorsView* errorView( ecuDiagnosticErrorsView() );

   if( !errorView )
   {
      errorView = new CECUDiagnosticErrorsView( this );

      _pageId2Widget[ piErrors ] = errorView;

      insertTab( PageId2Index( piErrors ), errorView, "ECU Errors" );
      setCurrentIndex( PageId2Index( piErrors ) );
   }
   else
   {
      RemoveView( piErrors );
   }
}

void CECUDeviceView::OnShowHideFlowMeterView()
{
   TRACE_FUN( Routine, "CECUDeviceView::OnShowHideFlowMeter" );

   CECUFlowMeterDeviceView* flowMeterView( ecuFlowMeterDeviceView() );

   if( !flowMeterView )
   {
      flowMeterView = new CECUFlowMeterDeviceView( this );

      _pageId2Widget[ piFlowMeter ] = flowMeterView;

      insertTab( PageId2Index( piFlowMeter ), flowMeterView, QIcon( ":/Menu/View/menuViewFlowMeter.png" ), "Flow Meter" );
      setCurrentIndex( PageId2Index( piFlowMeter ) );
   }
   else
   {
      RemoveView( piFlowMeter );
   }
}

void CECUDeviceView::OnShowHideGraphView()
{
   TRACE_FUN( Routine, "CECUDeviceView::OnShowHideGraphView" );

   CECUDiagnosticGraphView* graphView( ecuDiagnosticGraphView() );

   if( !graphView )
   {
      OpenGraphView();
      
      setCurrentIndex( PageId2Index( piGraph ) );

      if( meterGraphManager() )
      {
         meterGraphManager()->RestoreView();
      }
   }
   else
   {
      meterGraphManager()->CloseView();

      RemoveView( piGraph );
   }
}

void CECUDeviceView::OpenGraphView()
{
   CECUDiagnosticGraphView* graphView( ecuDiagnosticGraphView() );
   
   if( !graphView )
   {
      QScrollArea* scrollArea( new QScrollArea( this ) );
      
      scrollArea->setWidget( new CECUDiagnosticGraphView( this ) );
      scrollArea->setWidgetResizable( true );

      _pageId2Widget[ piGraph ] = scrollArea;

      insertTab( PageId2Index( piGraph ), scrollArea, QIcon( ":/Menu/Meter/menuMeterGraphView.png" ), "Graph" );
   }
}

void CECUDeviceView::UpdateCommunicationStatus()const
{
   std::string message;

   message += "Requests. Queue: " + l2max::CVariant( int(
      double( ECU::CDeviceStatistic::GetInstance()->reqQueueSize() ) /
              ECU::CDeviceStatistic::GetInstance()->reqQueueMaxSize() * 100. ) ).toString() + "%";

   message += " Count: " + l2max::CVariant( ECU::CDeviceStatistic::GetInstance()->reqCount() ).toString();
   message += " Errors: " + l2max::CVariant( ECU::CDeviceStatistic::GetInstance()->reqErrorCount() ).toString();

   mainWindow().communicationStatus().setText( message.c_str() );
}

void CECUDeviceView::OnStatUpdated()
{
   UpdateCommunicationStatus();
}

int CECUDeviceView::PageId2Index( EPageId anId )const
{
   int ret( -1 );

   for( TPageId2Widget::const_iterator i( _pageId2Widget.begin() ); i != _pageId2Widget.end(); ++i )
   {
      ++ret;

      if( i->first == anId )
      {
         break;
      }
   }

   return ret;
}

QWidget* CECUDeviceView::PageId2View( EPageId anId )const
{
   QWidget* ret( 0 );

   TPageId2Widget::const_iterator i( _pageId2Widget.find( anId ) );

   if( i != _pageId2Widget.end() )
   {
      ret = i->second;
   }

   return ret;
}

CECUDiagnosticUnitsMonitorView* CECUDeviceView::ecuDiagnosticUnitsMonitorView()const
{
   return static_cast< CECUDiagnosticUnitsMonitorView* >( PageId2View( piUnitsMonitor ) );
}

CECUDiagnosticErrorsView* CECUDeviceView::ecuDiagnosticErrorsView()const
{
   return static_cast< CECUDiagnosticErrorsView* >( PageId2View( piErrors ) );
}

CECUFlowMeterDeviceView* CECUDeviceView::ecuFlowMeterDeviceView()const
{
   return static_cast< CECUFlowMeterDeviceView* >( PageId2View( piFlowMeter ) );
}

CECUDiagnosticGraphView* CECUDeviceView::ecuDiagnosticGraphView()const
{
   CECUDiagnosticGraphView* ret( 0 );
   
   QScrollArea* scrollArea( static_cast< QScrollArea* >( PageId2View( piGraph ) ) );
   
   if( scrollArea )
   {
      ret = static_cast< CECUDiagnosticGraphView* >( scrollArea->widget() );
   }
   
   return ret;
}

void CECUDeviceView::RemoveView( EPageId anId )
{
   removeTab( PageId2Index( anId ) );

   delete PageId2View( anId );

   _pageId2Widget.erase( anId );
}

CECUDeviceView::EPageId CECUDeviceView::Index2PageId( int anIndex )const
{
   EPageId ret( piInvalid );

   QWidget* view( widget( anIndex ) );

   if( anIndex != -1 )
   {
      for( TPageId2Widget::const_iterator i( _pageId2Widget.begin() ); i != _pageId2Widget.end(); ++i )
      {
         if( i->second == view )
         {
            ret = i->first;
            break;
         }
      }
   }

   return ret;
}

void CECUDeviceView::OnControlChanged()
{
   CHECK_OBJECT_STATE( _ecuDevice, "ECU device should not be NULL" );

   mainWindow().deviceStatus().setText( "Control Data sent to ECU" );

   _ecuDevice->TransmitData();
}

void CECUDeviceView::OnPeriodicTransmission( bool aState )
{
   TRACE_FUN( Routine, "CECUDeviceView::OnPeriodicTransmission" );

   _ecuDevice->SetPeriodicTransmissionEnabled( aState );

   mainWindow().menuBarManager().menu( CMenuBarManager::mDeviceSnapshot )->setEnabled( !aState );
}

void CECUDeviceView::OnSnapshot()
{
   TRACE_FUN( Routine, "CECUDeviceView::OnSnapshot" );

   mainWindow().menuBarManager().menu( CMenuBarManager::mDeviceSnapshot )->setEnabled( false );

   _ecuDevice->ReadDiagnosticData();
}

void CECUDeviceView::CreatePacketSelectionView()
{
   TRACE_FUN( Routine, "CECUDeviceView::CreatePacketSelectionView" );

   l2max::CVariant isPackSelVisible;
   CAppContext::GetInstance()->MainRegistry().Read( V_FILE_PROTO_PACKSEL_DLG_ISVISIBLE, isPackSelVisible );

   CHECK_OBJECT_STATE( _ecuDevice, "ECU device is null" );

   CProtocolDiagnosticDataStorage* theStorage( 0 );

   CAbstractProto* proto( _ecuDevice->protocol() );

   CHECK_OBJECT_STATE( proto, "Protocol is null" );

   if( proto->type() == Protocol::ptALDL )
   {
      ALDL::CProto* aldlProto( dynamic_cast< ALDL::CProto* >( proto ) );

      CHECK_OBJECT_STATE( aldlProto, "Can't cast to ALDL proto" );

      theStorage = aldlProto->storage();
   }

   if( theStorage && theStorage->IsReadEnabled() )
   {
      _fileProtocolPacketSelectionView = new CFileProtocolPacketSelectionView( this, *theStorage );

      if( isPackSelVisible.toBool() )
      {
         if( !mainWindow().menuBarManager().menu( CMenuBarManager::mViewPacketSelection )->isChecked() )
         {
            mainWindow().menuBarManager().menu( CMenuBarManager::mViewPacketSelection )->trigger();
         }
         else
         {
            OnShowHidePacketSelectionView();
         }
      }

      mainWindow().menuBarManager().ShowMenu( CMenuBarManager::mViewPacketSelection, _menuSessionId );
   }
}

void CECUDeviceView::OnShowHidePacketSelectionView()
{
   TRACE_FUN( Routine, "CECUDeviceView::OnShowHidePacketSelectionView" );

   if( !_fileProtocolPacketSelectionView->isVisible() )
   {
      mainWindow().statusBar()->addWidget( _fileProtocolPacketSelectionView );

      _fileProtocolPacketSelectionView->show();
   }
   else
   {
      mainWindow().statusBar()->removeWidget( _fileProtocolPacketSelectionView );
   }
}

std::string CECUDeviceView::registryNamespace()const
{
   CHECK_OBJECT_STATE( _ecuDevice, "ECU device is null" );
   
   CAbstractProto* proto( _ecuDevice->protocol() );
   
   CHECK_OBJECT_STATE( proto, "Protocol is null" );
   
   return proto->registryNamespace();
}

CMainWindow& CECUDeviceView::mainWindow()const
{
   return *_mainWindow;
}

void CECUDeviceView::UpdateMeterMenu( bool anInGraph )const
{
   TRACE_FUN( Routine, "CECUDeviceView::ShowMeterMenu" );
   
   QMenu* measureUnitsMenu( mainWindow().menuBarManager().menu( CMenuBarManager::mMeterMeasureUnits )->menu() );
   measureUnitsMenu->clear();
   
   mainWindow().menuBarManager().HideMenu( CMenuBarManager::mMeterMeasureUnits, _menuSessionId );
   mainWindow().menuBarManager().HideMenu( CMenuBarManager::mMeterGraphWindow, _menuSessionId );
   mainWindow().menuBarManager().HideMenu( CMenuBarManager::mMeterGraphView, _menuSessionId );
   mainWindow().menuBarManager().HideMenu( CMenuBarManager::mMeterGraphClose, _menuSessionId );
   mainWindow().menuBarManager().HideMenu( CMenuBarManager::mMeterGraphCloseAll, _menuSessionId );

   if( selectionModel()->hasSelection() )
   {
      Diagnostic::CUnitKey key( selectionModel()->current() );
      
      if( key.isValid() )
      {
         typedef std::map< Diagnostic::EUnit, std::string > TMeasureUnitsContainer;
         TMeasureUnitsContainer units;
         
         ECU::HDataUnit unit( _ecuDevice->unitsTable().data( key ) );
         Diagnostic::TMeasureUnitsMap measureUnitsMap( unit->measureUnits() );
      
         for( Diagnostic::TMeasureUnitsMap::iterator I_unit( measureUnitsMap.begin() ); I_unit != measureUnitsMap.end(); ++I_unit )
         {
            units[ I_unit->first ] = Diagnostic::unitDescriptionMap()->find( I_unit->first )->second._description;
         }
      
         if( units.size() )
         {
            QActionGroup* measureActionGroup( new QActionGroup( measureUnitsMenu ) );
      
            for( TMeasureUnitsContainer::const_iterator I_unit( units.begin() ); I_unit != units.end(); ++I_unit )
            {
               QAction* action( measureUnitsMenu->addAction( I_unit->second.c_str() ) );
               action->setData( I_unit->first );
      
               action->setCheckable( true );
      
               if( I_unit->first == unit->measureUnit() )
               {
                  action->setChecked( true );
               }
      
               measureActionGroup->addAction( action );
            }
      
            if( measureActionGroup->actions().size() )
            {
               connect( measureActionGroup, SIGNAL( triggered( QAction* ) ), SLOT( OnDefaultUnitChanged( QAction* ) ) );
            }
      
            mainWindow().menuBarManager().ShowMenu( CMenuBarManager::mMeterMeasureUnits, _menuSessionId );
         }
         
         if( meterGraphManager()->IsWindow( key ) || meterGraphManager()->IsView( key ) )
         {
            mainWindow().menuBarManager().ShowMenu( CMenuBarManager::mMeterGraphClose, _menuSessionId );
         }
      }
      
      ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      Diagnostic::TUnitKeys keys( selectionModel()->selected() );
   
      for( Diagnostic::TUnitKeys::const_iterator I_key( keys.begin() ); I_key != keys.end(); ++I_key )
      {
         CHECK_OBJECT_STATE( I_key->isValid(), "Unit key is invalid" );
         
         if( !anInGraph || !meterGraphManager()->IsWindow( *I_key ) )
         {
            mainWindow().menuBarManager().ShowMenu( CMenuBarManager::mMeterGraphWindow, _menuSessionId );
         }
         
         if( !anInGraph || !meterGraphManager()->IsView( *I_key ) )
         {
            mainWindow().menuBarManager().ShowMenu( CMenuBarManager::mMeterGraphView, _menuSessionId );
         }
      }
   }
   
   if( meterGraphManager()->hasOpenedGraph() )
   {
      mainWindow().menuBarManager().ShowMenu( CMenuBarManager::mMeterGraphCloseAll, _menuSessionId );
   }
}

CECUDeviceUnitSelectionModel* CECUDeviceView::selectionModel()const
{
   return _selectionModel;
}

void CECUDeviceView::OnDefaultUnitChanged( QAction* action )
{
   TRACE_FUN( Routine, "CECUDeviceView::OnDefaultUnitChanged" );

   CHECK_OBJECT_STATE( selectionModel(), "Selection Model is null" );
   
   Diagnostic::CUnitKey key( selectionModel()->current() );
   
   CHECK_OBJECT_STATE( key.isValid(), "Selected key is invalid" );
   
   ECU::HDataUnit unit( _ecuDevice->unitsTable().data( key ) );
   unit->SetMeasureUnit( static_cast< Diagnostic::EUnit >( action->data().toInt() ) );
}

void CECUDeviceView::OnOpenGraphInWindow()
{
   TRACE_FUN( Routine, "CECUDeviceView::OnOpenGraphInWindow" );
   
   CHECK_OBJECT_STATE( selectionModel(), "Selection Model is null" );
   
   Diagnostic::TUnitKeys keys( selectionModel()->selected() );
   
   meterGraphManager()->OpenGraphsInWindow( keys );
}

void CECUDeviceView::OnOpenGraphInView()
{
   TRACE_FUN( Routine, "CECUDeviceView::OnOpenGraphInView" );
   
   CHECK_OBJECT_STATE( selectionModel(), "Selection Model is null" );
   
   if( !ecuDiagnosticGraphView() )
   {
      mainWindow().menuBarManager().menu( CMenuBarManager::mViewGraph )->trigger();
   }
   else
   {
      setCurrentIndex( PageId2Index( piGraph ) );
   }
   
   Diagnostic::TUnitKeys keys( selectionModel()->selected() );
   
   meterGraphManager()->OpenGraphsInView( keys );
}

ECU::CDevice& CECUDeviceView::ecuDevice()const
{
   return *_ecuDevice;
}

CMeterGraphManager* CECUDeviceView::meterGraphManager()const
{
   return _meterGraphManager;
}

void CECUDeviceView::OnCloseGraph()
{
   TRACE_FUN( Routine, "CECUDeviceView::OnCloseGraph" );
   
   meterGraphManager()->Close( selectionModel()->current() );
}

void CECUDeviceView::OnCloseAllGraph()
{
   TRACE_FUN( Routine, "CECUDeviceView::OnCloseAllGraph" );
   
   meterGraphManager()->CloseAll();
}

bool CECUDeviceView::IsReadyToClose()const
{
   return _readyToClose;
}

void CECUDeviceView::closeEvent( QCloseEvent* anEvent )
{
   TRACE_FUN( Routine, "CECUDeviceView::closeEvent" );

   if( _ecuDevice->isInitialized() )
   {
      OnStartStopDiagnosticSession();

      anEvent->ignore();
   }
   else
   {
      QTabWidget::closeEvent( anEvent );
   }
}

void CECUDeviceView::hideEvent( QHideEvent* anEvent )
{
   QTabWidget::hideEvent( anEvent );
}
