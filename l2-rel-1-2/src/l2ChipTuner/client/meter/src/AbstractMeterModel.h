/*
 * AbstractMeterModel.h
 *
 *  Created on: 05.03.2009
 *      Author: mars
 */

#ifndef ABSTRACTMETERMODEL_H_
#define ABSTRACTMETERMODEL_H_

#include <ECUDataUnit.h>

#include <l2Variant.h>

#include <string>
#include <list>

namespace Diagnostic
{
   class CUnitKey;
}

struct IMeterUpdateConsumer;

class CECUDeviceView;
class CAbstractMeterView;

class CAbstractMeterModel
{
   std::string _registryKey;

   const CECUDeviceView& _deviceView;

private:
   ECU::HDataUnit _dataUnit;
   IMeterUpdateConsumer& _updateConsumer;

protected:
   IMeterUpdateConsumer& updateConsumer()const;
   
   ECU::HDataUnit dataUnit()const;

public:
   CAbstractMeterModel( const Diagnostic::CUnitKey& aKey,
                        const CECUDeviceView& aDeviceView,
                        IMeterUpdateConsumer& aUpdateConsumer );

   virtual ~CAbstractMeterModel();

   virtual void update();
   
   std::string unit()const;
   std::string text()const;
   
   const std::string& registryKey()const;
};

#endif /* ABSTRACTMETERMODEL_H_ */
