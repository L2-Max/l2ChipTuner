/*
 * ECUDeviceUnitSelectionModel.h
 *
 *  Created on: 28.09.2009
 *      Author: mars
 */

#ifndef ECUDEVICEUNITSELECTIONMODEL_H_
#define ECUDEVICEUNITSELECTIONMODEL_H_

#include <DiagnosticTable.h>

class CECUDeviceUnitSelectionModel
{
   Diagnostic::TUnitKeys _selectedKeys;
   Diagnostic::CUnitKey _currentKey;
   
public:
   CECUDeviceUnitSelectionModel();
   virtual ~CECUDeviceUnitSelectionModel();
   
   void Select( const Diagnostic::TUnitKeys& aKeys );
   void SetCurrent( const Diagnostic::CUnitKey& aKey );
   void Clear();
   
   const Diagnostic::TUnitKeys& selected()const;
   const Diagnostic::CUnitKey& current()const;
   
   bool hasSelection()const;
};

#endif /* ECUDEVICEUNITSELECTIONMODEL_H_ */
