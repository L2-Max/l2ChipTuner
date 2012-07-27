/*
 * DiagnosticDataAdapter.h
 *
 *  Created on: 12.10.2008
 *      Author: L2-Max
 */

#ifndef DIAGNOSTICDATAADAPTER_H_
#define DIAGNOSTICDATAADAPTER_H_

#include "DiagnosticAbstractAdapter.h"
#include "DiagnosticAbstractDataFormula.h"

#include <l2Mutex.h>
#include <l2RefObject.h>
#include <l2Variant.h>

#include <vector>

namespace l2max
{
   class CVariant;
}

namespace Diagnostic
{
   class CAbstractDataFormula;

   class CDataAdapter : public CAbstractAdapter
   {
      CDataAdapter( const CDataAdapter& );
      CDataAdapter& operator = ( const CDataAdapter& );
      
      typedef std::set< EDataType > TDataChangeContainer;
      TDataChangeContainer _dataChangeContainer;

   protected:
      typedef std::map< EDataType, int > TRawDataMap;
      
   private:
      TRawDataMap _rawDataMap;
      
   protected:
      mutable l2max::CMutex _dataMutex;

      typedef std::map< EDataType, TMeasureUnitsMap > TDataMeasureUnitsMap;
      TDataMeasureUnitsMap _dataMeasureUnitsMap;

      typedef std::map< EUnit, HAbstractDataFormula > TFormulaUnitMap;
      typedef std::map< EDataType, TFormulaUnitMap > TFormulaMap;
      TFormulaMap _formulaMap;
      
      void DataChanged( const TRawDataMap& aData );

   public:
      CDataAdapter();
      virtual ~CDataAdapter();

      virtual TUnitKeys keys()const;

      l2max::CVariant value( EDataType aType, EUnit anUnit )const;

      TMeasureUnitsMap measureUnits( EDataType aType )const;

      HAbstractDataFormula formula( EDataType aType, EUnit anUnit )const;
      
      bool IsChanged( EDataType aType )const;
   };

}

#endif /* DIAGNOSTICDATAADAPTER_H_ */
