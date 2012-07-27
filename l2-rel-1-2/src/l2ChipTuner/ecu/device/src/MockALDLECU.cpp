/*
 * KLineALDLECUMock.cpp
 *
 *  Created on: 09.10.2008
 *      Author: mars
 */

#include "MockALDLECU.h"

#include <ALDLLanos15SOHCZXJNMode1Data0StreamImpl.h>
#include <ALDLProto.h>

#include <l2Trace.h>
#include <l2Buffer.h>
#include <l2Thread.h>

namespace Mock
{
   CALDLECU::CALDLECU( unsigned char aNumber ) : CKLine( aNumber ),
                                                 _file( "dump.bin" )
   {
      _firstModeDataReq = true;

      _mode = ALDL::CDataStreamLayer::mReturnToNormal;

      _file.open( l2max::CFile::omReadOnly );
      _file.seek( 4, l2max::CFile::smFromCurrent );
   }

   CALDLECU::~CALDLECU(){}

   int CALDLECU::read( void* aData, unsigned int aSize )
   {
      TRACE_FUN( Routine, "CKLineALDLECUMock::read" );

      int ret( -1 );

      if( !( ret = CKLine::read( aData, aSize ) ) )
      {
         l2max::CByteArray resp;
         l2max::CBuffer respStream( &resp );

         respStream << _ecuId;

         if( _mode == ALDL::CDataStreamLayer::mReturnToNormal )
         {
            respStream << ALDL::CDataStreamLayer::encLength( 0x01 );
            respStream << _mode;
         }
         else if( _mode == ALDL::CDataStreamLayer::mTransmitData )
         {
            static unsigned char respBuf[] =
               { 0x07, 0x41, 0x0f, 0x00, 0x00, 0x00, 0x8D, 0x5E, 0x52, 0xF9, 0x3B, 0x1F, 0x21, 0x5B, 0x00,
                 0x00, 0x00, 0x01, 0x74, 0x8D, 0x80, 0x00, 0x10, 0x1F, 0x1F, 0x87, 0x1D, 0x19, 0x23, 0x23,
                 0xFD, 0x00, 0x00, 0x00, 0x06, 0x02, 0x00, 0xBC, 0x92, 0x78, 0x00, 0x76, 0x9A, 0x00, 0x00,
                 0x00, 0x00, 0x80, 0x0D, 0x08, 0x30, 0x05, 0x83, 0x43, 0x68, 0x0E, 0xFD, 0xA5, 0x24, 0x98 };

            if( _file.isOpen() )
            {
               _file.seek( 3, l2max::CFile::smFromCurrent );
               _file.read( respBuf, sizeof( respBuf ) );
               _file.seek( 1, l2max::CFile::smFromCurrent );
            }

            if( !_table )
            {
               respStream << ALDL::CDataStreamLayer::encLength( 0x01 + sizeof( respBuf ) );
               respStream << _mode;

               respStream.write( respBuf, sizeof( respBuf ) );
            }

            if( !_firstModeDataReq )
            {
               for( int i( 0 ); i < sizeof( respBuf ); ++i )
               {
                  ++respBuf[ i ];
               }
            }

            if( _firstModeDataReq )
            {
               _firstModeDataReq = false;
            }
         }
         else if( _mode == ALDL::CDataStreamLayer::mECUFunc )
         {
            respStream << ALDL::CDataStreamLayer::encLength( _control.size() + 1 );
            respStream << _mode;

            respStream << _control;
         }
         else
         {
            resp.Clear();
         }

         if( resp.size() )
         {
            respStream << ALDL::CDataStreamLayer::checkSum( resp );
         }

         ret = aSize < resp.size() ? aSize : resp.size();

         TraceRoutine << "Resp: " << l2max::CVariant( resp ).toString() << std::endl;

         ::memcpy( aData, resp.data(), ret );
      }

      l2max::CThread::Sleep( 20 );

      return ret;
   }

   int CALDLECU::write( const void* aData, unsigned int aSize )
   {
      TRACE_FUN( Routine, "CKLineALDLECUMock::write" );

      int ret( -1 );

      if( ( ret = CKLine::write( aData, aSize ) ) > 0 )
      {
         l2max::CByteArray req( aData, aSize );

         TraceRoutine << "Req: " << l2max::CVariant( req ).toString() << std::endl;

         unsigned char checkSum( req.back() );
         req.Pop_back();

         if( checkSum == ::ALDL::CProto::checkSum( req ) )
         {
            l2max::CBuffer reqStream( &req );

            reqStream >> _ecuId;

            unsigned char length( 0 );
            reqStream >> length;

            unsigned char mode( 0 );
            reqStream >> mode;

            if( mode == ALDL::CDataStreamLayer::mReturnToNormal )
            {
               _firstModeDataReq = true;
            }
            else if( mode == ALDL::CDataStreamLayer::mTransmitData )
            {
               reqStream >> _table;
            }
            else if( mode == ALDL::CDataStreamLayer::mECUFunc )
            {
               reqStream >> _control;
            }

            _mode = mode;

            ret = aSize;
         }
      }

      return ret;
   }
}
