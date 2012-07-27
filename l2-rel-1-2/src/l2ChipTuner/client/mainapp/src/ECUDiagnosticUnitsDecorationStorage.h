/*
 * ECUDiagnosticUnitsDecorationStorage.h
 *
 *  Created on: 17.02.2009
 *      Author: mars
 */

#ifndef ECUDIAGNOSTICUNITSDECORATIONSTORAGE_H_
#define ECUDIAGNOSTICUNITSDECORATIONSTORAGE_H_

#include <DiagnosticTable.h>

#include <l2Singleton.hxx>

#include <QIcon>

class CECUDiagnosticUnitsDecorationStorage : public l2max::CSingleton< CECUDiagnosticUnitsDecorationStorage >
{
   friend class l2max::CSingleton< CECUDiagnosticUnitsDecorationStorage >;
   
   QIcon _dataUnitReadOnlyIcon;
   QIcon _dataUnitReadWriteIcon;
   
   CECUDiagnosticUnitsDecorationStorage( const CECUDiagnosticUnitsDecorationStorage& );
   CECUDiagnosticUnitsDecorationStorage& operator = ( const CECUDiagnosticUnitsDecorationStorage& );
   
   CECUDiagnosticUnitsDecorationStorage();
   virtual ~CECUDiagnosticUnitsDecorationStorage();

public:
   enum EUnitType
   {
      utReadOnly,
      utReadWrite
   };
   
   QIcon unitIcon( EUnitType aType )const;
};

#endif /* ECUDIAGNOSTICUNITSDECORATIONSTORAGE_H_ */
