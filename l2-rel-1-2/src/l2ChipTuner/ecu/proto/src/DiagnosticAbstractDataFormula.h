/*
 * DiagnosticAbstractDataFormula.h
 *
 *  Created on: 20.11.2008
 *      Author: mars
 */

#ifndef DIAGNOSTICABSTRACTDATAFORMULA_H_
#define DIAGNOSTICABSTRACTDATAFORMULA_H_

#include <l2RefObject.h>
#include <l2RefHandle.hxx>

namespace l2max
{
   class CVariant;
}

namespace Diagnostic
{
   class CAbstractDataFormula;
   
   typedef l2max::CRefHandle< CAbstractDataFormula > HAbstractDataFormula;
   
   class CAbstractDataFormula : virtual public l2max::CRefObject
   {
   public:
      CAbstractDataFormula();
      virtual ~CAbstractDataFormula();

      virtual l2max::CVariant encode( const l2max::CVariant& aValue )const = 0;
      virtual l2max::CVariant decode( const l2max::CVariant& aValue )const = 0;
   };
}

#endif /* DIAGNOSTICABSTRACTDATAFORMULA_H_ */
