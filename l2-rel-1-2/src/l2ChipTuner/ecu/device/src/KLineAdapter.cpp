/*
 * KLineAdapter.cpp
 *
 *  Created on: 08.10.2008
 *      Author: mars
 */

#include "KLineAdapter.h"

#include "MockALDLECU.h"
#include "MockKWP2kECU.h"

#include <AbstractProto.h>

#include <AppDefs.h>
#include <AppContext.h>

#include <l2SerialLine.h>
#include <l2ByteArray.h>
#include <l2Trace.h>
#include <l2File.h>

bool CKLineAdapter::_debug( false );

CKLineAdapter::CKLineAdapter()
{
   _errorTexts[ ecCommDevOpenFail ] = "Serial line open fail";
   _errorTexts[ ecCommDevSetupFail ] = "Serial line set param fail";

   _device = 0;
}

CKLineAdapter::~CKLineAdapter()
{
   CloseDevice();
}

bool CKLineAdapter::Initialize()
{
   TRACE_FUN( Routine, "CKLineAdapter::Initialize" );

   _errorCode = ecNoError;

   l2max::CVariant lineNum;
   CAppContext::GetInstance()->MainRegistry().Read( V_ECU_ADAPT_COMMDEVTYP_SL_NUM, lineNum );

   l2max::CVariant proto;
   CAppContext::GetInstance()->MainRegistry().Read( V_ECU_PROTO_TYPE, proto );

   l2max::CSerialLine* serialLine( 0 );

   if( CKLineAdapter::_debug )
   {
      if( static_cast< Protocol::EProtocolType >( proto.toInt() ) == Protocol::ptALDL )
      {
         serialLine = new ::Mock::CALDLECU( lineNum.toInt() );
      }
      else if( static_cast< Protocol::EProtocolType >( proto.toInt() ) == Protocol::ptKWP2k )
      {
         serialLine = new ::Mock::CKWP2kECU( lineNum.toInt() );
      }
   }
   else
   {
      serialLine = new l2max::CSerialLine( lineNum.toInt() );
   }

   _device = serialLine;

   l2max::CVariant isReadFromFileEnabled;
   CAppContext::GetInstance()->MainRegistry().Read( V_ECU_DIAG_SESS_DATAFILER_E, isReadFromFileEnabled );

   if( !CKLineAdapter::_debug && !isReadFromFileEnabled.toInt() )
   {
      if( !serialLine->open( l2max::CAbstractIODevice::omReadWrite ) )
      {
         _errorCode = ecCommDevOpenFail;
      }

      if( _errorCode == ecNoError )
      {
         l2max::SSerialLineParam param;

         l2max::CVariant baudRate;
         l2max::CVariant rdTimeo;
         l2max::CVariant wrTimeo;
         l2max::CVariant bufSize;

         CAppContext::GetInstance()->MainRegistry().Read( V_ECU_ADAPT_COMMDEVTYP_SL_BPS, baudRate );
         CAppContext::GetInstance()->MainRegistry().Read( V_ECU_ADAPT_COMMDEVTYP_SL_RDTIMEO, rdTimeo );
         CAppContext::GetInstance()->MainRegistry().Read( V_ECU_ADAPT_COMMDEVTYP_SL_WRTIMEO, wrTimeo );
         CAppContext::GetInstance()->MainRegistry().Read( V_ECU_ADAPT_COMMDEVTYP_SL_BUFSZ, bufSize );

         param._baudRate = baudRate.toInt();
         param._byteSize = 8;
         param._stopBits = l2max::SSerialLineParam::sbOne;
         param._parity = l2max::SSerialLineParam::pNoParity;


         unsigned int bitsPerFrame( param._byteSize + 1 );

         ++bitsPerFrame;

         if( param._stopBits != l2max::SSerialLineParam::sbOne )
         {
            ++bitsPerFrame;
         }

         unsigned int msecPerFrame( static_cast< unsigned int >( 1000. / param._baudRate * bitsPerFrame ) + 1 );

         unsigned int readInt( msecPerFrame + rdTimeo.toInt() );

         unsigned int readMult( msecPerFrame + readInt / 2 );
         unsigned int writeMult( msecPerFrame + readInt / 2 );

         if( !serialLine->SetParam( param ) ||
             !serialLine->SetConstantTimeOuts( 0, wrTimeo.toInt() ) ||
             !serialLine->SetMultiplierTimeOuts( readMult, writeMult ) ||
             !serialLine->SetReadIntervalTimeOuts( readInt ) ||
             !serialLine->SetBuffersSize( bufSize.toInt(), bufSize.toInt() ) )
         {
            _errorCode = ecCommDevSetupFail;
         }
      }

      if( _errorCode != ecNoError )
      {
         CloseDevice();
      }
   }

   return _errorCode == ecNoError;
}

void CKLineAdapter::UnInitialize()
{
   CloseDevice();
}

void CKLineAdapter::CloseDevice()
{
   delete _device;

   _device = 0;
}

bool CKLineAdapter::Write( const l2max::CByteArray& aData )const
{
   TRACE_FUN( Routine, "CKLineAdapter::Write" );

   if( _device->write( aData.data(), aData.size() ) != aData.size() )
   {
      _errorCode = ecWriteFail;
   }

   l2max::CByteArray resp;
   resp.Resize( aData.size() );

   if( _errorCode == ecNoError )
   {
      if( _device->read( resp.data(), resp.size() ) != resp.size() ||
          aData != resp )
      {
         _errorCode = ecAdapterTrouble;
      }
   }

   return _errorCode == ecNoError;
}

bool CKLineAdapter::Read( l2max::CByteArray& aData )const
{
   TRACE_FUN( Routine, "CKLineAdapter::Read" );

   int bytesRead( _device->read( aData.data(), aData.size() ) );

   if( bytesRead < 0 )
   {
      bytesRead = 0;

      _errorCode = ecReadFail;
   }

   aData.Resize( bytesRead );

   return _errorCode == ecNoError;
}
