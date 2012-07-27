/*
 * ECUDiagnosticErrorsView.h
 *
 *  Created on: 07.12.2008
 *      Author: L2-Max
 */

#ifndef ECUDIAGNOSTICERRORSVIEW_H_
#define ECUDIAGNOSTICERRORSVIEW_H_

#include <DiagnosticTable.h>

#include <QListWidget>

namespace ECU
{
   class CErrorsTable;
}

class CECUDiagnosticErrorsView : public QListWidget
{
   Diagnostic::TErrorTypes _errors;

public:
   CECUDiagnosticErrorsView( QWidget* aParent );
   virtual ~CECUDiagnosticErrorsView();

   void UpdateDiagnosticErrorsTable( ECU::CErrorsTable& aTable );
};

#endif /* ECUDIAGNOSTICERRORSVIEW_H_ */
