/*
 * ECUDiagnosticGraphViewLayout.cpp
 *
 *  Created on: 09.10.2009
 *      Author: mars
 */

#include "ECUDiagnosticGraphViewLayout.h"

#include <AbstractMeterView.h>

#include <l2Trace.h>

CECUDiagnosticGraphViewLayout::CECUDiagnosticGraphViewLayout()
{
   setSpacing( 3 );
}

CECUDiagnosticGraphViewLayout::~CECUDiagnosticGraphViewLayout()
{
   for( QLayoutItem* I_item( takeAt( 0 ) ); I_item; I_item = takeAt( 0 ) )
   {
      delete I_item;
   }
}

void CECUDiagnosticGraphViewLayout::addItem( QLayoutItem* anItem )
{
   _itemContainer.append( anItem );
}

void CECUDiagnosticGraphViewLayout::setGeometry( const QRect& aRect )
{
   QLayout::setGeometry( aRect );

   doLayout( aRect, false );
}

bool CECUDiagnosticGraphViewLayout::hasHeightForWidth()const
{
   return true;
}

int CECUDiagnosticGraphViewLayout::heightForWidth( int aWidth )const
{
   return doLayout( QRect( 0, 0, aWidth, 0 ), true );
}

int CECUDiagnosticGraphViewLayout::count()const
{
   return _itemContainer.size();
}

Qt::Orientations CECUDiagnosticGraphViewLayout::expandingDirections()const
{
   return 0;
}

QLayoutItem* CECUDiagnosticGraphViewLayout::itemAt( int anIndex )const
{
   return _itemContainer.value( anIndex );
}

QSize CECUDiagnosticGraphViewLayout::minimumSize()const
{
   QSize ret;
   
   for( TItemContainer::const_iterator I_item( _itemContainer.begin() ); I_item != _itemContainer.end(); ++I_item )
   {
      ret = ret.expandedTo( ( *I_item )->minimumSize() );
   }
   
   ret += QSize( 2 * margin(), 2 * margin() );
   
   return ret;

}

QSize CECUDiagnosticGraphViewLayout::sizeHint()const
{
   return minimumSize();
}

QLayoutItem* CECUDiagnosticGraphViewLayout::takeAt( int anIndex )
{
   QLayoutItem* ret( 0 );
   
   if( anIndex >= 0 && anIndex < _itemContainer.size() )
   {
      ret =  _itemContainer.takeAt( anIndex );

      _viewContainer.erase( Widget2View( ret->widget() ) );
   }
   
   return ret;
}

int CECUDiagnosticGraphViewLayout::doLayout( const QRect& aRect, bool anIsTestOnly )const
{
   TRACE_FUN( Routine, "CECUDiagnosticGraphViewLayout::doLayout" );
   
   int x( aRect.x() );
   int y( aRect.y() );
   int lineHeight( 0 );

   for( TItemContainer::const_iterator I_item( _itemContainer.begin() ); I_item != _itemContainer.end(); ++I_item )
   {
      int nextX( x + ( *I_item )->sizeHint().width() + spacing() );
      
      if( nextX - spacing() > aRect.right() && lineHeight > 0 )
      {
         x = aRect.x();
         
         y += lineHeight + spacing();
         
         nextX = x + ( *I_item )->sizeHint().width() + spacing();
         
         lineHeight = 0;
      }

      if( !anIsTestOnly )
      {
         ( *I_item )->setGeometry( QRect( QPoint( x, y ), ( *I_item )->sizeHint() ) );
      }
      
      x = nextX;
      
      lineHeight = qMax( lineHeight, ( *I_item )->sizeHint().height() );
   }
   
   return y + lineHeight - aRect.y();
}

void CECUDiagnosticGraphViewLayout::AddView( CAbstractMeterView* aView )
{
   TRACE_FUN( Routine, "CECUDiagnosticGraphViewLayout::AddView" );
   
   _viewContainer.insert( aView );

   QLayout::addWidget( aView );
}

CAbstractMeterView* CECUDiagnosticGraphViewLayout::Widget2View( QWidget* aWidget )const
{
   CAbstractMeterView* ret( 0 );

   TViewContainer::const_iterator I_view( _viewContainer.find( static_cast< CAbstractMeterView* >( aWidget ) ) );

   if( I_view != _viewContainer.end() )
   {
      ret = *I_view;
   }

   return ret;
}
