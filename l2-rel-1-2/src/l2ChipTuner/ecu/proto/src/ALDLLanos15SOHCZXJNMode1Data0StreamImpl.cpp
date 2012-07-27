/*
 * LanosZXJNMode1Data0StreamImpl.cpp
 *
 *  Created on: 12.10.2008
 *      Author: L2-Max
 */

#include "ALDLLanos15SOHCZXJNMode1Data0StreamImpl.h"

#include <l2MutexLocker.h>
#include <l2AbstractIODevice.h>
#include <l2Variant.h>

#define SB1_COOLSW         0x04
#define SB1_HIGHFAN        0x08
#define SB1_LOWFAN         0x20

#define SB2_FUELCUTOFF     0x10
#define SB2_FUELPUMP       0x04

#define SB3_FLDSRVC        0x04
#define SB3_CHARPURGE      0x80
#define SB3_VGIS           0x40

#define SB4_TPSCLOSED      0x01
#define SB4_DECELCUTOFF    0x40

#define SB5_BLOCKLEARN     0x08
#define SB5_CLOSEDLOOP     0x02
#define SB5_REACHLEAN      0x40

#define SB6_CATCONVOVER    0x80

#define SB7_IDLERUN        0x04
#define SB7_INTRESET       0x40
#define SB7_RPMLOW         0x08

#define SB8_IACRESET       0x02
#define SB8_IACRESETREQ    0x01

#define SB9_O2READY        0x01

#define SB10_IACRUN        0x08

#define SB11_FANREQ        0x04
#define SB11_OCTANE83      0x00
#define SB11_OCTANE87      0x20
#define SB11_OCTANE91      0xa0
#define SB11_OCTANE95      0x80

#define MALF1_EST          0x01 // Electronic Spark Timing. Firing point control
#define MALF1_HIMAP        0x02 // High manifold pressure
#define MALF1_EGR          0x04 // Exhaust Gas Recirculation Circuit
#define MALF1_SPEED        0x08 // No signal from speed sensor
#define MALF1_HIMAT        0x10 // High manifold air temperature
#define MALF1_HITPS        0x20 // High signal from Trotle Position Sensor
#define MALF1_LOCOLTEMP    0x40 // Low collant temperature
#define MALF1_O2           0x80 // Oxygen sensor circuit

#define MALF2_ESTASHORT    0x01 // Electronic Spark Timing A shorted at BAT+
#define MALF2_ESTASHORTGND 0x02 // Electronic Spark Timing A shorted at ground
#define MALF2_IMMOB        0x04 // Immobilizer no connection
#define MALF2_OCTANE       0x08 // Octane corrector
#define MALF2_KNOCK        0x10 // Knock sensor
#define MALF2_ECU1         0x20 // ECU Error
#define MALF2_O2REACH      0x40 // O2 Sensor reach
#define MALF2_O2LEAN       0x80 // O2 Sensor lean

#define MALF3_LOTPS        0x01 // Low signal from Trotle Position Sensor
#define MALF3_HICOLTEMP    0x02 // High collant temperature
#define MALF3_IAC          0x04 // Idle Air Control
#define MALF3_KPS          0x08 // Crank position sensor
#define MALF3_ECU2         0x10 // ECU Error
#define MALF3_INJECTOR     0x20 // Injector
#define MALF3_ESTBSHORT    0x40 // Electronic Spark Timing A shorted at BAT+
#define MALF3_ESTBSHORTGND 0x80 // Electronic Spark Timing A shorted at ground

#define MALF4_LOMAP        0x02 // Low manifold pressure
#define MALF4_LOMAT        0x10 // Low manifold air temperature

namespace ALDL
{
   ////////////////////////////////////////////////////////////////////////////////////////
   struct SLanosZXJNMode1Data0Stream
   {
      static const unsigned char _size = 60;

      SLanosZXJNMode1Data0Stream()
      {
         ::memset( this, 0, sizeof( *this ) );
      }

      unsigned char _promid[ 2 ];

      unsigned char _malffw1;
      unsigned char _malffw2;
      unsigned char _malffw3;
      unsigned char _malffw4;

      unsigned char _cooldeg;
      unsigned char _matdeg;
      unsigned char _admap;

      unsigned char _not_used1[ 2 ];

      unsigned char _adthrot;
      unsigned char _ntrpmx;
      unsigned char _ado2a;
      unsigned char _filtmph;

      unsigned char _not_used2[ 4 ];

      unsigned char _adbat;

      unsigned char _not_used3;

      unsigned char _sap[ 2 ];

      unsigned char _isdsmp;
      unsigned char _isspmp;
      unsigned char _desspd;

      unsigned char _not_used4[ 10 ];

      unsigned char _dispbpw[ 2 ];

      unsigned char _airfuel;
      unsigned char _blm;
      unsigned char _blmcell;
      unsigned char _int;

      unsigned char _not_used5[ 5 ];

      unsigned char _stateByte1;
      unsigned char _stateByte2;

      unsigned char _not_used6[ 1 ];

      unsigned char _stateByte3;
      unsigned char _stateByte4;
      unsigned char _stateByte5;
      unsigned char _stateByte6;

      unsigned char _not_used7[ 1 ];

      unsigned char _stateByte7;
      unsigned char _stateByte8;
      unsigned char _stateByte9;
      unsigned char _stateByte10;
      unsigned char _stateByte11;
   };

   ////////////////////////////////////////////////////////////////////////////////////////
   CLanos15SOHCZXJNMode1Data0StreamImpl::CLanos15SOHCZXJNMode1Data0StreamImpl(){}
   CLanos15SOHCZXJNMode1Data0StreamImpl::~CLanos15SOHCZXJNMode1Data0StreamImpl(){}

   bool CLanos15SOHCZXJNMode1Data0StreamImpl::Deserialize( l2max::CAbstractIODevice& anIODevice )
   {
      SLanosZXJNMode1Data0Stream mode1Data0Stream;

      bool ret( anIODevice.read( &mode1Data0Stream, sizeof( mode1Data0Stream ) ) == sizeof( mode1Data0Stream ) &&
                anIODevice.seek( SLanosZXJNMode1Data0Stream::_size - sizeof( mode1Data0Stream ), l2max::CAbstractIODevice::smFromCurrent ) );

      /*
       * Data
       * */
      TRawDataMap rawDataMap;
      
      rawDataMap[ Diagnostic::dtCoolantTemp ]        = mode1Data0Stream._cooldeg;
      rawDataMap[ Diagnostic::dtMap ]                = mode1Data0Stream._admap;
      rawDataMap[ Diagnostic::dtTps ]                = mode1Data0Stream._adthrot;
      rawDataMap[ Diagnostic::dtPresentEngineSpeed ] = mode1Data0Stream._ntrpmx;
      rawDataMap[ Diagnostic::dtO2 ]                 = mode1Data0Stream._ado2a;
      rawDataMap[ Diagnostic::dtSpeed ]              = mode1Data0Stream._filtmph;
      rawDataMap[ Diagnostic::dtMat ]                = mode1Data0Stream._matdeg;
      rawDataMap[ Diagnostic::dtBat ]                = mode1Data0Stream._adbat;

      {
         short val( 0 );

         reinterpret_cast< unsigned char* >( &val )[ 0 ] = mode1Data0Stream._sap[ 1 ];
         reinterpret_cast< unsigned char* >( &val )[ 1 ] = mode1Data0Stream._sap[ 0 ];

         rawDataMap[ Diagnostic::dtSparcAdvance ] = val;
      }

      rawDataMap[ Diagnostic::dtIACDesiredPosition ] = mode1Data0Stream._isdsmp;
      rawDataMap[ Diagnostic::dtIACPresentPosition ] = mode1Data0Stream._isspmp;
      rawDataMap[ Diagnostic::dtDesiredEngineSpeed ] = mode1Data0Stream._desspd;

      {
         unsigned short val( 0 );

         reinterpret_cast< unsigned char* >( &val )[ 0 ] = mode1Data0Stream._dispbpw[ 1 ];
         reinterpret_cast< unsigned char* >( &val )[ 1 ] = mode1Data0Stream._dispbpw[ 0 ];

         rawDataMap[ Diagnostic::dtBasePulseWidth ] = val;
      }

      rawDataMap[ Diagnostic::dtAirFuel ]   = mode1Data0Stream._airfuel;
      rawDataMap[ Diagnostic::dtBlm ]       = mode1Data0Stream._blm;
      rawDataMap[ Diagnostic::dtBlmCell ]   = mode1Data0Stream._blmcell;
      rawDataMap[ Diagnostic::dtInt ]       = mode1Data0Stream._int;

      switch( mode1Data0Stream._stateByte11 & 0xe0 )
      {
      case SB11_OCTANE83: rawDataMap[ Diagnostic::dtOctane ] = 83; break;
      case SB11_OCTANE87: rawDataMap[ Diagnostic::dtOctane ] = 87; break;
      case SB11_OCTANE91: rawDataMap[ Diagnostic::dtOctane ] = 91; break;
      case SB11_OCTANE95: rawDataMap[ Diagnostic::dtOctane ] = 95; break;
      }

     rawDataMap[ Diagnostic::dtCoolantSw ]      = mode1Data0Stream._stateByte1 & SB1_COOLSW;
     rawDataMap[ Diagnostic::dtHiFan ]          = mode1Data0Stream._stateByte1 & SB1_HIGHFAN;
     rawDataMap[ Diagnostic::dtLoFan ]          = mode1Data0Stream._stateByte1 & SB1_LOWFAN;

     rawDataMap[ Diagnostic::dtFuelCutOff ]     = mode1Data0Stream._stateByte2 & SB2_FUELCUTOFF;
     rawDataMap[ Diagnostic::dtFuelPump ]       = mode1Data0Stream._stateByte2 & SB2_FUELPUMP;

     rawDataMap[ Diagnostic::dtFieldService ]   = mode1Data0Stream._stateByte3 & SB3_FLDSRVC;
     rawDataMap[ Diagnostic::dtCharPurge ]      = mode1Data0Stream._stateByte3 & SB3_CHARPURGE;
     rawDataMap[ Diagnostic::dtVGIS ]           = mode1Data0Stream._stateByte3 & SB3_VGIS;

     rawDataMap[ Diagnostic::dtTPSClosed ]      = mode1Data0Stream._stateByte4 & SB4_TPSCLOSED;
     rawDataMap[ Diagnostic::dtDecelCutOff ]    = mode1Data0Stream._stateByte4 & SB4_DECELCUTOFF;

     rawDataMap[ Diagnostic::dtBlockLearn ]     = mode1Data0Stream._stateByte5 & SB5_BLOCKLEARN;
     rawDataMap[ Diagnostic::dtClosedLoop ]     = mode1Data0Stream._stateByte5 & SB5_CLOSEDLOOP;
     rawDataMap[ Diagnostic::dtReachLean ]      = mode1Data0Stream._stateByte5 & SB5_REACHLEAN;

     rawDataMap[ Diagnostic::dtCatConvOver ]    = mode1Data0Stream._stateByte6 & SB6_CATCONVOVER;

     rawDataMap[ Diagnostic::dtIdleRun ]        = mode1Data0Stream._stateByte7 & SB7_IDLERUN;
     rawDataMap[ Diagnostic::dtIntReset ]       = mode1Data0Stream._stateByte7 & SB7_INTRESET;
     rawDataMap[ Diagnostic::dtRpmLow ]         = mode1Data0Stream._stateByte7 & SB7_RPMLOW;

     rawDataMap[ Diagnostic::dtIACReset ]       = mode1Data0Stream._stateByte8 & SB8_IACRESET;
     rawDataMap[ Diagnostic::dtIACResetReq ]    = mode1Data0Stream._stateByte8 & SB8_IACRESETREQ;

     rawDataMap[ Diagnostic::dtO2Ready ]        = mode1Data0Stream._stateByte9 & SB9_O2READY;

     rawDataMap[ Diagnostic::dtIACRun ]         = mode1Data0Stream._stateByte10 & SB10_IACRUN;

     rawDataMap[ Diagnostic::dtFANReq ]         = mode1Data0Stream._stateByte11 & SB11_FANREQ;

     ///////////////////////////////////////////////////////////////////////////////////////////
     DataChanged( rawDataMap );

      /*
       * Error
       * */
      _errorMutex.Lock();

      _errorTypes.clear();

      if( mode1Data0Stream._malffw1 & MALF1_EST )           { _errorTypes.insert( Diagnostic::etMalf0 ); }
      if( mode1Data0Stream._malffw1 & MALF1_HIMAP )         { _errorTypes.insert( Diagnostic::etMalf1 ); }
      if( mode1Data0Stream._malffw1 & MALF1_EGR )           { _errorTypes.insert( Diagnostic::etMalf2 ); }
      if( mode1Data0Stream._malffw1 & MALF1_SPEED )         { _errorTypes.insert( Diagnostic::etMalf3 ); }
      if( mode1Data0Stream._malffw1 & MALF1_HIMAT )         { _errorTypes.insert( Diagnostic::etMalf4 ); }
      if( mode1Data0Stream._malffw1 & MALF1_HITPS )         { _errorTypes.insert( Diagnostic::etMalf5 ); }
      if( mode1Data0Stream._malffw1 & MALF1_LOCOLTEMP )     { _errorTypes.insert( Diagnostic::etMalf6 ); }
      if( mode1Data0Stream._malffw1 & MALF1_O2 )            { _errorTypes.insert( Diagnostic::etMalf7 ); }
      if( mode1Data0Stream._malffw2 & MALF2_ESTASHORT )     { _errorTypes.insert( Diagnostic::etMalf8 ); }
      if( mode1Data0Stream._malffw2 & MALF2_ESTASHORTGND )  { _errorTypes.insert( Diagnostic::etMalf9 ); }
      if( mode1Data0Stream._malffw2 & MALF2_IMMOB )         { _errorTypes.insert( Diagnostic::etMalf10 ); }
      if( mode1Data0Stream._malffw2 & MALF2_OCTANE )        { _errorTypes.insert( Diagnostic::etMalf11 ); }
      if( mode1Data0Stream._malffw2 & MALF2_KNOCK )         { _errorTypes.insert( Diagnostic::etMalf12 ); }
      if( mode1Data0Stream._malffw2 & MALF2_ECU1 )          { _errorTypes.insert( Diagnostic::etMalf13 ); }
      if( mode1Data0Stream._malffw2 & MALF2_O2REACH )       { _errorTypes.insert( Diagnostic::etMalf14 ); }
      if( mode1Data0Stream._malffw2 & MALF2_O2LEAN )        { _errorTypes.insert( Diagnostic::etMalf15 ); }
      if( mode1Data0Stream._malffw3 & MALF3_LOTPS )         { _errorTypes.insert( Diagnostic::etMalf16 ); }
      if( mode1Data0Stream._malffw3 & MALF3_HICOLTEMP )     { _errorTypes.insert( Diagnostic::etMalf17 ); }
      if( mode1Data0Stream._malffw3 & MALF3_IAC )           { _errorTypes.insert( Diagnostic::etMalf18 ); }
      if( mode1Data0Stream._malffw3 & MALF3_KPS )           { _errorTypes.insert( Diagnostic::etMalf19 ); }
      if( mode1Data0Stream._malffw3 & MALF3_ECU2 )          { _errorTypes.insert( Diagnostic::etMalf20 ); }
      if( mode1Data0Stream._malffw3 & MALF3_INJECTOR )      { _errorTypes.insert( Diagnostic::etMalf21 ); }
      if( mode1Data0Stream._malffw3 & MALF3_ESTBSHORT )     { _errorTypes.insert( Diagnostic::etMalf22 ); }
      if( mode1Data0Stream._malffw3 & MALF3_ESTBSHORTGND )  { _errorTypes.insert( Diagnostic::etMalf23 ); }
      if( mode1Data0Stream._malffw4 & MALF4_LOMAP )         { _errorTypes.insert( Diagnostic::etMalf26 ); }
      if( mode1Data0Stream._malffw4 & MALF4_LOMAT )         { _errorTypes.insert( Diagnostic::etMalf27 ); }

      _errorMutex.UnLock();

      Adjust();

      return ret;
   }
}
