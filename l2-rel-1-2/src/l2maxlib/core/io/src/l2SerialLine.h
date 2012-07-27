/*
 * l2SerialLine.h
 *
 *  Created on: 04.10.2008
 *      Author: L2-Max
 */

#ifndef L2SERIALLINE_H_
#define L2SERIALLINE_H_

#include "l2AbstractIODevice.h"

#include <string>

namespace l2max
{
   struct SSerialLineParam
   {
      enum EStopBits
      {
         sbOne,
         sbOneAndHalf,
         sbTwo
      };

      enum EParity
      {
         pNoParity
      };

      unsigned int _baudRate;
      unsigned char _byteSize;

      EStopBits _stopBits;
      EParity _parity;
   };

   class CSerialLine : public CAbstractIODevice
   {
      SSerialLineParam _param;

   public:
      enum ETerminateCommunicationFlags
      {
         tcpWriteAbort = 0x01,
         tcpReadAbort = 0x02,
         tcpWriteClear = 0x04,
         tcpReadClear = 0x08,
         tcpAll = 0xff
      };

      typedef unsigned int TTerminateCommunicationFlags;

   private:
      std::string _fileName;

      void* _hFile;

   public:
      CSerialLine( unsigned char aNumber );
      virtual ~CSerialLine();

      virtual bool open( TOpenMode aOpenMode );
      virtual int read( void* aData, unsigned int aSize );
      virtual int write( const void* aData, unsigned int aSize );
      virtual bool close();

      bool SetParam( const SSerialLineParam& aParam );
      bool SetBuffersSize( unsigned int aInSize, unsigned int aOutSize )const;
      bool TerminateCommunication( TTerminateCommunicationFlags aFlags = tcpAll )const;
      bool Flush()const;

      bool SetConstantTimeOuts( unsigned int aReadTimeOut, unsigned int aWriteTimeOut )const;
      bool SetMultiplierTimeOuts( unsigned int aReadTimeOut, unsigned int aWriteTimeOut )const;
      bool SetReadIntervalTimeOuts( unsigned int aReadTimeOut )const;

      const SSerialLineParam& param()const;
   };

}

#endif /* L2SERIALLINE_H_ */
