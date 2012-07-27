/*
 * MenuBarManager.h
 *
 *  Created on: 11.12.2008
 *      Author: mars
 */

#ifndef MENUBARMANAGER_H_
#define MENUBARMANAGER_H_

#include <QObject>

#include <map>
#include <set>

class QMenuBar;
class QAction;

class CMenuBarManager : public QObject
{
   Q_OBJECT;

   QMenuBar& _menuBar;
   
public:
   enum EMenu
   {
      mMain,
         mMainPreferences,
         mMainExit,

      mView,
         mViewUnits,
         mViewError,
         mViewFlowMeter,
         mViewGraph,
         mViewPacketSelection,
         mViewDataUnits,

      mDevice,
         mDeviceStartStopDiagnostic,
         mDevicePeriodicTrasmission,
         mDeviceSnapshot,

      mMeter,
         mMeterMeasureUnits,
         mMeterGraph,
            mMeterGraphWindow,
            mMeterGraphView,
            mMeterGraphClose,
            mMeterGraphCloseAll,

      mHelp,
         mHelpAbout,

      mNoMenu = -1
   };
   
   typedef unsigned int TSessionId;
   
   static const TSessionId _invalidSessionId;
   
   typedef std::set< TSessionId > TSessionsContainer;
   TSessionsContainer _sessionsContainer;

private:
   typedef std::pair< QAction*, QObject* > TMenuRelationship;
   typedef std::map< EMenu, TMenuRelationship > TMenuActionMap;
   TMenuActionMap _menuActionMap;
   
   typedef std::set< TSessionId > TSessionContainer;
   typedef std::map< EMenu, TSessionContainer > TMenuSessionContainer;
   mutable TMenuSessionContainer _menuSessionContainer;

   EMenu parentMenu( EMenu aMenu )const;

   template< class TParent >
      void Replace( EMenu aMenu, QAction* anAction, TParent& aParent );
   
   void ShowMenu( EMenu aMenu )const;
   void HideMenu( EMenu aMenu )const;

public:
   CMenuBarManager( QMenuBar& aMenuBar );
   virtual ~CMenuBarManager();

   QAction* menu( EMenu aMenu )const;

   void ShowMenu( EMenu aMenu, const TSessionId& anId )const;
   void HideMenu( EMenu aMenu, const TSessionId& anId )const;

   void Connect( EMenu aMenu, QObject* anObject, const char* aSlot, const char* aSignal = SIGNAL( triggered( bool ) ) );
   void Replace( EMenu aMenu, QAction* anAction );
   
   TSessionId OpenSession();
   void CloseSession( const TSessionId& anId );

signals:
   void menuShowHideEvent()const;
};

#endif /* MENUBARMANAGER_H_ */
