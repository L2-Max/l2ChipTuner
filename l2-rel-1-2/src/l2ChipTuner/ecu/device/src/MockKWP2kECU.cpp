/*
 * MockKWP2kECU.cpp
 *
 *  Created on: 07.11.2008
 *      Author: mars
 */

#include "MockKWP2kECU.h"

#include <KWP2kApplicationLayer.h>

#include <l2Buffer.h>

namespace Mock
{
   CKWP2kECU::CKWP2kECU( unsigned char aNumber ) : CKLine( aNumber )
   {
      _state = KWP2k::CDataLinkLayer::dlsiStopCommunicationResp;
   }

   CKWP2kECU::~CKWP2kECU()
   {
   }

   int CKWP2kECU::read( void* aData, unsigned int aSize )
   {
      int ret( -1 );

      if( !( ret = CKLine::read( aData, aSize ) ) )
      {
         l2max::CByteArray resp;
         l2max::CBuffer respStream( &resp );

         respStream << _format;
         respStream << _source;
         respStream << _target;

         if( _state == KWP2k::CDataLinkLayer::dlsiStartCommunicationReq )
         {
            _state = KWP2k::CDataLinkLayer::dlsiStartCommunicationResp;

            respStream << ( unsigned char )( 0x03 );
            respStream << _state;
            respStream << ( unsigned char )( 0xea );
            respStream << ( unsigned char )( 0x8f );
         }
         else if( _state == KWP2k::CDataLinkLayer::dlsiStopCommunicationReq )
         {
            _state = KWP2k::CDataLinkLayer::dlsiStopCommunicationResp;

            respStream << ( unsigned char )( 0x01 );
            respStream << _state;
         }
         else if( _state == KWP2k::CApplicationLayer::alsiStartDiagnosticSessionReq )
         {
            _state = KWP2k::CApplicationLayer::alsiStartDiagnosticSessionResp;

            respStream << ( unsigned char )( 0x02 );
            respStream << _state;
            respStream << _diagSession;
         }
         else if( _state == KWP2k::CApplicationLayer::alsiReadDataByLocalIdentifierReq )
         {
            _state = KWP2k::CApplicationLayer::alsiReadDataByLocalIdentifierResp;

            respStream << ( unsigned char )( 0x02 );
            respStream << _state;
            respStream << _recordLocalIdentifier;
         }
         else
         {
            resp.Clear();

            _state = KWP2k::CDataLinkLayer::dlsiStopCommunicationResp;
         }

         if( resp.size() )
         {
            respStream << KWP2k::CDataLinkLayer::checkSum( resp );
         }

         ret = aSize < resp.size() ? aSize : resp.size();

         ::memcpy( aData, resp.data(), ret );
      }

      return ret;
   }

   int CKWP2kECU::write( const void* aData, unsigned int aSize )
   {
      int ret( -1 );

      if( ( ret = CKLine::write( aData, aSize ) ) > 0 )
      {
         l2max::CByteArray req( aData, aSize );

         unsigned char checkSum( req.back() );
         req.Pop_back();

         l2max::CBuffer reqStream( &req );

         if( checkSum == KWP2k::CDataLinkLayer::checkSum( req ) )
         {
            unsigned char length( 0 );

            reqStream >> _format;
            reqStream >> _target;
            reqStream >> _source;

            if( _format & 0x3f )
            {
               length = _format & 0x3f;

               _format &= 0xc0;
            }
            else
            {
               reqStream >> length;
            }

            unsigned char serviceId( 0 );

            reqStream >> serviceId;

            if( serviceId == KWP2k::CDataLinkLayer::dlsiStartCommunicationReq )
            {
               if( _state == KWP2k::CDataLinkLayer::dlsiStopCommunicationResp )
               {
                  _state = KWP2k::CDataLinkLayer::dlsiStartCommunicationReq;
               }
            }
            else if( serviceId == KWP2k::CDataLinkLayer::dlsiStopCommunicationReq )
            {
               if( _state == KWP2k::CDataLinkLayer::dlsiStartCommunicationResp ||
                   _state == KWP2k::CApplicationLayer::alsiStartDiagnosticSessionResp )
               {
                  _state = KWP2k::CDataLinkLayer::dlsiStopCommunicationReq;
               }
            }
            else if( serviceId == KWP2k::CApplicationLayer::alsiStartDiagnosticSessionReq )
            {
               if( _state == KWP2k::CDataLinkLayer::dlsiStartCommunicationResp ||
                   _state == KWP2k::CApplicationLayer::alsiStartDiagnosticSessionResp ||
                   _state == KWP2k::CApplicationLayer::alsiReadDataByLocalIdentifierResp )
               {
                  reqStream >> _diagSession;

                  _state = KWP2k::CApplicationLayer::alsiStartDiagnosticSessionReq;
               }
            }
            else if( serviceId == KWP2k::CApplicationLayer::alsiReadDataByLocalIdentifierReq )
            {
               if( _state == KWP2k::CApplicationLayer::alsiStartDiagnosticSessionResp ||
                   _state == KWP2k::CApplicationLayer::alsiReadDataByLocalIdentifierResp )
               {
                  reqStream >> _recordLocalIdentifier;

                  _state = KWP2k::CApplicationLayer::alsiReadDataByLocalIdentifierReq;
               }
            }
         }
      }

      return ret;
   }
}
