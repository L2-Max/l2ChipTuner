/*
 * ECUDiagnosticUnitsMonitorItemDelegate.h
 *
 *  Created on: 06.02.2009
 *      Author: mars
 */

#ifndef ECUDIAGNOSTICUNITSMONITORITEMDELEGATE_H_
#define ECUDIAGNOSTICUNITSMONITORITEMDELEGATE_H_

#include <QItemDelegate>

namespace ECU
{
   class CUnitsTable;
}

class CECUDiagnosticUnitsMonitorItemDelegate : public QItemDelegate
{
   ECU::CUnitsTable& _dataUnitsTable;
   
public:
   CECUDiagnosticUnitsMonitorItemDelegate( ECU::CUnitsTable& aTable );
   virtual ~CECUDiagnosticUnitsMonitorItemDelegate();
   
   virtual QWidget* createEditor( QWidget* aParent, const QStyleOptionViewItem& anOption, const QModelIndex& anIndex )const;
};

#endif /* ECUDIAGNOSTICUNITSMONITORITEMDELEGATE_H_ */
