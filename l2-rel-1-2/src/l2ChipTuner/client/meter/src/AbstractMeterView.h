/*
 * AbstractMeterView.h
 *
 *  Created on: 05.03.2009
 *      Author: mars
 */

#ifndef ABSTRACTMETERVIEW_H_
#define ABSTRACTMETERVIEW_H_

#include "IMeterUpdateConsumer.h"

#include <DiagnosticTable.h>

#include <QGLWidget>

class CAbstractMeterModel;
class CECUDeviceView;

class CAbstractMeterView : public QGLWidget,
                           virtual private IMeterUpdateConsumer
{
   Q_OBJECT;
   
   CAbstractMeterModel* _model;
   Diagnostic::CUnitKey _key;
   const CECUDeviceView& _deviceView;
   
   virtual void modelUpdated();

protected:
   virtual void initializeGL();
   virtual void paintGL();
   virtual void resizeGL( int aWidth, int aHeight );
   virtual void paintEvent( QPaintEvent* anEvent );
	
   void SetModel( CAbstractMeterModel* aModel );

   void SaveGLState();
   void RestoreGLState();

protected:
   const CECUDeviceView& deviceView()const;

public:
   CAbstractMeterView( const Diagnostic::CUnitKey& aKey, CECUDeviceView& aView );
   virtual ~CAbstractMeterView();
   
   void Activate();

   CAbstractMeterModel* model()const;

   const Diagnostic::CUnitKey& key()const;
};

#endif /* ABSTRACTMETERVIEW_H_ */
