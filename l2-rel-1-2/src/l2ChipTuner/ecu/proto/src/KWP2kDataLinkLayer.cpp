/*
 * KWP2kDataLinkLayer.cpp
 *
 *  Created on: 06.11.2008
 *      Author: mars
 */

#include "KWP2kDataLinkLayer.h"

#include <AbstractAdapter.h>

#include <l2Buffer.h>
#include <l2ByteArray.h>

namespace KWP2k
{
   const unsigned short CDataLinkLayer::_maxResponseLength( 0x104 );

   CDataLinkLayer::CDataLinkLayer( CAbstractAdapter& anAdapter ) : CAbstractProto( anAdapter, Protocol::ptKWP2k ){}
   CDataLinkLayer::~CDataLinkLayer(){}

   bool CDataLinkLayer::StartCommunication()
   {
      bool ret( false );

      l2max::CByteArray resp;

      if( ret = MakeTransaction( dlsiStartCommunicationReq, l2max::CByteArray(), resp ) )
      {
         l2max::CBuffer respStream( &resp );

         unsigned char serviceId( 0 );

         respStream >> serviceId;

         if( serviceId == dlsiStartCommunicationResp )
         {
            respStream >> _keyByte1;
            respStream >> _keyByte2;
         }
         else
         {
            _errorCode = ecResponseUnExpected;
         }
      }

      return ret && _errorCode == ecNoError;
   }

   bool CDataLinkLayer::StopCommunication()
   {
      bool ret( false );

      l2max::CByteArray resp;

      if( ret = MakeTransaction( dlsiStopCommunicationReq, l2max::CByteArray(), resp ) )
      {
         l2max::CBuffer respStream( &resp );

         unsigned char serviceId( 0 );

         respStream >> serviceId;

         if( serviceId != dlsiStopCommunicationResp )
         {
            l2max::CByteArray resp;
            respStream >> resp;

            if( !( ret = ProcessNegativeResponse( dlsiStopCommunicationReq, resp ) ) )
            {
               _errorCode = ecResponseUnExpected;
            }
            else
            {
               ret = ( _negativeResponseHandlerResponseCode == nrhrcNoError );
            }
         }
      }

      return ret && _errorCode == ecNoError;
   }

   void CDataLinkLayer::SetAddressingType( EAddressingType anAddressingType )
   {
      _addressingType = anAddressingType;
   }

   void CDataLinkLayer::SetTargetAddress( unsigned char aByte )
   {
      _targetAddress = aByte;
   }

   void CDataLinkLayer::SetSourceAddress( unsigned char aByte )
   {
      _sourceAddress = aByte;
   }

   unsigned char CDataLinkLayer::checkSum( const l2max::CByteArray& aData )
   {
      unsigned char ret( 0 );

      for( int i( 0 ); i < aData.size(); ++i )
      {
         ret += aData[ i ];
      }

      return ret;
   }

   unsigned char CDataLinkLayer::formatByte( unsigned char aServiceId, unsigned char aLength )const
   {
      unsigned char ret( 0 );

      ret = _addressingType;

      if( aServiceId == dlsiStartCommunicationReq )
      {
         ret |= aLength;
      }

      return ret;
   }

   unsigned char CDataLinkLayer::targetByte()const
   {
      return _targetAddress;
   }

   unsigned char CDataLinkLayer::sourceByte()const
   {
      return _sourceAddress;
   }

   l2max::CByteArray CDataLinkLayer::reqHeader( unsigned char aServiceId, unsigned char aLength )const
   {
      l2max::CByteArray ret;
      l2max::CBuffer reqStream( &ret );

      reqStream << formatByte( aServiceId, aLength );
      reqStream << targetByte();
      reqStream << sourceByte();

      if( aServiceId != dlsiStartCommunicationReq )
      {
         reqStream << aLength;
      }

      return ret;
   }

   bool CDataLinkLayer::MakeTransaction( unsigned char aServiceId, const l2max::CByteArray& aParam, l2max::CByteArray& aResp )
   {
      bool ret( false );

      l2max::CByteArray req;
      l2max::CBuffer reqStream( &req );

      reqStream << reqHeader( aServiceId, sizeof( aServiceId ) + aParam.size() );
      reqStream << aServiceId;
      reqStream << aParam;
      reqStream << CDataLinkLayer::checkSum( req );

      if( ret = _adapter.Write( req ) )
      {
         l2max::CByteArray resp;
         resp.Resize( CDataLinkLayer::_maxResponseLength );

         if( ret = _adapter.Read( resp ) )
         {
            if( resp.size() )
            {
               unsigned char checkSum( resp.back() );
               resp.Pop_back();

               if( checkSum != CDataLinkLayer::checkSum( resp ) )
               {
                  _errorCode = ecBinaryError;
               }

               if( _errorCode == ecNoError )
               {
                  l2max::CBuffer respStream( &resp );

                  unsigned char format( 0 );
                  unsigned char target( 0 );
                  unsigned char source( 0 );

                  respStream >> format;
                  respStream >> target;
                  respStream >> source;

                  if( format == formatByte( 0, 0 ) &&
                      target == sourceByte() &&
                      source == targetByte() )
                  {
                     unsigned char length( 0 );

                     respStream >> length;
                     respStream >> aResp;

                     if( aResp.size() != length )
                     {
                        _errorCode = ecBinaryError;
                     }
                  }
                  else
                  {
                     _errorCode = ecResponseUnExpected;
                  }
               }
            }
            else
            {
               _errorCode = ecNoResponse;
            }
         }
      }

      return ret && _errorCode == ecNoError;
   }
}
