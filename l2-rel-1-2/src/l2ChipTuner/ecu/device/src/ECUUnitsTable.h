/*
 * ECUUnitsTable.h
 *
 *  Created on: 25.10.2008
 *      Author: L2-Max
 */

#ifndef ECUUNITSTABLE_H_
#define ECUUNITSTABLE_H_

#include "ECUDataUnit.h"

#include <DiagnosticTable.h>
#include <DiagnosticAdapterHandles.h>

#include <l2Mutex.h>

#include <vector>
#include <string>

class CAbstractProto;

namespace ECU
{
   class CDataUnit;
   class CControlUnit;

   class CUnitsTable
   {
      typedef std::map< Diagnostic::CUnitKey, HDataUnit > TUnitContainer;
      TUnitContainer _unitsContainer;

      typedef std::map< Diagnostic::CUnitKey, CControlUnit* > TControlContainer;
      TControlContainer _controlsContainer;

      mutable l2max::CMutex _mutex;

      const std::string _namespace;

   public:
      CUnitsTable( const std::string& aNamespace );
      virtual ~CUnitsTable();

      /*
       * Data Units related members
       * */
      void BuildDataTable( Diagnostic::HDataAdapter anAdapter );
      bool hasData( const Diagnostic::CUnitKey& aKey )const;
      unsigned int dataCount()const;

      Diagnostic::CUnitKey Synonym2Key( const std::string& aSynonym );
      Diagnostic::TUnitKeys dataKeys()const;
      HDataUnit data( const Diagnostic::CUnitKey& aKey )const;

      /*
       * Control Units related members
       * */
      void BuildControlTable( Diagnostic::HControlAdapter anAdapter );
      bool hasControl( const Diagnostic::CUnitKey& aKey )const;
      unsigned int controlCount()const;

      Diagnostic::TUnitKeys controlKeys()const;
      CControlUnit& control( const Diagnostic::CUnitKey& aKey );

      const std::string& registryNamespace()const;
   };
}

#endif /* ECUUNITSTABLE_H_ */
