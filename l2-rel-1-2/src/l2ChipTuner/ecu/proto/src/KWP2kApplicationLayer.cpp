/*
 * KWP2kApplicationLayer.cpp
 *
 *  Created on: 06.11.2008
 *      Author: mars
 */

#include "KWP2kApplicationLayer.h"

#include <l2Buffer.h>
#include <l2ByteArray.h>
#include <l2Trace.h>

namespace KWP2k
{
   CApplicationLayer::CApplicationLayer( CAbstractAdapter& anAdapter ) : CDataLinkLayer( anAdapter )
   {
   }

   CApplicationLayer::~CApplicationLayer()
   {
   }

   bool CApplicationLayer::StartDiagnosticSession()
   {
      TRACE_FUN( Routine, "CApplicationLayer::StartDiagnosticSession" );
      
      bool ret( false );

      if( ret = StartCommunication() )
      {
         ret = StartStandardDiagnosticSession();
      }

      return ret && _errorCode == ecNoError;
   }

   bool CApplicationLayer::StopDiagnosticSession()
   {
      TRACE_FUN( Routine, "CApplicationLayer::StopDiagnosticSession" );
      
      bool ret( false );

      if( ret = StartStandardDiagnosticSession() )
      {
         ret = StopCommunication();
      }

      return ret;
   }

   bool CApplicationLayer::ReadDataByLocalIdentifier()
   {
      TRACE_FUN( Routine, "CApplicationLayer::ReadDataByLocalIdentifier" );
      
      bool ret( false );

      l2max::CByteArray param;
      l2max::CBuffer paramStream( &param );

      paramStream << ( unsigned char )( alrliLocalIdentifierScalingTable );

      l2max::CByteArray resp;

      if( ret = MakeTransaction( alsiReadDataByLocalIdentifierReq, param, resp ) )
      {
         l2max::CBuffer respStream( &resp );

         unsigned char serviceId( 0 );
         respStream >> serviceId;

         if( serviceId == alsiReadDataByLocalIdentifierResp )
         {
            respStream >> serviceId;

            if( serviceId != alrliLocalIdentifierScalingTable )
            {
               _errorCode = ecResponseUnExpected;
            }
         }
         else
         {
            l2max::CByteArray resp;
            respStream >> resp;

            if( ! ( ret == ProcessNegativeResponse( alsiReadDataByLocalIdentifierReq, resp ) ) )
            {
               _errorCode = ecResponseUnExpected;
            }
            else
            {
               ret = ( _negativeResponseHandlerResponseCode == nrhrcNoError );
            }
         }
      }

      return ret;
   }
   
   bool CApplicationLayer::WriteDataByLocalIdentifier()
   {
      TRACE_FUN( Routine, "CApplicationLayer::WriteDataByLocalIdentifier" );
      
      bool ret( false );
      
      return ret;
   }

   bool CApplicationLayer::StartStandardDiagnosticSession()
   {
      TRACE_FUN( Routine, "CApplicationLayer::StartStandardDiagnosticSession" );
      
      bool ret( false );

      l2max::CByteArray param;
      l2max::CBuffer paramStream( &param );

      paramStream << ( unsigned char )( aldsStandardSession );

      l2max::CByteArray resp;

      if( ret = MakeTransaction( alsiStartDiagnosticSessionReq, param, resp ) )
      {
         l2max::CBuffer respStream( &resp );

         unsigned char serviceId( 0 );
         respStream >> serviceId;

         if( serviceId == alsiStartDiagnosticSessionResp )
         {
            respStream >> _diagnosticSessionType;

            if( _diagnosticSessionType != aldsStandardSession )
            {
               _errorCode = ecResponseUnExpected;
            }
         }
         else
         {
            l2max::CByteArray resp;
            respStream >> resp;

            if( ! ( ret == ProcessNegativeResponse( alsiStartDiagnosticSessionReq, resp ) ) )
            {
               _errorCode = ecResponseUnExpected;
            }
            else
            {
               ret = ( _negativeResponseHandlerResponseCode == nrhrcNoError );
            }
         }
      }

      return ret;
   }
}
