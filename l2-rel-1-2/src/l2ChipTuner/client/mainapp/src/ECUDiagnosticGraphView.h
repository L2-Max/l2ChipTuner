/*
 * ECUDiagnosticGraphView.h
 *
 *  Created on: 22.09.2009
 *      Author: mars
 */

#ifndef ECUDIAGNOSTICGRAPHVIEW_H_
#define ECUDIAGNOSTICGRAPHVIEW_H_

#include <QWidget>

class CAbstractMeterView;
class CECUDiagnosticGraphViewLayout;

class CECUDiagnosticGraphView : public QWidget
{
   virtual bool event( QEvent* anEvent);
   virtual void paintEvent( QPaintEvent* anEvent );

public:
   CECUDiagnosticGraphView( QWidget* aParent );
   virtual ~CECUDiagnosticGraphView();
   
   void InsertGraph( CAbstractMeterView* aView );
   
   CECUDiagnosticGraphViewLayout* layout();
};

#endif /* ECUDIAGNOSTICGRAPHVIEW_H_ */
