/*
 * AbstractFlowMeterFormula.h
 *
 *  Created on: 23.10.2009
 *      Author: mars
 */

#ifndef ABSTRACTFLOWMETERFORMULA_H_
#define ABSTRACTFLOWMETERFORMULA_H_

namespace ECU
{
   class CUnitsTable;
   
   namespace FlowMeter
   {
      class CAbstractFlowMeterFormula;
      
      namespace Factory
      {
         extern CAbstractFlowMeterFormula* CreateFlowMeterFormula( const CUnitsTable& aTable );
      }
      
      class CAbstractFlowMeterFormula
      {
      public:
         CAbstractFlowMeterFormula();
         virtual ~CAbstractFlowMeterFormula();
         
         virtual double flow()const = 0;
      };
   }
}

#endif /* ABSTRACTFLOWMETERFORMULA_H_ */
