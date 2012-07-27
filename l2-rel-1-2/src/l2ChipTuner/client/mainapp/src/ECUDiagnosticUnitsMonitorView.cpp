/*
 * ECUDiagnosticUnitsMonitorView.cpp
 *
 *  Created on: 10.10.2008
 *      Author: mars
 */

#include "ECUDiagnosticUnitsMonitorView.h"
#include "ECUDiagnosticUnitsMonitorModel.h"
#include "ECUDiagnosticUnitsMonitorModelNodes.h"
#include "ECUDiagnosticUnitsMonitorItemDelegate.h"
#include "ECUDeviceView.h"
#include "ECUDeviceUnitSelectionModel.h"
#include "MainWindow.h"
#include "MenuBarManager.h"

#include <AppDefs.h>
#include <AppContext.h>

#include <ECUUnitsTable.h>
#include <ECUDataUnit.h>

#include <l2Trace.h>
#include <l2ExceptionThrower.h>

#include <QPainter>
#include <QHeaderView>
#include <QMenu>
#include <QContextMenuEvent>
#include <QItemSelectionModel>
#include <QDialog>

CECUDiagnosticUnitsMonitorView::CECUDiagnosticUnitsMonitorView( CECUDeviceView* aParent ) : QTreeView( aParent )
{
   _model = 0;
   
   _ecuDeviceView = aParent;

   setRootIsDecorated( true );
   setSelectionBehavior( QAbstractItemView::SelectRows );
   setSelectionMode( QAbstractItemView::ExtendedSelection );
   setDragEnabled( true );
   setAcceptDrops( true );
   setDropIndicatorShown( true );
   setAlternatingRowColors( true );
   setHeaderHidden( true );
   setExpandsOnDoubleClick( true );

   QFont newFont( font() );
   newFont.setPointSize( 10 );

   setFont( newFont );

   _menuSessionId = _ecuDeviceView->mainWindow().menuBarManager().OpenSession();
}

CECUDiagnosticUnitsMonitorView::~CECUDiagnosticUnitsMonitorView()
{
   TRACE_FUN( Routine, "CECUDiagnosticUnitsMonitorView::~CECUDiagnosticUnitsMonitorView" );

   if( _model )
   {
      delete _model;
      _model = 0;

      setModel( _model );
   }
   
   _ecuDeviceView->mainWindow().menuBarManager().CloseSession( _menuSessionId );
}

void CECUDiagnosticUnitsMonitorView::UpdateHeader()
{
   if( isHeaderHidden() && model()->rowCount() )
   {
      l2max::CVariant nameSize;
      l2max::CVariant valueSize;
      l2max::CVariant descrSize;

      CAppContext::GetInstance()->MainRegistry().Read( V_DUMV_COLSIZE_NAME, nameSize );
      CAppContext::GetInstance()->MainRegistry().Read( V_DUMV_COLSIZE_VALUE, valueSize );
      CAppContext::GetInstance()->MainRegistry().Read( V_DUMV_COLSIZE_DESCR, descrSize );

      if( nameSize.toInt() != -1 && valueSize.toInt() != -1 && descrSize.toInt() != -1 )
      {
         header()->resizeSection( CECUDiagnosticUnitsMonitorModel::ciName, nameSize.toInt() );
         header()->resizeSection( CECUDiagnosticUnitsMonitorModel::ciValue, valueSize.toInt() );

         resizeColumnToContents( CECUDiagnosticUnitsMonitorModel::ciDescription );
      }

      setHeaderHidden( false );
   }
   else if( !isHeaderHidden() && !model()->rowCount() )
   {
      setHeaderHidden( true );

      SaveHeaderSizes();
   }
}

void CECUDiagnosticUnitsMonitorView::SaveHeaderSizes()const
{
   if( _model )
   {
      CAppContext::GetInstance()->MainRegistry().Write( V_DUMV_COLSIZE_NAME, header()->sectionSize( CECUDiagnosticUnitsMonitorModel::ciName ) );
      CAppContext::GetInstance()->MainRegistry().Write( V_DUMV_COLSIZE_VALUE, header()->sectionSize( CECUDiagnosticUnitsMonitorModel::ciValue ) );
      CAppContext::GetInstance()->MainRegistry().Write( V_DUMV_COLSIZE_DESCR, header()->sectionSize( CECUDiagnosticUnitsMonitorModel::ciDescription ) );
   }
}

void CECUDiagnosticUnitsMonitorView::UpdateDiagnosticDataTable( ECU::CUnitsTable& aTable )
{
   TRACE_FUN( Routine, "CECUDiagnosticUnitsMonitorView::UpdateDiagnosticDataTable" );

   if( !_model )
   {
      _model = new CECUDiagnosticUnitsMonitorModel( font(), aTable );
      _model->setSupportedDragActions( Qt::MoveAction );

      setModel( _model );

      connect( _model, SIGNAL( rowsInserted( const QModelIndex&, int, int ) ),
                       SLOT( OnRowsInserted( const QModelIndex&, int, int ) ) );

      connect( _model, SIGNAL( rowsRemoved( const QModelIndex&, int, int ) ),
                       SLOT( OnRowsRemoved( const QModelIndex&, int, int ) ) );

      connect( _model, SIGNAL( controlChangedEvent() ),
                       SLOT( OnControlChanged() ) );

      UpdateHeader();

      setItemDelegate( new CECUDiagnosticUnitsMonitorItemDelegate( aTable ) );

      expandAll();
   }

   if( state() == QAbstractItemView::NoState )
   {
      for( int i( 0 ), groupCount( _model->rowCount() ); i < groupCount; ++i )
      {
         QModelIndex theParent( _model->index( i, CECUDiagnosticUnitsMonitorModel::ciName ) );

         for( int j( 0 ), unitsCount( _model->rowCount( theParent ) ); j < unitsCount; ++j )
         {
            QModelIndex updateIndex( _model->index( j, CECUDiagnosticUnitsMonitorModel::ciValue, theParent ) );
            
            if( _model->Index2Key( updateIndex ).isValid() )
            {
               if( _model->dataUnit( _model->Index2Key( updateIndex ) )->IsChanged() )
               {
                  update( updateIndex );
               }
            }
         }
      }
   }
}

void CECUDiagnosticUnitsMonitorView::paintEvent( QPaintEvent* aPaintEvent )
{
   TRACE_FUN( Frequently, "CECUDiagnosticUnitsMonitorView::paintEvent" );

   if( _model && model()->rowCount() )
   {
      QTreeView::paintEvent( aPaintEvent );
   }
   else
   {
      QFont font( viewport()->font() );
      font.setPointSize( 10 );

      QPainter painter( viewport() );
      painter.setFont( font );
      painter.drawText( viewport()->rect(),
                        Qt::AlignCenter | Qt::TextWordWrap,
                        "To begin data monitoring drag awailable units into this view." );
   }
}

void CECUDiagnosticUnitsMonitorView::OnRowsInserted( const QModelIndex& aParent, int aStart, int anEnd )
{
   UpdateHeader();

   if( aParent.isValid() && !isExpanded( aParent ) )
   {
      expand( aParent );
   }
}

void CECUDiagnosticUnitsMonitorView::OnRowsRemoved( const QModelIndex& parent, int start, int end )
{
   UpdateHeader();
}

void CECUDiagnosticUnitsMonitorView::contextMenuEvent( QContextMenuEvent* anEvent )
{
   QTreeView::contextMenuEvent( anEvent );
   
   QMenu popupMenu( this );

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
   QMenu* measureUnitsMenu( _ecuDeviceView->mainWindow().menuBarManager().menu( CMenuBarManager::mMeterMeasureUnits )->menu() );

   if( !measureUnitsMenu->isEmpty() )
   {
      popupMenu.addMenu( measureUnitsMenu );
   }

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
   popupMenu.addAction( _ecuDeviceView->mainWindow().menuBarManager().menu( CMenuBarManager::mMeterGraph ) );

   if( !popupMenu.isEmpty() )
   {
      popupMenu.exec( anEvent->globalPos() );
   }
}

void CECUDiagnosticUnitsMonitorView::startDrag( Qt::DropActions supportedActions )
{
   if( supportedActions & Qt::MoveAction )
   {
      PrepareUnits2Removal();
   }

   QTreeView::startDrag( supportedActions );
}

void CECUDiagnosticUnitsMonitorView::showEvent( QShowEvent* anEvent )
{
   TRACE_FUN( Routine, "CECUDiagnosticUnitsMonitorView::showEvent" );

   QTreeView::showEvent( anEvent );   
   UpdateSelection();
}

void CECUDiagnosticUnitsMonitorView::hideEvent( QHideEvent* anEvent )
{
   TRACE_FUN( Routine, "CECUDiagnosticUnitsMonitorView::hideEvent" );

   SaveHeaderSizes();

   QTreeView::hideEvent( anEvent );
}

void CECUDiagnosticUnitsMonitorView::keyPressEvent( QKeyEvent* anEvent )
{
   TRACE_FUN( Routine, "CECUDiagnosticUnitsMonitorView::keyPressEvent" );

   if( anEvent->key() == Qt::Key_Space )
   {
      PrepareUnits2Removal();
   }

   QTreeView::keyPressEvent( anEvent );
}

void CECUDiagnosticUnitsMonitorView::PrepareUnits2Removal()
{
   TRACE_FUN( Routine, "CECUDiagnosticUnitsMonitorView::PrepareUnits2Removal" );

   QModelIndex index( currentIndex() );

   if( index.isValid() )
   {
      CECUDiagnosticUnitsMonitorModelUnitNode* currentNode(
         reinterpret_cast< CECUDiagnosticUnitsMonitorModelUnitNode* >( index.internalPointer() ) );

      if( currentNode->type() == CECUDiagnosticUnitsMonitorModelNode::tGroupUnit )
      {
         currentNode = static_cast< CECUDiagnosticUnitsMonitorModelUnitNode* >( currentNode->child( 0 ) );
      }

      QModelIndexList indexes( selectionModel()->selectedRows() );

      for( QModelIndexList::iterator i( indexes.begin() ); i != indexes.end(); ++i )
      {
         CECUDiagnosticUnitsMonitorModelUnitNode* node(
            reinterpret_cast< CECUDiagnosticUnitsMonitorModelUnitNode* >( i->internalPointer() ) );

         if( node->type() == CECUDiagnosticUnitsMonitorModelNode::tGroupUnit )
         {
            selectionModel()->select( *i, QItemSelectionModel::Deselect | QItemSelectionModel::Rows );
         }
      }
   }
}

void CECUDiagnosticUnitsMonitorView::OnControlChanged()
{
   controlChangedEvent();
}

void CECUDiagnosticUnitsMonitorView::UpdateSelection()
{
   if( selectionModel() )
   {
      //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      QModelIndexList list( selectionModel()->selection().indexes() );
      
      Diagnostic::TUnitKeys keys;
      
      for( QModelIndexList::const_iterator I_index( list.begin() ); I_index != list.end(); ++I_index )
      {
         if( I_index->isValid() )
         {
            keys.insert( _model->Index2Key( *I_index ) );
         }
      }

      _ecuDeviceView->selectionModel()->Select( keys );
      
      //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      QModelIndex current( selectionModel()->currentIndex() );
      
      Diagnostic::CUnitKey key;
      
      if( current.isValid() )
      {
         key = _model->Index2Key( current );
      }
      
      _ecuDeviceView->selectionModel()->SetCurrent( key );
      
      //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      _ecuDeviceView->UpdateMeterMenu( false );
   }
}

void CECUDiagnosticUnitsMonitorView::selectionChanged( const QItemSelection& aSelected, const QItemSelection& aDeselected )
{
   TRACE_FUN( Routine, "CECUDiagnosticUnitsMonitorView::selectionChanged" );

   QTreeView::selectionChanged( aSelected, aDeselected );
   
   UpdateSelection();
}

void CECUDiagnosticUnitsMonitorView::currentChanged( const QModelIndex& aCurrent, const QModelIndex& aPrevious )
{
   TRACE_FUN( Routine, "CECUDiagnosticUnitsMonitorView::currentChanged" );
   
   QTreeView::currentChanged( aCurrent, aPrevious );

   UpdateSelection();
}

bool CECUDiagnosticUnitsMonitorView::event( QEvent* anEvent )
{
   bool ret( false );
   
   if( anEvent->type() == QEvent::WindowActivate )
   {
      if( isActiveWindow() )
      {
         if( selectionModel() )
         {
            Diagnostic::TUnitKeys keys;
            
            QModelIndexList indexContainer( selectionModel()->selection().indexes() );
   
            for( QModelIndexList::const_iterator I_index( indexContainer.begin() ); I_index != indexContainer.end(); ++I_index )
            {
               if( I_index->isValid() )
               {
                  keys.insert( _model->Index2Key( *I_index ) );
               }
            }
   
            _ecuDeviceView->selectionModel()->Select( keys );
            _ecuDeviceView->selectionModel()->SetCurrent( _model->Index2Key( selectionModel()->currentIndex() ) );
            
            _ecuDeviceView->UpdateMeterMenu( false );
         }
      }
      
      ret = QTreeView::event( anEvent );
   }
   else
   {
      ret = QTreeView::event( anEvent );
   }
   
   return ret;
}

CECUDiagnosticUnitsMonitorModel* CECUDiagnosticUnitsMonitorView::model()const
{
   return _model;
}
