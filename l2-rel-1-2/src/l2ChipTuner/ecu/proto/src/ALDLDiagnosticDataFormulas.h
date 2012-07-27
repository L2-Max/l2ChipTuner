/*
 * ALDLDiagnosticDataFormulas.h
 *
 *  Created on: 20.11.2008
 *      Author: mars
 */

#ifndef ALDLDIAGNOSTICDATAFORMULAS_H_
#define ALDLDIAGNOSTICDATAFORMULAS_H_

#include "DiagnosticAbstractDataFormula.h"

namespace ALDL
{
   class CBasicFormula : public Diagnostic::CAbstractDataFormula
   {
      double _scaling;
      unsigned char _precission;
      double _base;
      int _offset;

   public:
      CBasicFormula( double aScaling, unsigned char aPrecission = 0, int anOffset = 0, double aBase = 0 );
      virtual ~CBasicFormula();

      virtual l2max::CVariant encode( const l2max::CVariant& aValue )const;
      virtual l2max::CVariant decode( const l2max::CVariant& aValue )const;

      void SetScaling( double aScaling );
      void SetBase( double aBase );
   };
}

#endif /* ALDLDIAGNOSTICDATAFORMULAS_H_ */
