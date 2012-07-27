/*
 * l2AbstractIODevice.h
 *
 *  Created on: 04.10.2008
 *      Author: L2-Max
 */

#ifndef L2ABSTRACTIODEVICE_H_
#define L2ABSTRACTIODEVICE_H_

namespace l2max
{
   class CByteArray;

   class CAbstractIODevice
   {
   public:
      enum EOpenMode
      {
         omNotOpen = 0x0000,
         omReadOnly = 0x0001,
         omWriteOnly = 0x0002,
         omReadWrite = omReadOnly | omWriteOnly,
         omAppend = 0x0040,
         omTruncate = 0x0080
      };

      enum ESeekMode
      {
         smFromBegining,
         smFromCurrent,
         smFromEnd
      };

      typedef unsigned int TOpenMode;

   private:
      TOpenMode _openMode;

   public:
      CAbstractIODevice();
      virtual ~CAbstractIODevice();

      virtual unsigned int position()const;
      virtual unsigned int size()const;

      virtual int read( void* aData, unsigned int aSize ) = 0;
      virtual int write( const void* aData, unsigned int aSize ) = 0;

      virtual bool open( TOpenMode aOpenMode );
      virtual bool seek( int offset, ESeekMode seekMode );
      virtual bool close();

      bool isOpen()const;

      template < class T >
         CAbstractIODevice& operator << ( const T& anObject );

      template < class T >
         CAbstractIODevice& operator >> ( T& anObject );
   };
}

#endif /* L2ABSTRACTIODEVICE_H_ */
