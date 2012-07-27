/*
 * ALDLLanosYAIHMode1Data0StreamImpl.cpp
 *
 *  Created on: 29.10.2008
 *      Author: mars
 */

#include "ALDLLanos16DOHCYAIHMode1Data0StreamImpl.h"
#include "ALDLDiagnosticDataFormulas.h"

#include <l2MutexLocker.h>
#include <l2AbstractIODevice.h>
#include <l2Variant.h>
#include <l2Trace.h>

namespace ALDL
{
   ////////////////////////////////////////////////////////////////////////////////////////
   struct SLanosYAIHMode1Data0Stream
   {
      static const unsigned char _size = 60;

      SLanosYAIHMode1Data0Stream()
      {
         ::memset( this, 0, sizeof( *this ) );
      }

      unsigned char _promid[ 2 ];

      unsigned char _malffw1;
      unsigned char _malffw2;
      unsigned char _malffw3;
      unsigned char _malffw4;

      unsigned char _not_used1[ 2 ];

      unsigned char _cooldeg;
      unsigned char _matdeg;
      unsigned char _admap;

      unsigned char _not_used2;

      unsigned char _adthrot;
      unsigned char _ntrpmx;

      unsigned char _not_used3;

      unsigned char _filtmph;

      unsigned char _not_used4;

      unsigned char _acpres;

      unsigned char _not_used5[ 2 ];

      unsigned char _adbat;
      unsigned char _fildimpt;

      unsigned char _sap[ 2 ];
      unsigned char _isspmp;

      unsigned char _not_used6[ 11 ];

      unsigned char _dispbpw[ 2 ];

      unsigned char _airfuel;
   };

   ////////////////////////////////////////////////////////////////////////////////////////
   CLanos16DOHCYAIHMode1Data0StreamImpl::CLanos16DOHCYAIHMode1Data0StreamImpl(){}
   CLanos16DOHCYAIHMode1Data0StreamImpl::~CLanos16DOHCYAIHMode1Data0StreamImpl(){}

   bool CLanos16DOHCYAIHMode1Data0StreamImpl::Deserialize( l2max::CAbstractIODevice& anIODevice )
   {
      SLanosYAIHMode1Data0Stream mode1Data0Stream;

      bool ret( anIODevice.read( &mode1Data0Stream, sizeof( mode1Data0Stream ) ) == sizeof( mode1Data0Stream ) &&
                anIODevice.seek( SLanosYAIHMode1Data0Stream::_size - sizeof( mode1Data0Stream ), l2max::CAbstractIODevice::smFromCurrent ) );

      TRawDataMap rawDataMap;

      rawDataMap[ Diagnostic::dtCoolantTemp ]        = mode1Data0Stream._cooldeg;
      rawDataMap[ Diagnostic::dtMat ]                = mode1Data0Stream._matdeg;
      rawDataMap[ Diagnostic::dtMap ]                = mode1Data0Stream._admap;
      rawDataMap[ Diagnostic::dtTps ]                = mode1Data0Stream._adthrot;
      rawDataMap[ Diagnostic::dtPresentEngineSpeed ] = mode1Data0Stream._ntrpmx;
      rawDataMap[ Diagnostic::dtSpeed ]              = mode1Data0Stream._filtmph;
      rawDataMap[ Diagnostic::dtACPresure ]          = mode1Data0Stream._acpres;
      rawDataMap[ Diagnostic::dtBat ]                = mode1Data0Stream._adbat;
      rawDataMap[ Diagnostic::dtCO ]                 = mode1Data0Stream._fildimpt;
      rawDataMap[ Diagnostic::dtIACPresentPosition ] = mode1Data0Stream._isspmp;

      {
         short val( 0 );

         reinterpret_cast< unsigned char* >( &val )[ 0 ] = mode1Data0Stream._sap[ 1 ];
         reinterpret_cast< unsigned char* >( &val )[ 1 ] = mode1Data0Stream._sap[ 0 ];

         val = -val;

         rawDataMap[ Diagnostic::dtSparcAdvance ] = val;
      }

      {
         unsigned short val( 0 );

         reinterpret_cast< unsigned char* >( &val )[ 0 ] = mode1Data0Stream._dispbpw[ 1 ];
         reinterpret_cast< unsigned char* >( &val )[ 1 ] = mode1Data0Stream._dispbpw[ 0 ];

         rawDataMap[ Diagnostic::dtBasePulseWidth ] = val;
      }

      rawDataMap[ Diagnostic::dtAirFuel ] = mode1Data0Stream._airfuel;
      
      DataChanged( rawDataMap );
   }
}
