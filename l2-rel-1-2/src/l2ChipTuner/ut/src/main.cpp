#include <AppDefs.h>

#include <l2SerialLine.h>
#include <l2File.h>
#include <l2ByteArray.h>
#include <l2Trace.h>
#include <l2TraceStream.h>
#include <l2Context.h>
#include <l2Variant.h>
#include <l2TextIniRegistry.h>
#include <l2TextTreeRegistry.h>
#include <l2Thread.h>

#include <iostream>

void bkScan();
void drScan();

int main( int argc, char* argv[] )
{
   int ret( 0 );

   l2max::CContext ctx( argc, argv );

   l2max::CTextIniRegistry initReg;
   initReg.Open( "../datastream/aldl_l15_idle_lpg.ini" );

   l2max::CTextTreeRegistry treeReg;
   treeReg.Open( "../datastream/aldl_l15_idle_lpg.rg" );

   l2max::CVariant val;

   initReg.Read( V_DIAGF_CONF_PROTOTYPE, val );
   treeReg.Write( V_DIAGF_CONF_PROTOTYPE, val );

   initReg.Read( V_DIAGF_CONF_ALDLECUID, val );
   treeReg.Write( V_DIAGF_CONF_ALDLECUID, val );

   l2max::Registry::TKeyList keys;
   l2max::Registry::TKeyListIterator I_keys;

   int packetNo( 0 );

   for( ; ( I_keys = initReg.VariableList( "::TransmitData", keys ) ) != keys.end(); ++I_keys )
   {
      if( initReg.Read( "::TransmitData::" + *I_keys, val ) )
      {
         std::string packetKey( S_DIAGF_DIAGNOSTICDATA_PACKET );
         packetKey += "_";

         std::string number( l2max::CVariant( packetNo++ ).toString() );
         number.insert( number.begin(), 10 - number.size(), '0' );

         packetKey += number;

         std::string packetTimeoKey( packetKey + V_DIAGF_DIAGNOSTICDATA_TIMEO );
         std::string packetDataKey( packetKey + V_DIAGF_DIAGNOSTICDATA_DATA );

         std::cout << packetDataKey.c_str() << std::endl;

         treeReg.Write( packetTimeoKey, 250 );
         treeReg.Write( packetDataKey, val );
      }
      else
      {
         std::cout << "Error" << std::endl;

         return -1;
      }
   }

   initReg.Close();
   treeReg.Close();


   //drScan();
   //bkScan();

   return ret;
}

void drScan()
{
   l2max::CSerialLine* serialDev( new l2max::CSerialLine( 5 ) );
   l2max::CFile f( "zxjn.bin" );
   f.open( l2max::CFile::omReadOnly );
   f.seek( 0xff80, l2max::CFile::smFromBegining );

   if( serialDev->open( l2max::CAbstractIODevice::omReadWrite ) )
   {
      l2max::SSerialLineParam param;

      param._baudRate = 8192;
      param._byteSize = 8;
      param._stopBits = l2max::SSerialLineParam::sbOne;
      param._parity = l2max::SSerialLineParam::pNoParity;

      serialDev->SetParam( param );
      serialDev->SetConstantTimeOuts( 100, 100 );

      unsigned char b( 0 );

      while( true )
      {
         static unsigned char respBuf[] =
         { 0xF4, 0x96, 0x02,
           0x00, 0x12, 0x00, 0x00, 0x00, 0x00, 0x8D, 0x5E, 0x52, 0xF9, 0x3B, 0x1F, 0x21, 0x5B, 0x00,
           0x00, 0x00, 0x01, 0x74, 0x8D, 0x80, 0x00, 0x10, 0x1F, 0x1F, 0x87, 0x1D, 0x19, 0x23, 0x23,
           0xFD, 0x00, 0x00, 0x00, 0x06, 0x02, 0x00, 0xBC, 0x92, 0x78, 0x00, 0x76, 0x9A, 0x00, 0x00,
           0x00, 0x00, 0x80, 0x0D, 0x08, 0x30, 0x05, 0x83, 0x43, 0x68, 0x0E, 0xFD, 0xA5, 0x24, 0x98,
           0x00, 0x00, 0x00, 0x00, 0x00 };

         static bool re( false );
         if(!re)
         {
            f.read( respBuf + 3, 64 );
            re = true;
         }
         unsigned char checksum( 0 );

         for( int i( 0 ); i < sizeof( respBuf ) - 1; ++i )
         {
            checksum -= respBuf[ i ];
         }

         respBuf[ sizeof( respBuf ) - 1 ] = checksum;

         unsigned char buf[ 128 ];
         int rd( 0 );

         if( rd = serialDev->read( buf, sizeof( buf ) ) )
         {
            serialDev->write( buf, rd );

            TraceRoutine << "Req: " << l2max::CVariant( l2max::CByteArray( buf, rd ) ).toString() << std::endl;

            if( buf[ 0 ] == 0xf4 )
            {
               if( buf[ 2 ] == 0x04 )
               {
                  /*unsigned char checksum( 0 );

                  for( int i( 0 ); i < 3; ++i )
                  {
                     checksum -= buf[ i ];
                  }

                  buf[ 3 ] = checksum;*/

                  serialDev->write( buf, rd );

                  TraceRoutine << "Resp: " << l2max::CVariant( l2max::CByteArray( buf, rd ) ).toString() << std::endl;
               }
               else if( buf[ 2 ] == 0x08 || buf[ 2 ] == 0x09 )
               {
                  serialDev->write( buf, rd );
               }
               else if( buf[ 2 ] == 0x02 )
               {
                  serialDev->write( respBuf, sizeof( respBuf ) );

                  for( int i( 3 ); i < sizeof( respBuf ); ++i )
                  {
                     respBuf[ i ] += 1;
                  }
               }
            }
         }
      }
   }
   else
   {
      std::cout << "Cant open port" << std::endl;
   }
}

void bkScan()
{
   l2max::CSerialLine* serialDev( new l2max::CSerialLine( 5 ) );
   if( serialDev->open( l2max::CAbstractIODevice::omReadWrite ) )
   {
      l2max::SSerialLineParam param;

      param._baudRate = 8192;
      param._byteSize = 8;
      param._stopBits = l2max::SSerialLineParam::sbOne;
      param._parity = l2max::SSerialLineParam::pNoParity;

      serialDev->SetBuffersSize( 1024 * 3, 1024 * 3 );

      serialDev->SetParam( param );

      serialDev->SetConstantTimeOuts( 1000, 1000 );
      serialDev->SetMultiplierTimeOuts( 30, 30 );

      serialDev->SetReadIntervalTimeOuts( 50 );

      while( true )
      {
         static unsigned char respBuf[] =
         { 0xF4, 0x92, 0x01,
           0x18, 0x23, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA1, 0x67, 0x3F, 0xF9, 0x1E, 0x21, 0x65,
           0x00, 0x00, 0x33, 0x00, 0x7F, 0x8A, 0x5C, 0x00, 0x18, 0x20, 0x20, 0x42, 0x38, 0x00, 0x00,
           0x00, 0x01, 0x00, 0x3E, 0x45, 0x00, 0x01, 0x02, 0x92, 0x80, 0x12, 0x80, 0x07, 0xE8, 0x7A,
           0x00, 0x00, 0x02, 0x98, 0x25, 0x04, 0xE8, 0x08, 0x20, 0x00, 0x78, 0x03, 0xE0, 0xB4, 0x93,
           0xE7 };

         respBuf[ 59 ] = ~respBuf[ 59 ];

         for( int i( 40 ); i < sizeof( respBuf ); ++i )
         {
            respBuf[ i ] = ~respBuf[ i ];
         }

         unsigned char checksum( 0 );

         for( int i( 0 ); i < sizeof( respBuf ) - 1; ++i )
         {
            checksum -= respBuf[ i ];
         }

         respBuf[ sizeof( respBuf ) - 1 ] = checksum;

         char buf[ 5 ];
         unsigned int rd( 0 );
         if( rd = serialDev->read( buf, sizeof( buf ) ) )
         {
            std::cout << "Read: " << rd << std::endl;

            serialDev->write( respBuf, sizeof( respBuf ) );

            char _bufff[ sizeof( respBuf ) ];

            serialDev->read( _bufff, sizeof( _bufff ) );
         }
      }
   }
   else
   {
      std::cout << "Cant open port" << std::endl;
   }
}
