/*
 * DataUnit.h
 *
 *  Created on: 12.10.2008
 *      Author: L2-Max
 */

#ifndef ECUDATAUNIT_H_
#define ECUDATAUNIT_H_

#include "ECUAbstractUnit.h"

#include <DiagnosticAdapterHandles.h>
#include <DiagnosticAbstractDataFormula.h>

#include <l2RefObject.h>
#include <l2RefHandle.hxx>
#include <l2Mutex.h>

#include <string>
#include <list>

namespace l2max
{
   class CVariant;
}

namespace ECU
{
   class CDataUnit;
   
   typedef l2max::CRefHandle< CDataUnit > HDataUnit;
   typedef std::list< HDataUnit > TDataUnitContainer;
   
   class CDataUnit : public CAbstractUnit,
                     virtual public l2max::CRefObject
   {
      mutable l2max::CMutex _mutex;
      
      const std::string _namespace;
      const std::string _synonym;
      const std::string _name;
      const std::string _description;
      
      Diagnostic::HDataAdapter _diagnosticDataAdapter;

      Diagnostic::EUnit _measureUnit;
      Diagnostic::EUnit _defaultUnit;

      CDataUnit( const CDataUnit& );
      CDataUnit& operator = ( const CDataUnit& );
      
      std::string registryKeyName()const;

   public:
      CDataUnit( Diagnostic::EDataType aType,
                 Diagnostic::HDataAdapter anAdapter, const std::string& aNamespace );

      virtual ~CDataUnit();

      virtual const std::string& synonym()const;
      virtual const std::string& name()const;
      virtual const std::string& description()const;
      std::string text()const;

      l2max::CVariant value()const;
      l2max::CVariant value( Diagnostic::EUnit anUnit )const;

      void SetMeasureUnit( Diagnostic::EUnit anUnit );
      Diagnostic::EUnit measureUnit()const;
      Diagnostic::TMeasureUnitsMap measureUnits()const;

      std::string measureUnitName()const;
      std::string measureUnitSymbol()const;
      
      bool IsChanged()const;
      
      Diagnostic::HAbstractDataFormula formula()const;
      Diagnostic::HAbstractDataFormula formula( Diagnostic::EUnit anUnit )const;
   };
}

#endif /* ECUDATAUNIT_H_ */
