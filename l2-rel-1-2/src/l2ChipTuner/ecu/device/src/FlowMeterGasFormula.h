/*
 * FlowMeterGasFormula.h
 *
 *  Created on: 23.10.2009
 *      Author: mars
 */

#ifndef FLOWMETERGASFORMULA_H_
#define FLOWMETERGASFORMULA_H_

#include "AbstractFlowMeterFormula.h"
#include "ECUDataUnit.h"

namespace ECU
{
   namespace FlowMeter
   {
      class CFlowMeterGasFormula : public CAbstractFlowMeterFormula
      {
         HDataUnit _rpmUnit;
         HDataUnit _mapUnit;
         HDataUnit _matUnit;
         HDataUnit _cutOffState;
         
         double _literWeight;
         double _lambda;
         
      public:
         CFlowMeterGasFormula( const CUnitsTable& aTable );
         virtual ~CFlowMeterGasFormula();
         
         virtual double flow()const;
      };
   }
}

#endif /* FLOWMETERGASFORMULA_H_ */
