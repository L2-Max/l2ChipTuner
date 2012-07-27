/*
 * l2SerialLine.cpp
 *
 *  Created on: 04.10.2008
 *      Author: L2-Max
 */
#include "l2SerialLine.h"

#include <l2Trace.h>

#include <fcntl.h>
#include <stdio.h>

#ifdef __WIN32__
   #include <windows.h>
#endif

namespace l2max
{
   namespace
   {
      void dcbFromParam( DCB& aDcb, const SSerialLineParam& aParam )
      {
         aDcb.BaudRate = aParam._baudRate;
         aDcb.ByteSize = aParam._byteSize;

         if( aParam._stopBits == SSerialLineParam::sbOne )
         {
            aDcb.StopBits = ONESTOPBIT;
         }
         else if( aParam._stopBits == SSerialLineParam::sbOneAndHalf )
         {
            aDcb.StopBits = ONE5STOPBITS;
         }
         else if( aParam._stopBits == SSerialLineParam::sbTwo )
         {
            aDcb.StopBits = TWOSTOPBITS;
         }

         if( aParam._parity == SSerialLineParam::pNoParity )
         {
            aDcb.Parity = NOPARITY;
         }
      }

      void paramFromDcb( SSerialLineParam& aParam, const DCB& aDcb )
      {
         aParam._baudRate = aDcb.BaudRate;
         aParam._byteSize = aDcb.ByteSize;

         if( aDcb.StopBits == ONESTOPBIT )
         {
            aParam._stopBits = SSerialLineParam::sbOne;
         }
         else if( aDcb.StopBits == ONE5STOPBITS )
         {
            aParam._stopBits = SSerialLineParam::sbOneAndHalf;
         }
         else if( aDcb.StopBits == TWOSTOPBITS )
         {
            aParam._stopBits = SSerialLineParam::sbTwo;
         }

         if( aDcb.Parity == NOPARITY )
         {
            aParam._parity = SSerialLineParam::pNoParity;
         }
      }
   }

   CSerialLine::CSerialLine( unsigned char aNumber )
   {
      TRACE_FUN( Frequently, "CSerialLine::CSerialLine" );

      char numBuf[ 4 ] = { 0 };

      sprintf( numBuf, "%d", ( unsigned int )aNumber );

      #ifdef __WIN32__
         _fileName = "COM";
      #endif

      _fileName += numBuf;

      _hFile = 0;
   }

   CSerialLine::~CSerialLine()
   {
      TRACE_FUN( Frequently, "CSerialLine::~CSerialLine" );

      close();
   }

   bool CSerialLine::open( TOpenMode aOpenMode )
   {
      TRACE_FUN( Frequently, "CSerialLine::open" );

      bool ret( false );

      if( !isOpen() )
      {
         #ifdef __WIN32__
            if( aOpenMode != omReadWrite )
            {
               TraceRoutine << "Incorrect open mode specified: " << aOpenMode << std::endl;
               TraceRoutine << "Only l2max::IIODevice::omReadWrite supported." << std::endl;
            }

            _hFile = CreateFile( _fileName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0 );

            if( INVALID_HANDLE_VALUE != _hFile )
            {
               TraceRoutine << "SerialLine was opened ..." << std::endl;
               TraceRoutine << "Initial setting is ... " << std::endl;

               DCB dcb;

               if( GetCommState( _hFile, &dcb ) )
               {
                  TraceRoutine << "   dcb.BaudRate: " << dcb.BaudRate << std::endl;
                  TraceRoutine << "   dcb.Parity: "   << dcb.Parity << std::endl;
                  TraceRoutine << "   dcb.ByteSize: " << (int)dcb.ByteSize << std::endl;
                  TraceRoutine << "   dcb.StopBits: " << (int)dcb.StopBits << std::endl;

                  paramFromDcb( _param, dcb );

                  ret = CAbstractIODevice::open( aOpenMode );
               }
            }
            else
            {
               TraceRoutine << "Open serial line " << _fileName.c_str() << " fails" << std::endl;
            }
         #endif
      }
      else
      {
         TraceRoutine << "Serial line " << _fileName.c_str() << " already opened" << std::endl;
      }

      return ret;
   }

   int CSerialLine::read( void* aData, unsigned int aSize )
   {
      TRACE_FUN( Frequently, "CSerialLine::read" );

      int ret( -1 );

      if( isOpen() )
      {
         #ifdef __WIN32__
            unsigned int size( aSize );

            DWORD dwBytesRead( 0 );

            if( ReadFile( _hFile, aData, aSize, &dwBytesRead, 0 ) )
            {
               ret = dwBytesRead;
            }
         #endif
      }

      return ret;
   }

   int CSerialLine::write( const void* aData, unsigned int aSize )
   {
      TRACE_FUN( Frequently, "CSerialLine::write" );

      int ret( -1 );

      if( isOpen() )
      {
         #ifdef __WIN32__
            DWORD dwBytesWrittem( 0 );

            if( WriteFile( _hFile, aData, aSize, &dwBytesWrittem, 0 ) )
            {
               ret = dwBytesWrittem;
            }
         #endif
      }

      return ret;
   }

   bool CSerialLine::close()
   {
      TRACE_FUN( Frequently, "CSerialLine::close" );

      bool ret( false );

      if( isOpen() )
      {
         #ifdef __WIN32__
            TerminateCommunication( tcpAll );

            ret = CloseHandle( _hFile ) && CAbstractIODevice::close();

            _hFile = 0;
         #endif
      }

      return ret;
   }

   bool CSerialLine::SetParam( const SSerialLineParam& aParam )
   {
      TRACE_FUN( Routine, "CSerialLine::SetParam" );

      bool ret( false );

      if( isOpen() )
      {
         #ifdef __WIN32__
            DCB dcb;

            if( GetCommState( _hFile, &dcb ) )
            {
               dcbFromParam( dcb, aParam );

               if( SetCommState( _hFile, &dcb ) )
               {
                  TraceRoutine << "Serial line parameters set to: " << std::endl;

                  TraceRoutine << "   dcb.BaudRate: " << dcb.BaudRate << std::endl;
                  TraceRoutine << "   dcb.fParity: "  << dcb.fParity << std::endl;
                  TraceRoutine << "   dcb.ByteSize: " << (int)dcb.ByteSize << std::endl;
                  TraceRoutine << "   dcb.StopBits: " << (int)dcb.StopBits << std::endl;

                  _param = aParam;

                  ret = true;
               }
               else
               {
                  TraceRoutine << "There are unsopperted parameters specified" << std::endl;
               }
            }
         #endif
      }

      return ret;
   }

   bool CSerialLine::SetConstantTimeOuts( unsigned int aReadTimeOut, unsigned int aWriteTimeOut )const
   {
      TRACE_FUN( Routine, "CSerialLine::SetConstantTimeOuts" );

      bool ret( false );

      if( isOpen() )
      {
         #ifdef __WIN32__
            COMMTIMEOUTS commTimeOuts;

            if( GetCommTimeouts( _hFile, &commTimeOuts ) )
            {
               TraceRoutine << "commTimeOuts.ReadTotalTimeoutConstant: " << commTimeOuts.ReadTotalTimeoutConstant << std::endl;
               TraceRoutine << "commTimeOuts.WriteTotalTimeoutConstant: " << commTimeOuts.WriteTotalTimeoutConstant << std::endl;

               commTimeOuts.ReadTotalTimeoutConstant = aReadTimeOut;
               commTimeOuts.WriteTotalTimeoutConstant = aWriteTimeOut;

               ret = SetCommTimeouts( _hFile, &commTimeOuts );
            }
         #endif
      }
      return ret;
   }

   bool CSerialLine::SetMultiplierTimeOuts( unsigned int aReadTimeOut, unsigned int aWriteTimeOut )const
   {
      TRACE_FUN( Routine, "CSerialLine::SetMultiplyTimeOuts" );

      bool ret( false );

      if( isOpen() )
      {
         #ifdef __WIN32__
            COMMTIMEOUTS commTimeOuts;

            if( GetCommTimeouts( _hFile, &commTimeOuts ) )
            {
               TraceRoutine << "commTimeOuts.ReadTotalTimeoutMultiplier: " << commTimeOuts.ReadTotalTimeoutMultiplier << std::endl;
               TraceRoutine << "commTimeOuts.WriteTotalTimeoutMultiplier: " << commTimeOuts.WriteTotalTimeoutMultiplier << std::endl;

               commTimeOuts.ReadTotalTimeoutMultiplier = aReadTimeOut;
               commTimeOuts.WriteTotalTimeoutMultiplier = aWriteTimeOut;

               ret = SetCommTimeouts( _hFile, &commTimeOuts );
            }
         #endif
      }
      return ret;
   }

   bool CSerialLine::SetReadIntervalTimeOuts( unsigned int aReadTimeOut )const
   {
      TRACE_FUN( Routine, "CSerialLine::SetReadIntervalTimeOuts" );

      bool ret( false );

      if( isOpen() )
      {
         #ifdef __WIN32__
            COMMTIMEOUTS commTimeOuts;

            if( GetCommTimeouts( _hFile, &commTimeOuts ) )
            {
               TraceRoutine << "commTimeOuts.ReadIntervalTimeout: " << commTimeOuts.ReadIntervalTimeout << std::endl;

               commTimeOuts.ReadIntervalTimeout = aReadTimeOut;

               ret = SetCommTimeouts( _hFile, &commTimeOuts );
            }
         #endif
      }
      return ret;
   }

   bool CSerialLine::SetBuffersSize( unsigned int aInSize, unsigned int aOutSize )const
   {
      bool ret( false );

      if( isOpen() )
      {
         #ifdef __WIN32__
            if( SetupComm( _hFile, aInSize, aOutSize ) )
            {
               ret = true;
            }
         #endif
      }

      return ret;
   }

   bool CSerialLine::TerminateCommunication( TTerminateCommunicationFlags aFlags )const
   {
      bool ret( false );

      if( isOpen() )
      {
         #ifdef __WIN32__
            DWORD dwFlags( 0 );

            if( aFlags & tcpWriteAbort )
            {
               dwFlags |= PURGE_TXABORT;
            }

            if( aFlags & tcpReadAbort )
            {
               dwFlags |= PURGE_RXABORT;
            }

            if( aFlags & tcpWriteClear )
            {
               dwFlags |= PURGE_TXCLEAR;
            }

            if( aFlags & tcpReadClear )
            {
               dwFlags |= PURGE_RXCLEAR;
            }

            if( PurgeComm( _hFile, dwFlags ) )
            {
               ret = true;
            }
         #endif
      }

      return ret;
   }

   bool CSerialLine::Flush()const
   {
      bool ret( false );

      if( isOpen() )
      {
         #ifdef __WIN32__
            ret = FlushFileBuffers( _hFile );
         #endif
      }

      return ret;
   }

   const SSerialLineParam& CSerialLine::param()const
   {
      return _param;
   }
}
