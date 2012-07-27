/*
 * ECUDiagnosticErrorsView.cpp
 *
 *  Created on: 07.12.2008
 *      Author: L2-Max
 */

#include "ECUDiagnosticErrorsView.h"

#include <ECUErrorsTable.h>

#include <l2Trace.h>

#include <QPainter>

CECUDiagnosticErrorsView::CECUDiagnosticErrorsView( QWidget* aParent ) : QListWidget( aParent )
{
}

CECUDiagnosticErrorsView::~CECUDiagnosticErrorsView()
{
}

void CECUDiagnosticErrorsView::UpdateDiagnosticErrorsTable( ECU::CErrorsTable& aTable )
{
   TRACE_FUN( Routine, "CECUDiagnosticErrorsView::UpdateDiagnosticErrorsTable" );

   Diagnostic::TErrorTypes errors( aTable.errors() );

   if( _errors != errors )
   {
      clear();

      _errors = errors;

      for( Diagnostic::TErrorTypes::iterator i( _errors.begin() ); i != _errors.end(); ++i )
      {
         addItem( Diagnostic::errorDescriptionMap()->find( *i )->second._description );
      }
   }
}
