/*
 * ALDLLanos15SOHCMode4StreamImpl.cpp
 *
 *  Created on: 02.02.2009
 *      Author: mars
 */

#include "ALDLLanos15SOHCZXJNMode4StreamImpl.h"

#include <l2AbstractIODevice.h>
#include <l2Variant.h>
#include <l2MutexLocker.h>

#define DSEN_CHECKENGINE   0x01
#define DSEN_FAN           0x02
#define DSEN_AC            0x04
#define DSEN_CCP           0x10
#define DSEN_VGIS          0x20
#define DSEN_EGR           0x40
#define DSEN_EN            0x80

#define DSST_CHECKENGINE   0x01
#define DSST_FAN           0x02
#define DSST_AC            0x04
#define DSST_CCP           0x10
#define DSST_VGIS          0x20
#define DSST_EGR           0x40

#define NDEN_FUELMODE      0x01
#define NDEN_IACMODE       0x02
#define NDEN_BLMRESET      0x04
#define NDEN_IACRESET      0x08
#define NDEN_CLEARMALF     0x10

#define NDST_FUELMODE      0x01
#define NDST_IACMODE       0x02

#define PWEN_MAPDUTY       0x01

#define RAS_RPM            0x03
#define RAS_AIRFUEL        0x04
#define RAS_SPARKADV       0x08


namespace ALDL
{
   ////////////////////////////////////////////////////////////////////////////////////////
   struct SLanosZXJNMode4Stream
   {
      static const unsigned char _size = 10;

      SLanosZXJNMode4Stream()
      {
         ::memset( this, 0, sizeof( *this ) );
      }

      unsigned char _discreteOutputEnable;
      unsigned char _discreteOutputState;
      unsigned char _functionalControlEnable;
      unsigned char _functionalControlState;
      unsigned char _pwmOutputEnable;
      unsigned char _pwmDutyCycle;
      unsigned char _rpmAirSparc;
      unsigned char _rpmIac;
      unsigned char _airFuel;
      unsigned char _sparcAdvance;
   };

   ////////////////////////////////////////////////////////////////////////////////////////
   CLanos15SOHCZXJNMode4StreamImpl::CLanos15SOHCZXJNMode4StreamImpl( Diagnostic::CDataAdapter& anAdapter ) : CAbstractMode4( anAdapter ){}
   CLanos15SOHCZXJNMode4StreamImpl::~CLanos15SOHCZXJNMode4StreamImpl(){}

   void CLanos15SOHCZXJNMode4StreamImpl::Serialize( l2max::CAbstractIODevice& anIODevice )
   {
      SLanosZXJNMode4Stream mode4Stream;

      {
         l2max::CVariant v( value( Diagnostic::dtHiFan ) );

         if( v.isValid() )
         {
            mode4Stream._discreteOutputEnable |= DSEN_FAN;

            if( v.toBool() )
            {
               mode4Stream._discreteOutputState = DSST_FAN;
            }
         }
      }

      {
         l2max::CVariant v( value( Diagnostic::dtCharPurge ) );

         if( v.isValid() )
         {
            mode4Stream._discreteOutputEnable |= DSEN_CCP;

            if( v.toBool() )
            {
               mode4Stream._discreteOutputState = DSST_CCP;
            }
         }
      }

      {
         l2max::CVariant v( value( Diagnostic::dtClosedLoop ) );

         if( v.isValid() )
         {
            mode4Stream._functionalControlEnable |= NDEN_FUELMODE;

            if( v.toBool() )
            {
               mode4Stream._functionalControlState = NDST_FUELMODE;
            }
         }
      }

      {
         l2max::CVariant v( value( Diagnostic::dtIACResetReq ) );

         if( v.isValid() )
         {
            mode4Stream._functionalControlEnable |= NDEN_IACRESET;
         }
      }

      {
         l2max::CVariant v( value( Diagnostic::dtDesiredEngineSpeed ) );

         if( v.isValid() )
         {
            mode4Stream._rpmAirSparc |= RAS_RPM;
            mode4Stream._rpmIac = v.toInt();
         }
      }

      {
         l2max::CVariant v( value( Diagnostic::dtAirFuel ) );

         if( v.isValid() )
         {
            mode4Stream._rpmAirSparc |= RAS_AIRFUEL;
            mode4Stream._airFuel = v.toInt();
         }
      }

      {
         l2max::CVariant v( value( Diagnostic::dtSparcAdvance ) );

         if( v.isValid() )
         {
            mode4Stream._rpmAirSparc |= RAS_SPARKADV;
            mode4Stream._sparcAdvance = v.toInt();
         }
      }

      anIODevice.write( &mode4Stream, sizeof( mode4Stream ) );
   }

   bool CLanos15SOHCZXJNMode4StreamImpl::Deserialize( l2max::CAbstractIODevice& anIODevice )
   {
      SLanosZXJNMode4Stream mode4Stream;

      bool ret( anIODevice.read( &mode4Stream, sizeof( mode4Stream ) ) == sizeof( mode4Stream ) &&
                anIODevice.seek( SLanosZXJNMode4Stream::_size - sizeof( mode4Stream ), l2max::CAbstractIODevice::smFromCurrent ) );


      l2max::CMutexLocker locker( _controlMutex );

      if( mode4Stream._discreteOutputEnable & DSEN_FAN )
      {
         _rawDataMap[ Diagnostic::dtHiFan ] = static_cast< bool >( mode4Stream._discreteOutputState & DSST_FAN );
      }

      if( mode4Stream._discreteOutputEnable & DSEN_CCP )
      {
         _rawDataMap[ Diagnostic::dtCharPurge ] = static_cast< bool >( mode4Stream._discreteOutputState & DSST_CCP );
      }

      if( mode4Stream._functionalControlEnable & NDEN_FUELMODE )
      {
         _rawDataMap[ Diagnostic::dtClosedLoop ] = static_cast< bool >( mode4Stream._functionalControlState & NDST_FUELMODE );
      }

      if( mode4Stream._rpmAirSparc & RAS_RPM )
      {
         _rawDataMap[ Diagnostic::dtDesiredEngineSpeed ] = static_cast< int >( mode4Stream._rpmIac );
      }

      if( mode4Stream._rpmAirSparc & RAS_AIRFUEL )
      {
         _rawDataMap[ Diagnostic::dtAirFuel ] = static_cast< int >( mode4Stream._airFuel );
      }

      if( mode4Stream._rpmAirSparc & RAS_SPARKADV )
      {
         _rawDataMap[ Diagnostic::dtSparcAdvance ] = static_cast< int >( mode4Stream._sparcAdvance );
      }

      return ret;
   }

   HAbstractMode4 CLanos15SOHCZXJNMode4StreamImpl::clone()const
   {
      return *new CLanos15SOHCZXJNMode4StreamImpl( _dataAdapter );
   }
}
