/*
 * MeterGraphWindow.cpp
 *
 *  Created on: 07.03.2009
 *      Author: L2-Max
 */

#include "MeterGraphWindow.h"
#include "MeterGraphManager.h"
#include "Meter2DGraphView.h"
#include "Meter2DGraphModel.h"

#include <ECUDeviceView.h>
#include <ECUDevice.h>
#include <ECUUnitsTable.h>
#include <ECUDataUnit.h>
#include <AppContext.h>
#include <AppDefs.h>

#include <l2Trace.h>

#include <QVBoxLayout>
#include <QToolBar>
#include <QTimeEdit>
#include <QSpinBox>
#include <QEvent>

CMeterGraphWindow::CMeterGraphWindow( CECUDeviceView& aView ) :
   QMainWindow( &aView, Qt::Tool ),
   _deviceView( aView )
{
   _view = 0;
   
   setMinimumSize( 320, 175 );

   _timeFrame = new QTimeEdit( this );
   _timeFrame->setDisplayFormat( "mm \'min\': ss \'sec\'" );
   _timeFrame->setTimeRange( QTime( 0, 0, 5 ), QTime( 0, 59, 59 ) );

   _timeResolution = new QSpinBox( this );
   _timeResolution->setRange( 2, 10 );

   _valueResolution = new QSpinBox( this );
   _valueResolution->setRange( 2, 10 );

   QToolBar* toolBar( new QToolBar( this ) );

   toolBar->addWidget( _timeFrame );
   toolBar->addWidget( _timeResolution );
   toolBar->addWidget( _valueResolution );

   addToolBar( toolBar );

   connect( _timeFrame, SIGNAL( timeChanged( const QTime& ) ), SLOT( OnTimeFrameChanged( const QTime& ) ) );
   connect( _timeResolution, SIGNAL( valueChanged( int ) ), SLOT( OnTimeResolutionChanged( int ) ) );
   connect( _valueResolution, SIGNAL( valueChanged( int ) ), SLOT( OnValueResolutionChanged( int ) ) );
}

CMeterGraphWindow::~CMeterGraphWindow()
{
   TRACE_FUN( Routine, "CMeterGraphWindow::~CMeterGraphWindow" );
   
   SetView( 0 );
}

void CMeterGraphWindow::SetView( CMeter2DGraphView* aView )
{
   TRACE_FUN( Routine, "CMeterGraphWindow::SetView" );
   
   if( _view && _view != aView )
   {
      CAppContext::GetInstance()->MainRegistry().Write( _view->model()->registryKey() + V_CMN_X, x() );
      CAppContext::GetInstance()->MainRegistry().Write( _view->model()->registryKey() + V_CMN_Y, y() );
      CAppContext::GetInstance()->MainRegistry().Write( _view->model()->registryKey() + V_CMN_WIDTH, width() );
      CAppContext::GetInstance()->MainRegistry().Write( _view->model()->registryKey() + V_CMN_HEIGHT, height() );
      
      _view->setParent( 0 );
   }
   
   _view = aView;
   
   if( _view )
   {
      ECU::HDataUnit unit( _deviceView.ecuDevice().unitsTable().data( _view->key() ) );

      _view->setParent( 0 );
      _view->setMouseTracking( false );
      _view->setMinimumSize( 0, 0 );
      _view->setMaximumSize( QWIDGETSIZE_MAX, QWIDGETSIZE_MAX );
      
      setWindowTitle( std::string( unit->name() + " - graph view" ).c_str() );
      setCentralWidget( _view );
      
      l2max::CVariant xPos;
      l2max::CVariant yPos;
      l2max::CVariant widthPos;
      l2max::CVariant heightPos;
   
      CAppContext::GetInstance()->MainRegistry().Read( _view->model()->registryKey() + V_CMN_X, xPos );
      CAppContext::GetInstance()->MainRegistry().Read( _view->model()->registryKey() + V_CMN_Y, yPos );
      CAppContext::GetInstance()->MainRegistry().Read( _view->model()->registryKey() + V_CMN_WIDTH, widthPos );
      CAppContext::GetInstance()->MainRegistry().Read( _view->model()->registryKey() + V_CMN_HEIGHT, heightPos );
   
      if( xPos.isValid() && yPos.isValid() )
      {
         move( xPos.toInt(), yPos.toInt() );
      }
   
      if( widthPos.isValid() && heightPos.isValid() )
      {
         resize( widthPos.toInt(), heightPos.toInt() );
      }
      
      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      _timeFrame->setTime( QTime().addSecs( int( _view->model()->timeFrame() ) ) );
      
      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      _timeResolution->setValue( _view->model()->timeResolution() );
      
      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      _valueResolution->setValue( _view->model()->valueResolution() );
   }
}

void CMeterGraphWindow::OnTimeFrameChanged( const QTime& aTime )
{
   TRACE_FUN( Routine, "CMeterGraphWindow::OnTimeFrameChanged" );

   _view->model()->SetTimeFrame( aTime.hour() * 3600 + aTime.minute() * 60 + aTime.second() );
}

void CMeterGraphWindow::OnTimeResolutionChanged( int aValue )
{
   TRACE_FUN( Routine, "CMeterGraphWindow::OnTimeResolutionChanged" );

   _view->model()->SetTimeResolution( aValue );
}

void CMeterGraphWindow::OnValueResolutionChanged( int aValue )
{
   TRACE_FUN( Routine, "CMeterGraphWindow::OnTimeResolutionChanged" );

   _view->model()->SetValueResolution( aValue );
}

void CMeterGraphWindow::closeEvent( QCloseEvent* anEvent )
{
   QMainWindow::closeEvent( anEvent );
   
   _deviceView.meterGraphManager()->Close( _view->key() );
}

bool CMeterGraphWindow::event( QEvent* anEvent )
{
   bool ret( false );

   if( anEvent->type() == QEvent::WindowActivate )
   {
      if( isActiveWindow() )
      {
         if( _view )
         {
            _view->Activate();
         }
      }

      ret = QMainWindow::event( anEvent );
   }
   else
   {
      ret = QMainWindow::event( anEvent );
   }

   return ret;
}
