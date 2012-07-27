/*
 * ECUDiagnosticUnitsMonitorView.h
 *
 *  Created on: 10.10.2008
 *      Author: mars
 */

#ifndef ECUDIAGNOSTICUNITSMONITORVIEW_H_
#define ECUDIAGNOSTICUNITSMONITORVIEW_H_

#include "MenuBarManager.h"

#include <QTreeView>

class CECUDiagnosticUnitsManagerView;
class CECUDiagnosticUnitsMonitorModel;
class CECUDeviceView;

namespace Diagnostic
{
   class CUnitKey;
}

namespace ECU
{
   class CUnitsTable;
   class CDataUnit;
   class CStateUnit;
}

class CECUDiagnosticUnitsMonitorView : public QTreeView
{
   Q_OBJECT;

   CMenuBarManager::TSessionId _menuSessionId;
   
   CECUDeviceView* _ecuDeviceView;

   CECUDiagnosticUnitsMonitorModel* _model;

   virtual void paintEvent( QPaintEvent* aPaintEvent );
   virtual void contextMenuEvent( QContextMenuEvent* anEvent );
   virtual void startDrag( Qt::DropActions supportedActions );
   virtual void showEvent( QShowEvent* anEvent );
   virtual void hideEvent( QHideEvent* anEvent );
   virtual void keyPressEvent( QKeyEvent* anEvent );

   virtual bool event( QEvent* anEvent );
   
   void UpdateHeader();
   void SaveHeaderSizes()const;

   void PrepareUnits2Removal();
   void UpdateSelection();
   
protected:
   virtual void selectionChanged( const QItemSelection& aSelected, const QItemSelection& aDeselected );
   virtual void currentChanged( const QModelIndex& aCurrent, const QModelIndex& aPrevious );

public:
   CECUDiagnosticUnitsMonitorView( CECUDeviceView* aParent );
   virtual ~CECUDiagnosticUnitsMonitorView();

   void UpdateDiagnosticDataTable( ECU::CUnitsTable& aTable );
   
   CECUDiagnosticUnitsMonitorModel* model()const;

private slots:
   void OnRowsInserted( const QModelIndex& aParent, int aStart, int anEnd );
   void OnRowsRemoved( const QModelIndex& aParent, int aStart, int anEnd );
   void OnControlChanged();

signals:
   void controlChangedEvent();
   void controlStartEditEvent();
   void controlEndEditEvent();
};

#endif /* ECUDIAGNOSTICUNITSMONITORVIEW_H_ */
