/*
 * AbstractMeterView.cpp
 *
 *  Created on: 05.03.2009
 *      Author: mars
 */

#include "AbstractMeterView.h"
#include "AbstractMeterModel.h"

#include <ECUDeviceView.h>
#include <ECUDeviceUnitSelectionModel.h>

#include <l2Trace.h>

CAbstractMeterView::CAbstractMeterView( const Diagnostic::CUnitKey& aKey, CECUDeviceView& aView ) :
   QGLWidget( QGLFormat( QGL::DoubleBuffer | QGL::Rgba ), &aView ),
   _key( aKey ),
   _deviceView( aView )
{
   TRACE_FUN( Routine, "CAbstractMeterView::CAbstractMeterView" );
   
   _model = 0;

   setFocusPolicy( Qt::ClickFocus );
}

CAbstractMeterView::~CAbstractMeterView()
{
   TRACE_FUN( Routine, "CAbstractMeterView::~CAbstractMeterView" );
   
   delete _model;
}

void CAbstractMeterView::initializeGL()
{
   QGLWidget::initializeGL();

   glEnable( GL_BLEND );
   glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
}

void CAbstractMeterView::paintGL()
{
   QGLWidget::paintGL();

   if( key() == deviceView().selectionModel()->current() )
   {
      glClearColor( 1., 1., 1., .0 );
   }
   else
   {
      glClearColor( .95, .95, .97, .0 );
   }

   glClear( GL_COLOR_BUFFER_BIT );
}

void CAbstractMeterView::resizeGL( int aWidth, int aHeight ){}

void CAbstractMeterView::paintEvent( QPaintEvent* anEvent )
{
   QGLWidget::paintEvent( anEvent );
}

void CAbstractMeterView::SetModel( CAbstractMeterModel* aModel )
{
   delete _model;

   _model = aModel;
}

CAbstractMeterModel* CAbstractMeterView::model()const
{
   return _model;
}

void CAbstractMeterView::SaveGLState()
{
   glPushAttrib( GL_ALL_ATTRIB_BITS );

   glMatrixMode( GL_PROJECTION );
   glPushMatrix();

   glMatrixMode( GL_MODELVIEW );
   glPushMatrix();
}

void CAbstractMeterView::RestoreGLState()
{
   glPopAttrib();

   glMatrixMode( GL_PROJECTION );
   glPopMatrix();

   glMatrixMode( GL_MODELVIEW );
   glPopMatrix();
}

const CECUDeviceView& CAbstractMeterView::deviceView()const
{
   return _deviceView;
}

void CAbstractMeterView::Activate()
{
   Diagnostic::TUnitKeys keys;
   keys.insert( key() );

   deviceView().selectionModel()->Select( keys );
   deviceView().selectionModel()->SetCurrent( key() );
}

const Diagnostic::CUnitKey& CAbstractMeterView::key()const
{
   return _key;
}

void CAbstractMeterView::modelUpdated(){}
