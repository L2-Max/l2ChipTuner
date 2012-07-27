/*
 * Meter2DGraphView.cpp
 *
 *  Created on: 07.03.2009
 *      Author: L2-Max
 */

#include "Meter2DGraphView.h"
#include "Meter2DGraphModel.h"

#include <ECUDataUnit.h>
#include <ECUDeviceView.h>
#include <ECUDevice.h>
#include <ECUUnitsTable.h>
#include <MainWindow.h>

#include <l2Trace.h>

#include <QPainter>
#include <QColor>
#include <QTimerEvent>
#include <QContextMenuEvent>

#include <math.h>

const unsigned int CMeter2DGraphView::_timerInterval( 66 );

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct CMeter2DGraphView::SRaster
{
   int _x;
   int _y;
   
   QImage _image;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CMeter2DGraphView::CMeter2DGraphView( const Diagnostic::CUnitKey& aKey, CECUDeviceView& aView ) :
   CAbstractMeterView( aKey, aView ),
   _popupMenu( this )
{
   TRACE_FUN( Routine, "CMeter2DGraphView::CMeter2DGraphView" );
   
   _timerId = -1;
   
   _normalFont = aView.font();
   _boldFont = _normalFont;
   _boldFont.setBold( true );
   
   _bkgValueFont = _normalFont;
   _bkgValueFont.setPointSize( 32 );

   _bkgUnitFont = _normalFont;
   _bkgUnitFont.setPointSize( 10 );
   _bkgUnitFont.setBold( true );
   
   _menuSessionId = deviceView().mainWindow().menuBarManager().OpenSession();
   
   SetModel( new CMeter2DGraphModel( key(), aView, *this ) );

   setAutoFillBackground( false );
   
   _timerId = startTimer( CMeter2DGraphView::_timerInterval );
}

CMeter2DGraphView::~CMeter2DGraphView()
{
   TRACE_FUN( Routine, "CMeter2DGraphView::~CMeter2DGraphView" );
   
   killTimer( _timerId );
   
   glDeleteLists( _meshList, 1 );
   gluDeleteNurbsRenderer( _nurbsRenderer );
   
   deviceView().mainWindow().menuBarManager().CloseSession( _menuSessionId );
}

void CMeter2DGraphView::initializeGL()
{
   TRACE_FUN( Routine, "CMeter2DGraphView::initializeGL" );

   CAbstractMeterView::initializeGL();
   
   _meshList = glGenLists( 1 );
   
   _nurbsRenderer = gluNewNurbsRenderer();
   
   gluNurbsProperty( _nurbsRenderer, GLU_SAMPLING_TOLERANCE, 5. );
   gluNurbsProperty( _nurbsRenderer, GLU_DISPLAY_MODE, GLU_FILL );
}

void CMeter2DGraphView::paintGL()
{
   TRACE_FUN( Frequently, "CMeter2DGraphView::paintGL" );
   
   CAbstractMeterView::paintGL();

   if( model()->hasValues() )
   {
      DrawTitles();
      DrawMesh();
      DrawValueGraph();
   }
}

void CMeter2DGraphView::paintEvent( QPaintEvent* anEvent )
{
   TRACE_FUN( Frequently, "CMeter2DGraphView::paintEvent" );

   if( !model()->hasValues() )
   {
      QPainter painter( this );
      
      QFont f( font() );
      f.setPointSize( 13 );

      painter.setFont( f );
      painter.setRenderHint( QPainter::TextAntialiasing );

      painter.drawText( rect(), Qt::AlignCenter | Qt::TextWordWrap, "There no values to view. Refresh data units first." );
   }
   else
   {
      CAbstractMeterView::paintEvent( anEvent );
   }
}

void CMeter2DGraphView::timerEvent( QTimerEvent* anEvent )
{
   if( model() && anEvent->timerId() == _timerId )
   {
      model()->UpdateTimeOffset( CMeter2DGraphView::_timerInterval );

      updateGL();
   }
}

void CMeter2DGraphView::contextMenuEvent( QContextMenuEvent* anEvent )
{
   TRACE_FUN( Routine, "CMeter2DGraphView::contextMenuEvent" );
   
   CAbstractMeterView::contextMenuEvent( anEvent );
   
   Activate();

   deviceView().UpdateMeterMenu( true );
   
   _popupMenu.clear();
   
   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
   QMenu* measureUnitsMenu( deviceView().mainWindow().menuBarManager().menu( CMenuBarManager::mMeterMeasureUnits )->menu() );

   if( !measureUnitsMenu->isEmpty() )
   {
      _popupMenu.addMenu( measureUnitsMenu );
   }

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
   _popupMenu.addAction( deviceView().mainWindow().menuBarManager().menu( CMenuBarManager::mMeterGraph ) );

   if( !_popupMenu.isEmpty() )
   {
      _popupMenu.popup( anEvent->globalPos() );
   }
}

CMeter2DGraphModel* CMeter2DGraphView::model()const
{
   return static_cast< CMeter2DGraphModel* >( CAbstractMeterView::model() );
}

void CMeter2DGraphView::DrawTitles()
{
   TRACE_FUN( Frequently, "CMeter2DGraphView::DrawTitles" );
   
   ////////////////////////////////////////////////////////////////////////////////////////////////
   glViewport( 0, 0, width(), height() );
   
   glMatrixMode( GL_PROJECTION );
   glLoadIdentity();

   gluOrtho2D( 0, width(), 0, height() );

   glMatrixMode( GL_MODELVIEW );
   glLoadIdentity();
   
   glShadeModel( GL_FLAT );
   glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

   for( TRasterContainer::iterator I_raster( _rasterContainer.begin() );
         I_raster != _rasterContainer.end(); ++I_raster )
   {
      glRasterPos2i( I_raster->second._x, height() - I_raster->second._y );

      glDrawPixels( I_raster->second._image.width(), I_raster->second._image.height(), GL_RGBA,
                    GL_UNSIGNED_BYTE, I_raster->second._image.bits() );
   }
}

void CMeter2DGraphView::DrawMesh()
{
   TRACE_FUN( Frequently, "CMeter2DGraphView::DrawMesh" );
   
   glCallList( _meshList );
}

void CMeter2DGraphView::DrawValueGraph()
{
   TRACE_FUN( Frequently, "CMeter2DGraphView::DrawValueGraph" );
   
   float meshWidth( model()->timeFrame() );
   float meshHeight( model()->valueFrame() );

   glViewport( _meshArea.x(), _meshArea.y(), _meshArea.width(), _meshArea.height() );

   glMatrixMode( GL_PROJECTION );
   glLoadIdentity();

   gluOrtho2D( 0, meshWidth, 0, meshHeight );

   glMatrixMode( GL_MODELVIEW );
   glLoadIdentity();

   glEnable( GL_LINE_SMOOTH );
   glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );

   glColor4f( .0, .0, .0, .7 );
   glLineWidth( width() / 400 );

   const CMeter2DGraphModel::TValueTimeContainer& valueTimeContainer( model()->valueTimeContainer() );
   
   int knotCount( valueTimeContainer.size() * 4 + 4 );
   int pointCount( valueTimeContainer.size() * 4 * 3 );

   float* knots( new float[ knotCount ] );
   float* cpoints( new float[ pointCount + 12 ] );

   int knot( 0 );
   int timeOffset( int( rint( model()->timeOffset() ) ) );

   for( CMeter2DGraphModel::TValueTimeContainer::const_iterator I_value( valueTimeContainer.begin() );
         I_value != valueTimeContainer.end(); )
   {
      knots[ knot * 4 + 0 ] = knot;
      knots[ knot * 4 + 1 ] = knot;
      knots[ knot * 4 + 2 ] = knot;
      knots[ knot * 4 + 3 ] = knot;


      float xPoint( ( int( I_value->second ) + timeOffset ) / 1000. );

      cpoints[ knot * 4 * 3 + 0 ] = xPoint;
      cpoints[ knot * 4 * 3 + 1 ] = I_value->first;
      cpoints[ knot * 4 * 3 + 2 ] = 0;

      cpoints[ knot * 4 * 3 + 3 ] = xPoint;
      cpoints[ knot * 4 * 3 + 4 ] = I_value->first;
      cpoints[ knot * 4 * 3 + 5 ] = 0;

      cpoints[ knot * 4 * 3 + 6 ] = xPoint;
      cpoints[ knot * 4 * 3 + 7 ] = I_value->first;
      cpoints[ knot * 4 * 3 + 8 ] = 0;

      cpoints[ knot * 4 * 3 + 9 ] = xPoint;
      cpoints[ knot * 4 * 3 + 10 ] = I_value->first;
      cpoints[ knot * 4 * 3 + 11 ] = 0;

      ++I_value;

      if( I_value != valueTimeContainer.end() )
      {
         float xPoint( ( int( I_value->second ) + timeOffset ) / 1000. );

         cpoints[ knot * 4 * 3 + 6 ] = cpoints[ knot * 4 * 3 + 3 ];
         cpoints[ knot * 4 * 3 + 3 ] = xPoint;

         cpoints[ knot * 4 * 3 + 7 ] = cpoints[ knot * 4 * 3 + 4 ];
         cpoints[ knot * 4 * 3 + 4 ] = I_value->first;


         cpoints[ knot * 4 * 3 + 8 ] = 0;

         ///////////////////////////////////////////////////////////////////////////////////
         float xcorr( ( cpoints[ knot * 4 * 3 + 3 ] - cpoints[ knot * 4 * 3 + 6 ] ) * .6 );

         cpoints[ knot * 4 * 3 + 3 ] -= xcorr;
         cpoints[ knot * 4 * 3 + 6 ] += xcorr;

         float ycorr( ( cpoints[ knot * 4 * 3 + 4 ] - cpoints[ knot * 4 * 3 + 7 ] ) * 1. );

         cpoints[ knot * 4 * 3 + 4 ] -= ycorr;
         cpoints[ knot * 4 * 3 + 7 ] += ycorr;

         ///////////////////////////////////////////////////////////////////////////////////
         cpoints[ knot * 4 * 3 + 9 ] = xPoint;
         cpoints[ knot * 4 * 3 + 10 ] = I_value->first;
         cpoints[ knot * 4 * 3 + 11 ] = 0;
      }

      ++knot;
   }

   knots[ knot * 4 + 0 ] = knot;
   knots[ knot * 4 + 1 ] = knot;
   knots[ knot * 4 + 2 ] = knot;
   knots[ knot * 4 + 3 ] = knot;

   gluBeginCurve( _nurbsRenderer );

   gluNurbsCurve( _nurbsRenderer, knotCount, knots, 3, cpoints, 4, GL_MAP1_VERTEX_3 );

   gluEndCurve( _nurbsRenderer );

   delete[] cpoints;
   delete[] knots;
}

void CMeter2DGraphView::timeFrameChanged()
{
   if( model() )
   {
      UpdateTimeTitle();
      UpdateMesh();
   }
}

void CMeter2DGraphView::valueFrameChanged()
{
   if( model() )
   {
      UpdateValueTitle();
      UpdateMesh();
   }
}

void CMeter2DGraphView::valueChanged()
{
   if( model() )
   {
      UpdateValue();
   }
}

void CMeter2DGraphView::resizeGL( int aWidth, int aHeight )
{
   TRACE_FUN( Frequently, "CMeter2DGraphView::resizeGL" );
   
   CAbstractMeterView::resizeGL( aWidth, aHeight );
   
   if( model() )
   {
      UpdateTimeAxisTitle();
      UpdateValueAxisTitle();
      
      UpdateValueTitle();
      UpdateTimeTitle();
      
      UpdateUnitTitle();
      UpdateValue();
      
      UpdateMesh();
   }
}

void CMeter2DGraphView::UpdateValueAxisTitle()
{
   TRACE_FUN( Frequently, "CMeter2DGraphView::UpdateValueAxisTitle" );
   
   QString valueAxisTitle( QString::fromUtf8( model()->valueAxisTitle().c_str() ) );
   QRect valueAxisTitleRect( QFontMetrics( _boldFont ).boundingRect( 0, 0, _meshArea.height(), 0, Qt::AlignCenter | Qt::TextWordWrap,
                                                                     valueAxisTitle ) );

   _meshArea.setLeft( 0 );
   _meshArea.adjust( valueAxisTitleRect.height(), 0, 0, 0 );
   
   SRaster& raster( _rasterContainer[ CMeter2DGraphView::rtValueAxisTitle ] );
   
   raster._x = _meshArea.x() - valueAxisTitleRect.height();
   raster._y = _meshArea.y() + _meshArea.height() / 2 + valueAxisTitleRect.width() / 2;
   raster._image = QImage( valueAxisTitleRect.height(), valueAxisTitleRect.width(), QImage::Format_ARGB32 );
   
   QPainter painter;
   
   InitializeRasterRenderer( painter, raster._image );
   
   painter.setFont( _boldFont );
   
   painter.rotate( -90 );

   painter.drawText( -valueAxisTitleRect.width(), 0,
                     valueAxisTitleRect.width(), valueAxisTitleRect.height(),
                     Qt::AlignCenter | Qt::TextWordWrap, valueAxisTitle );
}

void CMeter2DGraphView::UpdateTimeAxisTitle()
{
   TRACE_FUN( Frequently, "CMeter2DGraphView::UpdateTimeAxisTitle" );
   
   QString timeAxisTitle( QString::fromUtf8( model()->timeAxisTitle().c_str() ) );
   QRect timeAxisTitleRect( QFontMetrics( _boldFont ).boundingRect( 0, 0, _meshArea.width(), 0, Qt::AlignCenter | Qt::TextWordWrap,
                                                                    timeAxisTitle ) );
   
   _meshArea.setBottom( height() );
   _meshArea.adjust( 0, 0, 0, -timeAxisTitleRect.height() - 1 );
   
   SRaster& raster( _rasterContainer[ CMeter2DGraphView::rtTimeAxisTitle ] );
   raster._x = _meshArea.x() + _meshArea.width() / 2 - timeAxisTitleRect.width() / 2;
   raster._y = _meshArea.y() + _meshArea.height() + timeAxisTitleRect.height();
   raster._image = QImage( timeAxisTitleRect.width(), timeAxisTitleRect.height(), QImage::Format_ARGB32 );

   QPainter painter;
   
   InitializeRasterRenderer( painter, raster._image );
   
   painter.setFont( _boldFont );

   painter.drawText( 0, 0, timeAxisTitleRect.width(), timeAxisTitleRect.height(), Qt::AlignCenter | Qt::TextWordWrap, timeAxisTitle );
}

void CMeter2DGraphView::UpdateValueTitle()
{
   TRACE_FUN( Frequently, "CMeter2DGraphView::UpdateValueTitle" );
   
   typedef std::list< int > THeightContainer;
   THeightContainer heightContainer;
   
   int maxValueTitleWidth( 0 );

   const CMeter2DGraphModel::TValueTitleContainer& valueTitleContainer( model()->valueTitleContainer() );

   for( CMeter2DGraphModel::TValueTitleContainer::const_iterator I_value( valueTitleContainer.begin() );
         I_value != valueTitleContainer.end(); ++I_value )
   {
      QSize valueTitleSize( QFontMetrics( _normalFont ).size( Qt::AlignCenter, I_value->c_str() ) );
      valueTitleSize.setWidth( valueTitleSize.width() + 2 );

      heightContainer.push_back( valueTitleSize.height() );

      if( maxValueTitleWidth < valueTitleSize.width() )
      {
         maxValueTitleWidth = valueTitleSize.width();
      }
   }
   
   _meshArea.setRight( width() );
   _meshArea.adjust( 0, 0, -maxValueTitleWidth - 1, 0 );
   
   SRaster& raster( _rasterContainer[ CMeter2DGraphView::rtValueTitles ] );
   
   raster._x = _meshArea.x() + _meshArea.width();
   raster._y = _meshArea.y() + _meshArea.height();
   raster._image = QImage( maxValueTitleWidth, _meshArea.height(), QImage::Format_ARGB32 );
   
   QPainter painter;
   
   InitializeRasterRenderer( painter, raster._image );
   
   painter.setFont( _normalFont );
   
   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   const CMeter2DGraphModel::TValueTitleContainer& titleContainer( model()->valueTitleContainer() );

   THeightContainer::iterator I_height( heightContainer.begin() );
   CMeter2DGraphModel::TValueTitleContainer::const_iterator I_title( titleContainer.begin() );
   
   float resolution( float( raster._image.height() ) / model()->valueResolution() );
   
   for( float I_y( resolution );
         I_y < raster._image.height() - resolution / 2 &&
         I_height != heightContainer.end() &&
         I_title != titleContainer.end(); I_y += resolution, ++I_height, ++I_title )
   {
      painter.drawText( 1, int( I_y ) - *I_height / 2,
                        maxValueTitleWidth, *I_height,
                        Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap, I_title->c_str() );
   }
}

void CMeter2DGraphView::UpdateTimeTitle()
{
   TRACE_FUN( Frequently, "CMeter2DGraphView::UpdateTimeTitle" );
   
   typedef std::list< int > TWidthContainer;
   TWidthContainer widthContainer;
   
   int maxTimeTitleHeight( 0 );

   const CMeter2DGraphModel::TTimeTitleContainer& timeTitleContainer( model()->timeTitleContainer() );

   for( CMeter2DGraphModel::TTimeTitleContainer::const_iterator I_time( timeTitleContainer.begin() );
         I_time != timeTitleContainer.end(); ++I_time )
   {
      QSize timeTitleSize( QFontMetrics( _normalFont ).size( Qt::AlignCenter, I_time->c_str() ) );

      widthContainer.push_back( timeTitleSize.width() );

      if( maxTimeTitleHeight < timeTitleSize.height() )
      {
         maxTimeTitleHeight = timeTitleSize.height();
      }
   }
   
   _meshArea.setTop( 0 );
   _meshArea.adjust( 0, maxTimeTitleHeight, 0, 0 );
   
   SRaster& raster( _rasterContainer[ CMeter2DGraphView::rtTimeTitles ] );
   
   raster._x = _meshArea.x();
   raster._y = _meshArea.y();
   raster._image = QImage( _meshArea.width(), maxTimeTitleHeight, QImage::Format_ARGB32 );
   
   QPainter painter;
   
   InitializeRasterRenderer( painter, raster._image );
   
   painter.setFont( _normalFont );
   
   const CMeter2DGraphModel::TTimeTitleContainer& titleContainer( model()->timeTitleContainer() );

   TWidthContainer::iterator I_rect( widthContainer.begin() );
   CMeter2DGraphModel::TTimeTitleContainer::const_iterator I_title( titleContainer.begin() );
   
   float resolution( float( raster._image.width() ) / model()->timeResolution() );
   
   int lastEndPosition( -1 );
   
   for( float I_x( resolution );
         I_x < ( raster._image.width() - resolution / 2 ) &&
         I_rect != widthContainer.end() &&
         I_title != titleContainer.end(); I_x += resolution, ++I_rect, ++I_title )
   {
      if( lastEndPosition == -1 || lastEndPosition < int( I_x ) )
      {
         lastEndPosition = int( I_x + *I_rect + 5 );

         painter.drawText( int( I_x ) - *I_rect / 2, 0,
                           *I_rect, maxTimeTitleHeight,
                           Qt::AlignCenter | Qt::TextWordWrap, I_title->c_str() );
      }
   }
}

void CMeter2DGraphView::UpdateUnitTitle()
{
   TRACE_FUN( Frequently, "CMeter2DGraphView::UpdateUnitTitle" );
   
   QString bkgUnit( QString::fromUtf8( model()->unit().c_str() ) );
   QRect bkgUnitRect( QFontMetrics( _bkgUnitFont ).boundingRect( _meshArea, Qt::AlignCenter | Qt::TextWordWrap, bkgUnit ) );
   
   SRaster& raster( _rasterContainer[ CMeter2DGraphView::rtUnitTitle ] );
   
   raster._x = _meshArea.x() + 5;
   raster._y = _meshArea.y() + bkgUnitRect.height();
   raster._image = QImage( bkgUnitRect.width(), bkgUnitRect.height(), QImage::Format_ARGB32 );
   
   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   QPainter painter;
   
   InitializeRasterRenderer( painter, raster._image );
   
   painter.setFont( _bkgUnitFont );
   painter.setOpacity( .7 );

   painter.drawText( 0, 0, bkgUnitRect.width(), bkgUnitRect.height(),
                     Qt::AlignCenter | Qt::TextWordWrap, bkgUnit );
}

void CMeter2DGraphView::UpdateValue()
{
   TRACE_FUN( Frequently, "CMeter2DGraphView::UpdateValue" );
   
   QString bkgValue( QString::fromUtf8( model()->text().c_str() ) );
   QRect bkgValueRect( QFontMetrics( _bkgValueFont ).boundingRect( _meshArea, Qt::AlignCenter | Qt::TextWordWrap, bkgValue ) );

   SRaster& raster( _rasterContainer[ CMeter2DGraphView::rtValue ] );
   
   raster._x = _meshArea.x() + _meshArea.width() / 2 - bkgValueRect.width() / 2;
   raster._y = _meshArea.y() + _meshArea.height() / 2 + bkgValueRect.height() / 2;
   raster._image = QImage( bkgValueRect.width(), bkgValueRect.height(), QImage::Format_ARGB32 );
   
   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   QPainter painter;
   
   InitializeRasterRenderer( painter, raster._image );
   
   painter.setFont( _bkgValueFont );
   painter.setOpacity( .4 );
   
   painter.drawText( 0, 0, bkgValueRect.width(), bkgValueRect.height(),
                     Qt::AlignCenter | Qt::TextWordWrap, bkgValue );
}

void CMeter2DGraphView::UpdateMesh()
{
   makeCurrent();
   
   glNewList( _meshList, GL_COMPILE );
   
   /////////////////////////////////////////////////////////////////////////////////////////
   float meshWidth( model()->timeFrame() );
   float meshHeight( model()->valueFrame() );
   float meshTimeResolution( meshWidth / model()->timeResolution() );
   float meshValueResolution( meshHeight / model()->valueResolution() );
   
   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   QRect glRect( _meshArea.x(), height() - ( _meshArea.y() + _meshArea.height() ),
                 _meshArea.width(), _meshArea.height() );
   
   glViewport( glRect.x(), glRect.y(), glRect.width(), glRect.height() );

   glMatrixMode( GL_PROJECTION );
   glLoadIdentity();

   gluOrtho2D( 0, meshWidth, 0, meshHeight );
   
   glMatrixMode( GL_MODELVIEW );
   glLoadIdentity();
   
   /////////////////////////////////////////////////////////////////////////////////////////
   glDisable( GL_LINE_SMOOTH );

   /////////////////////////////////////////////////////////////////////////////////////////
   glLineWidth( 2. );
   glColor4f( .1, .3, .3, .5 );

   glBegin( GL_LINES );

   glVertex2f( .0, .0 );
   glVertex2f( .0, meshHeight );

   glVertex2f( .0, .0 );
   glVertex2f( meshWidth, .0 );

   glEnd();

   /////////////////////////////////////////////////////////////////////////////////////////
   glLineWidth( 1. );
   glColor4f( .5, .8, .5, .5 );

   glBegin( GL_LINES );

   /////////////////////////////////////////////////////////////////////////////////////////
   for( float x( meshTimeResolution ); x < meshWidth - meshTimeResolution / 2; x += meshTimeResolution )
   {
      glVertex2f( x, 0 );
      glVertex2f( x, meshHeight );
   }

   /////////////////////////////////////////////////////////////////////////////////////////
   for( float y( meshValueResolution ); y < meshHeight - meshValueResolution / 2; y += meshValueResolution )
   {
      glVertex2f( 0, y );
      glVertex2f( meshWidth, y );
   }

   glEnd();
   
   glEndList();
}

void CMeter2DGraphView::InitializeRasterRenderer( QPainter& aPainter, QImage& anImage )
{
   anImage.fill( 0x00000000 );
   
   aPainter.begin( &anImage );
   aPainter.setViewport( 0, 0, anImage.width(), anImage.height() );
   aPainter.setWindow( 0, anImage.height(), anImage.width(), -anImage.height() );
   aPainter.setRenderHint( QPainter::HighQualityAntialiasing );
}
