/*
 * ECUDeviceView.h
 *
 *  Created on: 05.10.2008
 *      Author: L2-Max
 */

#ifndef ECUDEVICEVIEW_H_
#define ECUDEVICEVIEW_H_

#include "ECUDeviceNotifyHandler.h"
#include "MenuBarManager.h"

#include <DiagnosticTable.h>

#include <QTabWidget>

#include <map>

class CMainWindow;
class CECUDiagnosticUnitsMonitorView;
class CECUControlView;
class CECUDiagnosticErrorsView;
class CECUFlowMeterDeviceView;
class CFileProtocolPacketSelectionView;
class CMeterGraphWindow;
class CECUDiagnosticGraphView;
class CECUDeviceUnitSelectionModel;
class CMeterGraphManager;

namespace ECU
{
   class CDevice;
}

class QDockWidget;

class CECUDeviceView : public QTabWidget
{
   Q_OBJECT;

   enum EPageId
   {
      piUnitsMonitor,
      piErrors,
      piFlowMeter,
      piGraph,
      piInvalid = -1
   };

   CMenuBarManager::TSessionId _menuSessionId;
   
   CECUDeviceUnitSelectionModel* _selectionModel;
   
   bool _initialDataRead;
   bool _readyToClose;
   
   QIcon _noErrorIcon;
   QIcon _errorIcon;

   typedef std::map< EPageId, QWidget* > TPageId2Widget;
   TPageId2Widget _pageId2Widget;
   
   ECU::CDevice* _ecuDevice;

   CMainWindow* _mainWindow;
   CFileProtocolPacketSelectionView* _fileProtocolPacketSelectionView;

   CECUDeviceNotifyHandler _ecuNotifyHandler;
   
   CMeterGraphManager* _meterGraphManager;

   virtual void paintEvent( QPaintEvent* anEvent );
   virtual void closeEvent( QCloseEvent* anEvent );
   virtual void hideEvent( QHideEvent* anEvent );

   void CreateMenu();
   void CreatePacketSelectionView();
   
   void RemoveViews();
   void UpdateCommunicationStatus()const;
   void RemoveView( EPageId anId );
   void InitialDataRead();

   int PageId2Index( EPageId anId )const;
   EPageId Index2PageId( int anIndex )const;

   QWidget* PageId2View( EPageId anId )const;

   CECUDiagnosticUnitsMonitorView* ecuDiagnosticUnitsMonitorView()const;
   CECUDiagnosticErrorsView*       ecuDiagnosticErrorsView()const;
   CECUFlowMeterDeviceView*        ecuFlowMeterDeviceView()const;
   
public:
   CECUDeviceView( CMainWindow* aMainWindow );
   virtual ~CECUDeviceView();
   
   CMainWindow& mainWindow()const;
   ECU::CDevice& ecuDevice()const;

   void UpdateMeterMenu( bool anInGraph )const;
   
   CECUDeviceUnitSelectionModel* selectionModel()const;
   CMeterGraphManager* meterGraphManager()const;
   
   std::string registryNamespace()const;
   
   void OpenGraphView();
   
   CECUDiagnosticGraphView* ecuDiagnosticGraphView()const;

   bool IsReadyToClose()const;

private slots:
   void OnStartStopDiagnosticSession();
   void OnShowHideUnitsMonitorView();
   void OnShowHideErrorView();
   void OnShowHideFlowMeterView();
   void OnShowHideGraphView();
   void OnShowHidePacketSelectionView();

   void OnDiagnosticSessionStarted();
   void OnDiagnosticSessionStoped();
   void OnDataRead();
   void OnDataWriten( int aCode );
   void OnStopped();
   void OnStatUpdated();
   void OnControlChanged();

   void OnPeriodicTransmission( bool aState );
   void OnSnapshot();
   
   void OnOpenGraphInWindow();
   void OnOpenGraphInView();
   void OnCloseGraph();
   void OnCloseAllGraph();
   
   void OnDefaultUnitChanged( QAction* action );
};

#endif /* ECUDEVICEVIEW_H_ */
