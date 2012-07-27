/*
 * ECUDiagnosticGraphView.cpp
 *
 *  Created on: 22.09.2009
 *      Author: mars
 */

#include "ECUDiagnosticGraphView.h"
#include "ECUDiagnosticGraphViewLayout.h"

#include <AbstractMeterView.h>

#include <l2Trace.h>

#include <QMouseEvent>
#include <QPainter>
#include <QBrush>
#include <QPalette>

CECUDiagnosticGraphView::CECUDiagnosticGraphView( QWidget* aParent ) : QWidget( aParent )
{
   CECUDiagnosticGraphViewLayout* theLayout( new CECUDiagnosticGraphViewLayout() );
   
   setLayout( theLayout );
   setMouseTracking( true );

   QPalette thePalette( palette() );
   thePalette.setBrush( QPalette::Active, QPalette::Window, QColor( 100, 100, 100 ) );
   thePalette.setBrush( QPalette::Inactive, QPalette::Window, QColor( 100, 100, 100 ) );

   setPalette( thePalette );
}

CECUDiagnosticGraphView::~CECUDiagnosticGraphView(){}

CECUDiagnosticGraphViewLayout* CECUDiagnosticGraphView::layout()
{
   return static_cast< CECUDiagnosticGraphViewLayout* >( QWidget::layout() );
}

void CECUDiagnosticGraphView::InsertGraph( CAbstractMeterView* aView )
{
   TRACE_FUN( Routine, "CECUDiagnosticGraphView::InsertGraph" );
   
   aView->setMouseTracking( true );
   aView->setMinimumSize( 270, 140 );
   aView->setMaximumSize( 270, 140 );
   
   layout()->AddView( aView );
}

bool CECUDiagnosticGraphView::event( QEvent* anEvent)
{
   if( anEvent->type() == QEvent::MouseMove )
   {
      QMouseEvent* mouseEvent( static_cast< QMouseEvent* >( anEvent ) );

      CAbstractMeterView* view( layout()->Widget2View( childAt( mouseEvent->pos() ) ) );

      if( view )
      {
         view->Activate();
      }
   }

   return QWidget::event( anEvent );
}

void CECUDiagnosticGraphView::paintEvent( QPaintEvent* anEvent )
{
   if( layout()->count() )
   {
      QWidget::paintEvent( anEvent );
   }
   else
   {
      QPainter painter( this );

      QFont f( font() );
      f.setPointSize( 10 );

      painter.setFont( f );
      painter.setRenderHint( QPainter::TextAntialiasing );

      painter.drawText( rect(), Qt::AlignCenter | Qt::TextWordWrap, "There no graphs to view." );
   }
}
