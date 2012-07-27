/*
 * FlowMeterPetrolFormula.h
 *
 *  Created on: 23.10.2009
 *      Author: mars
 */

#ifndef FLOWMETERPETROLFORMULA_H_
#define FLOWMETERPETROLFORMULA_H_

#include "AbstractFlowMeterFormula.h"
#include "ECUDataUnit.h"

namespace ECU
{
   namespace FlowMeter
   {
      class CFlowMeterPetrolFormula : public CAbstractFlowMeterFormula
      {
         HDataUnit _rpmUnit;
         HDataUnit _pulseUnit;
         
         double _injectorCalibration;

      public:
         CFlowMeterPetrolFormula( const CUnitsTable& aTable );
         virtual ~CFlowMeterPetrolFormula();
         
         virtual double flow()const;
      };
   }
}

#endif /* FLOWMETERPETROLFORMULA_H_ */
