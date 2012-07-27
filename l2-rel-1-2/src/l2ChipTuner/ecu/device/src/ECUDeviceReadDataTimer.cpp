/*
 * ECUDeviceReadDataTimer.cpp
 *
 *  Created on: 19.02.2009
 *      Author: mars
 */

#include "ECUDeviceReadDataTimer.h"
#include "ECUDevice.h"

#include <AppDefs.h>
#include <AppContext.h>

namespace ECU
{
   CDeviceReadDataTimer::CDeviceReadDataTimer( CDevice& aDevice ) : _device( aDevice ){}
   CDeviceReadDataTimer::~CDeviceReadDataTimer(){}
   
   void CDeviceReadDataTimer::StartTimer()
   {
      l2max::CVariant val;
      CAppContext::GetInstance()->MainRegistry().Read( V_ECU_DIAG_SESS_REQTIMEO, val );
      
      l2max::CTimer::StartTimer( val.toInt() );
   }
   
   void CDeviceReadDataTimer::IncInterval()
   {
      ChangeInterval( interval() + 1 );
   }
   
   void CDeviceReadDataTimer::DecInterval()
   {
      unsigned int theInterval( interval() - 1 );
      
      if( theInterval < 15 )
      {
         theInterval = 15;
      }
      
      ChangeInterval( theInterval );
   }
   
   void CDeviceReadDataTimer::timeout()
   {
      _device.ReadDiagnosticData();
   }
}
