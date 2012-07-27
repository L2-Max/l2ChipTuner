/*
 * ECUDiagnosticGraphViewLayout.h
 *
 *  Created on: 09.10.2009
 *      Author: mars
 */

#ifndef ECUDIAGNOSTICGRAPHVIEWLAYOUT_H_
#define ECUDIAGNOSTICGRAPHVIEWLAYOUT_H_

#include <QLayout>

#include <set>

class CAbstractMeterView;

class CECUDiagnosticGraphViewLayout : public QLayout
{
   int doLayout( const QRect& aRect, bool anIsTestOnly )const;

   typedef QList< QLayoutItem* > TItemContainer;
   TItemContainer _itemContainer;

   typedef std::set< CAbstractMeterView* > TViewContainer;
   TViewContainer _viewContainer;

public:
   CECUDiagnosticGraphViewLayout();
   virtual ~CECUDiagnosticGraphViewLayout();
   
   virtual void addItem( QLayoutItem* anItem );
   virtual void setGeometry( const QRect& aRect );
   
   virtual bool hasHeightForWidth()const;
   virtual int heightForWidth( int aWidth )const;
   virtual int count()const;
   
   virtual Qt::Orientations expandingDirections()const;
   virtual QLayoutItem* itemAt( int anIndex )const;
   virtual QSize minimumSize()const;
   
   virtual QSize sizeHint()const;
   virtual QLayoutItem* takeAt( int anIndex );

   void AddView( CAbstractMeterView* aView );
   CAbstractMeterView* Widget2View( QWidget* aWidget )const;
};

#endif /* ECUDIAGNOSTICGRAPHVIEWLAYOUT_H_ */
