/*
 * MeterGraphWindow.h
 *
 *  Created on: 07.03.2009
 *      Author: L2-Max
 */

#ifndef METERGRAPHWINDOW_H_
#define METERGRAPHWINDOW_H_

#include <QMainWindow>

class CECUDeviceView;
class CMeter2DGraphView;

namespace ECU
{
   class CDataUnit;
}

namespace Diagnostic
{
   class CUnitKey;
}

class QSpinBox;
class QTimeEdit;

class CMeterGraphWindow : public QMainWindow
{
   Q_OBJECT;

   CECUDeviceView& _deviceView;
   CMeter2DGraphView* _view;

   QTimeEdit* _timeFrame;
   QSpinBox* _timeResolution;
   QSpinBox* _valueResolution;
   
   virtual void closeEvent( QCloseEvent* anEvent );
   virtual bool event( QEvent* anEvent );
   
public:
   CMeterGraphWindow( CECUDeviceView& aView );
   virtual ~CMeterGraphWindow();

   void SetView( CMeter2DGraphView* aView );
   
private slots:
   void OnTimeFrameChanged( const QTime& aTime );
   void OnTimeResolutionChanged( int aValue );
   void OnValueResolutionChanged( int aValue );
};

#endif /* METERGRAPHWINDOW_H_ */
