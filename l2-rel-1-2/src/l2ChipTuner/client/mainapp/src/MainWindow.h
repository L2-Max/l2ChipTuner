/*
 * MainWindow.h
 *
 *  Created on: 03.10.2008
 *      Author: mars
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include "MenuBarManager.h"

#include <QMainWindow>

class CMenuBarManager;
class CToolBarManager;

class CECUDeviceView;
class QAction;
class QDockWidget;
class QLabel;

class CMainWindow : public QMainWindow
{
   Q_OBJECT;

   CMenuBarManager::TSessionId _menuSessionId;
   
   CMenuBarManager* _menuBarManager;
   CToolBarManager* _toolBarManager;

   CECUDeviceView* _ecuDeviceView;

   QLabel* _deviceStatus;
   QLabel* _communicationStatus;

   bool _closeRequired;

   virtual void closeEvent( QCloseEvent* anEvent );

   void RestorePosition();
   void SavePosition()const;

   void CreateMenu();
   void CreateECUDeviceView();

public:
   CMainWindow();
   virtual ~CMainWindow();

   QLabel& deviceStatus()const;
   QLabel& communicationStatus()const;

   CMenuBarManager& menuBarManager()const;

   void CloseIfRequired();

private slots:
   void OnPreferences();
   void OnAbout();
};

#endif /* MAINWINDOW_H_ */
