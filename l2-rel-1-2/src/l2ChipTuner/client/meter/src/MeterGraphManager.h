/*
 * MeterGraphManager.h
 *
 *  Created on: 01.10.2009
 *      Author: mars
 */

#ifndef METERGRAPHMANAGER_H_
#define METERGRAPHMANAGER_H_

#include <DiagnosticTable.h>

#include <set>
#include <map>

class CECUDeviceView;

class CMeterGraphWindow;
class CMeter2DGraphView;

class CMeterGraphManager
{
   CECUDeviceView* _deviceView;
   
   typedef std::set< CMeter2DGraphView* > TGraphViewContainer;
   TGraphViewContainer _graphViewContainer;
   
   typedef std::map< Diagnostic::CUnitKey, CMeterGraphWindow* > TGraphWindowContainer;
   TGraphWindowContainer _graphWindowContainer;
   
   Diagnostic::TUnitKeys _viewKeyContainer;
   
   CMeter2DGraphView* view( const Diagnostic::CUnitKey& aKey )const;

public:
   CMeterGraphManager( CECUDeviceView* aView );
   virtual ~CMeterGraphManager();
   
   void OpenGraphsInView( const Diagnostic::TUnitKeys& aKeys );
   void OpenGraphsInWindow( const Diagnostic::TUnitKeys& aKeys );
   
   void Restore();
   void Close( const Diagnostic::CUnitKey& aKey );
   void Close( const Diagnostic::TUnitKeys& aKeys );
   void CloseAll();
   void Update();
   
   void CloseView();
   void RestoreView();
   
   bool IsView( const Diagnostic::CUnitKey& aKey )const;
   bool IsWindow( const Diagnostic::CUnitKey& aKey )const;
   bool hasOpenedGraph()const;
};

#endif /* METERGRAPHMANAGER_H_ */
