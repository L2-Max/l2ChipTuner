/*
 * ECUFlowMeterDevice.cpp
 *
 *  Created on: 10.12.2008
 *      Author: mars
 */

#include "ECUFlowMeterDevice.h"
#include "ECUUnitsTable.h"
#include "ECUDataUnit.h"
#include "ECUFlowMeterDeviceNotify.h"
#include "AbstractFlowMeterFormula.h"

#include <AppDefs.h>
#include <AppContext.h>

#include <l2Trace.h>
#include <l2MutexLocker.h>
#include <l2Timer.h>
#include <l2ExceptionThrower.h>

#include <numeric>
#include <math.h>

namespace ECU
{
   namespace FlowMeter
   {
      const int CDevice::_flowListSize( 0x10 );

      double CDevice::updateInterval()
      {
         double ret( .0 );

         static unsigned int lastClock( 0 );

         unsigned int currClock( l2max::CTimer::clock() );

         ret = ( currClock - lastClock ) / 1000.;

         lastClock = currClock;

         return ret;
      }

      CDevice::CDevice( CUnitsTable& aTable ) :
         _unitsTable( aTable )
      {
         _deviceNotify = 0;
         
         _formula = Factory::CreateFlowMeterFormula( _unitsTable );

         _cummulativeFlow = .0;
         _cummulativeDistance = .0;

         l2max::CVariant val;

         if( CAppContext::GetInstance()->MainRegistry().Read( V_ECU_FLOWMETER_LASTDIST, val ) )
         {
            _cummulativeDistance = val.toDouble();
         }

         if( CAppContext::GetInstance()->MainRegistry().Read( V_ECU_FLOWMETER_LASTFLOW, val ) )
         {
            _cummulativeFlow = val.toDouble();
         }

         if( CAppContext::GetInstance()->MainRegistry().Read( V_ECU_FLOWMETER_LASTTIME, val ) )
         {
            _cummulativeTime = val.toInt();
         }
      }

      CDevice::~CDevice()
      {
         CAppContext::GetInstance()->MainRegistry().Write( V_ECU_FLOWMETER_LASTFLOW, _cummulativeFlow );
         CAppContext::GetInstance()->MainRegistry().Write( V_ECU_FLOWMETER_LASTDIST, _cummulativeDistance );
         CAppContext::GetInstance()->MainRegistry().Write( V_ECU_FLOWMETER_LASTTIME, ( int )_cummulativeTime );
         
         delete _formula;
      }

      unsigned int CDevice::run()
      {
         TRACE_FUN( Routine, "ECU::FlowMeter::CDevice::run" );

         unsigned int ret( 0 );

         CDevice::updateInterval();

         while( _updateCondition.waitForEvent() && !_stopCondition.waitForEvent( 0 ) )
         {
            TRACE_FUN( Routine, "CDevice::run *** Process flow" );

            double interval( CDevice::updateInterval() );

            double flowPerSecond( _formula->flow() );

            AppendInstantFlow( _flowPerSecondList, flowPerSecond );

            double metersPerSecond( speed( _unitsTable.data( Diagnostic::dtSpeed )->value( Diagnostic::uKilometerPerHour ).toInt() ) );

            double flowPerMeter( .0 );

            if( metersPerSecond > 0. )
            {
               flowPerMeter = flowPerSecond / metersPerSecond;
            }
            else
            {
               flowPerMeter = -.1;
            }

            AppendInstantFlow( _flowPerMeterList, flowPerMeter );

            _cummulativeFlow += flowPerSecond * interval;
            _cummulativeDistance += metersPerSecond * interval;
            _cummulativeTime += interval;

            if( _deviceNotify )
            {
               _deviceNotify->updated();
            }
         }

         return ret;
      }

      void CDevice::update()
      {
         TRACE_FUN( Routine, "ECU::FlowMeter::CDevice::update" );

         if( isRun() )
         {
            _updateCondition.wake();
         }
      }

      bool CDevice::Start()
      {
         TRACE_FUN( Routine, "ECU::FlowMeter::CDevice::Start" );

         l2max::CMutexLocker locker( _dataMutex );

         bool ret( false );

         if( !isRun() && isEnoughDataUnits() )
         {
            l2max::CThread::Start();
         }

         return ret;
      }

      void CDevice::Stop()
      {
         TRACE_FUN( Routine, "ECU::FlowMeter::CDevice::Stop" );

         l2max::CMutexLocker locker( _dataMutex );

         if( isRun() )
         {
            _stopCondition.wake();

            _deviceNotify = 0;

            update();

            do
            {
               _dataMutex.ReLock();
            }
            while( !Wait( 0 ) );
         }
      }

      void CDevice::Reset()
      {
         TRACE_FUN( Routine, "ECU::FlowMeter::CDevice::Reset" );

         l2max::CMutexLocker locker( _dataMutex );

         _cummulativeFlow = .0;
         _cummulativeDistance = .0;
         _cummulativeTime = 0;
      }

      void CDevice::InstallNotifyHandler( IDeviceNotify* aNotify )
      {
         TRACE_FUN( Routine, "ECU::FlowMeter::CDevice::InstallNotifyHandler" );

         l2max::CMutexLocker locker( _dataMutex );

         _deviceNotify = aNotify;
      }

      void CDevice::AppendInstantFlow( TFlowAccList& aList, TFlowAccList::value_type aFlow )
      {
         l2max::CMutexLocker locker( _dataMutex );

         aList.push_back( aFlow );

         if( aList.size() > CDevice::_flowListSize )
         {
            aList.pop_front();
         }
      }

      double CDevice::speed( int aSpeed )
      {
         double ret( .0 );

         if( aSpeed > 0 )
         {
            ret = aSpeed * 1000. / 3600.;
         }

         return ret;
      }

      double CDevice::flowPerSecondInstant()const
      {
         l2max::CMutexLocker locker( _dataMutex );

         return std::accumulate( _flowPerSecondList.begin(), _flowPerSecondList.end(), 0. ) / _flowPerSecondList.size();
      }

      double CDevice::flowPerMeterInstant()const
      {
         l2max::CMutexLocker locker( _dataMutex );

         double ret( .0 );
         unsigned int count( 0 );

         for( TFlowAccList::const_iterator i( _flowPerMeterList.begin() ); i != _flowPerMeterList.end(); ++i )
         {
            if( *i >= 0 )
            {
               ret += *i;

               ++count;
            }
         }

         if( count )
         {
            ret /= count;
         }
         else
         {
            ret = -1.;
         }

         return ret;
      }

      double CDevice::flowPerMeterCummulative()const
      {
         l2max::CMutexLocker locker( _dataMutex );

         double ret( -.1 );

         if( _cummulativeDistance > .0 )
         {
            ret = _cummulativeFlow / _cummulativeDistance;
         }

         return ret;
      }

      double CDevice::cummulativeDistance()const
      {
         l2max::CMutexLocker locker( _dataMutex );

         return _cummulativeDistance;
      }

      double CDevice::cummulativeFlow()const
      {
         l2max::CMutexLocker locker( _dataMutex );

         return _cummulativeFlow;
      }

      double CDevice::averageSpeed()const
      {
         l2max::CMutexLocker locker( _dataMutex );

         return _cummulativeDistance / _cummulativeTime * 3600. / 1000.;
      }

      unsigned int CDevice::tps()const
      {
         return _unitsTable.data( Diagnostic::dtTps )->value( Diagnostic::uPercent ).toInt();
      }

      unsigned int CDevice::rpm()const
      {
         return _unitsTable.data( Diagnostic::dtPresentEngineSpeed )->value( Diagnostic::uRevolutionsPerMinute ).toInt();
      }

      bool CDevice::isEnoughDataUnits()const
      {
         return _unitsTable.hasData( Diagnostic::dtPresentEngineSpeed ) &&
                _unitsTable.hasData( Diagnostic::dtMat ) &&
                _unitsTable.hasData( Diagnostic::dtMap );
      }
   }
}
