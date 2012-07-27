/*
 * ECUFlowMeterDevice.h
 *
 *  Created on: 10.12.2008
 *      Author: mars
 */

#ifndef ECUFLOWMETERDEVICE_H_
#define ECUFLOWMETERDEVICE_H_

#include <l2Thread.h>
#include <l2WaitCondition.h>
#include <l2Mutex.h>

#include <list>

namespace ECU
{
   class CUnitsTable;

   namespace FlowMeter
   {
      struct IDeviceNotify;
      class CAbstractFlowMeterFormula;

      class CDevice : public l2max::CThread
      {
         static const int _flowListSize;

         typedef std::list< double > TFlowAccList;

         TFlowAccList _flowPerSecondList;
         TFlowAccList _flowPerMeterList;

         double _cummulativeFlow;
         double _cummulativeDistance;
         double _cummulativeTime;

         IDeviceNotify* _deviceNotify;
         CAbstractFlowMeterFormula* _formula;

         CUnitsTable& _unitsTable;

         l2max::CWaitCondition _updateCondition;
         l2max::CWaitCondition _stopCondition;

         mutable l2max::CMutex _dataMutex;

         static double speed( int aSpeed );
         static double updateInterval();

         virtual unsigned int run();

         void AppendInstantFlow( TFlowAccList& aList, TFlowAccList::value_type aFlow );

      public:
         CDevice( CUnitsTable& aTable );
         virtual ~CDevice();

         bool Start();
         void Stop();
         void Reset();
         void update();

         bool isEnoughDataUnits()const;

         void InstallNotifyHandler( IDeviceNotify* aNotify );

         double flowPerSecondInstant()const;
         double flowPerMeterInstant()const;
         double flowPerMeterCummulative()const;
         double cummulativeDistance()const;
         double cummulativeFlow()const;
         double averageSpeed()const;

         unsigned int tps()const;
         unsigned int rpm()const;
      };
   }
}

#endif /* ECUFLOWMETERDEVICE_H_ */
