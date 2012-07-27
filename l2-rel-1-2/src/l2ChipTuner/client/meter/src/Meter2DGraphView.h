/*
 * Meter2DGraphView.h
 *
 *  Created on: 07.03.2009
 *      Author: L2-Max
 */

#ifndef METER2DGRAPHVIEW_H_
#define METER2DGRAPHVIEW_H_

#include "AbstractMeterView.h"
#include "IMeter2DGraphUpdateConsumer.h"

#include <MenuBarManager.h>

#include <QMenu>

struct IMeterGraphRenderer;

class CMeter2DGraphModel;
class CMeterGraphMeshRenderer;
class CMeterGraphValueRenderer;

namespace ECU
{
   class CDataUnit;
}

class CMeter2DGraphView : public CAbstractMeterView,
                          virtual private IMeter2DGraphUpdateConsumer
{
   Q_OBJECT;

   static const unsigned int _timerInterval;

   int _timerId;

   GLUnurbsObj* _nurbsRenderer;
   GLuint _meshList;

   CMenuBarManager::TSessionId _menuSessionId;
   
   QMenu _popupMenu;
   
   QRect _meshArea;
   
   QFont _normalFont;
   QFont _boldFont;
   QFont _bkgValueFont;
   QFont _bkgUnitFont;
   
   QRect _bkgValueRect;
   QRect _bkgUnitRect;
   
   enum ERasterType
   {
      rtValueAxisTitle,
      rtTimeAxisTitle,
      rtValueTitles,
      rtTimeTitles,
      rtUnitTitle,
      rtValue
   };
   
   struct SRaster;
   
   typedef std::map< ERasterType, SRaster > TRasterContainer;
   TRasterContainer _rasterContainer;

   virtual void timerEvent( QTimerEvent* anEvent );
   virtual void initializeGL();
   virtual void paintGL();
   virtual void paintEvent( QPaintEvent* anEvent );
   virtual void contextMenuEvent( QContextMenuEvent* anEvent );
   virtual void resizeGL( int aWidth, int aHeight );
   
   void DrawTitles();
   void DrawMesh();
   void DrawValueGraph();
   
   void UpdateValueAxisTitle();
   void UpdateTimeAxisTitle();
   void UpdateValueTitle();
   void UpdateTimeTitle();
   void UpdateUnitTitle();
   void UpdateValue();
   void UpdateMesh();
   
   void InitializeRasterRenderer( QPainter& aPainter, QImage& anImage );
   
   virtual void timeFrameChanged();
   virtual void valueFrameChanged();
   virtual void valueChanged();

public:
   CMeter2DGraphView( const Diagnostic::CUnitKey& aKey, CECUDeviceView& aView );
   virtual ~CMeter2DGraphView();
   
   CMeter2DGraphModel* model()const;
};

#endif /* METER2DGRAPHVIEW_H_ */
