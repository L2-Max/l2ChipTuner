/*
 * ALDLDataStreamLayer.cpp
 *
 *  Created on: 14.11.2008
 *      Author: mars
 */

#include "ALDLDataStreamLayer.h"
#include "AbstractAdapter.h"

#include <l2ByteArray.h>
#include <l2Buffer.h>

namespace ALDL
{
   const unsigned char CDataStreamLayer::_lengthBase( 0x55 );
   const unsigned char CDataStreamLayer::_maxResponseLength( 0xff - CDataStreamLayer::_lengthBase + 0x03 );

   CDataStreamLayer::CDataStreamLayer( CAbstractAdapter& anAdapter ) : CAbstractProto( anAdapter, Protocol::ptALDL ){}

   CDataStreamLayer::~CDataStreamLayer(){}

   unsigned char CDataStreamLayer::ecuId()const
   {
      return _ecuId;
   }

   unsigned char CDataStreamLayer::encLength( unsigned char aLength )
   {
      return aLength + CDataStreamLayer::_lengthBase;
   }

   unsigned char CDataStreamLayer::decLength( unsigned char aLength )
   {
      return aLength - CDataStreamLayer::_lengthBase;
   }

   void CDataStreamLayer::SetECUId( unsigned char anECUId )
   {
      _ecuId = anECUId;
   }

   l2max::CByteArray CDataStreamLayer::reqHeader( unsigned char aLength )const
   {
      l2max::CByteArray req;
      l2max::CBuffer reqStream( &req );

      reqStream << ecuId();
      reqStream << encLength( aLength );

      return req;
   }

   bool CDataStreamLayer::MakeTransaction( unsigned char aMode, const l2max::CByteArray& aParam, l2max::CByteArray& aResp )
   {
      bool ret( false );

      l2max::CByteArray req;
      l2max::CBuffer reqStream( &req );

      reqStream << reqHeader( sizeof( aMode ) + aParam.size() );
      reqStream << aMode;
      reqStream << aParam;
      reqStream << CDataStreamLayer::checkSum( req );

      if( ret = _adapter.Write( req ) )
      {
         l2max::CByteArray resp;
         resp.Resize( _maxResponseLength );

         if( ret = _adapter.Read( resp ) )
         {
            if( resp.size() )
            {
               unsigned char checkSum( resp.back() );
               resp.Pop_back();

               if( checkSum != CDataStreamLayer::checkSum( resp ) )
               {
                  _errorCode = ecBinaryError;
               }

               if( _errorCode == ecNoError )
               {
                  l2max::CBuffer respStream( &resp );

                  unsigned char respEcuId( 0 );
                  unsigned char respMsgLength( 0 );

                  respStream >> respEcuId;
                  respStream >> respMsgLength;

                  if( respEcuId == ecuId() )
                  {
                     l2max::CByteArray resp;
                     respStream >> resp;

                     if( resp.size() != decLength( respMsgLength ) )
                     {
                        _errorCode = ecBinaryError;
                     }

                     if( _errorCode == ecNoError )
                     {
                        l2max::CBuffer respStream( &resp );

                        unsigned char respMode( 0 );

                        respStream >> respMode;

                        if( respMode == aMode )
                        {
                           respStream >> aResp;
                        }
                        else
                        {
                           _errorCode = ecResponseUnExpected;
                        }
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

   unsigned char CDataStreamLayer::checkSum( const l2max::CByteArray& aByteArray )
   {
      unsigned char ret( 0 );

      for( int i( 0 ); i < aByteArray.size(); ++i )
      {
         ret -= aByteArray[ i ];
      }

      return ret;
   }
}
