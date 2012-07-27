/*
 * ToolBarManager.h
 *
 *  Created on: 25.02.2009
 *      Author: L2-Max
 */

#ifndef TOOLBARMANAGER_H_
#define TOOLBARMANAGER_H_

#include <QObject>

#include <map>

class CMainWindow;

class QToolBar;

class CToolBarManager : public QObject
{
   Q_OBJECT;

   CMainWindow& _mainWindow;

public:
   enum EToolBar
   {
      tbMain,
      tbView,
      tbDevice,
   };

private:
   typedef std::map< EToolBar, QToolBar* > TToolBarMap;
   TToolBarMap _toolBarMap;

public:
   CToolBarManager( CMainWindow* aParent );
   virtual ~CToolBarManager();

private slots:
   void OnMenuShowHide();
};

#endif /* TOOLBARMANAGER_H_ */
