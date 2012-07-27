/*
 * l2AbstractIODevice.cpp
 *
 *  Created on: 27.10.2008
 *      Author: mars
 */

#include "l2AbstractIODevice.h"

#include <l2ByteArray.h>

namespace l2max
{
   CAbstractIODevice::CAbstractIODevice()
   {
      _openMode = omNotOpen;
   }

   CAbstractIODevice::~CAbstractIODevice()
   {
   }

   bool CAbstractIODevice::isOpen()const
   {
      return _openMode != omNotOpen;
   }

   bool CAbstractIODevice::open( TOpenMode aOpenMode )
   {
      bool ret( false );

      if( !isOpen() )
      {
         _openMode = aOpenMode;

         ret = true;
      }

      return ret;
   }

   bool CAbstractIODevice::seek( int offset, ESeekMode seekMode )
   {
      bool ret( true );

      ret = isOpen();

      return ret;
   }

   bool CAbstractIODevice::close()
   {
      bool ret( true );

      _openMode = omNotOpen;

      return ret;
   }

   unsigned int CAbstractIODevice::position()const
   {
      return 0;
   }

   unsigned int CAbstractIODevice::size()const
   {
      return 0;
   }

   template < class T >
      CAbstractIODevice& CAbstractIODevice::operator << ( const T& anObject )
      {
         if( !isOpen() )
         {
            open( omWriteOnly );
         }

         write( &anObject, sizeof( anObject ) );

         return *this;
      }

   template < class T >
      CAbstractIODevice& CAbstractIODevice::operator >> ( T& anObject )
      {
         if( !isOpen() )
         {
            open( omReadOnly );
         }

         read( &anObject, sizeof( anObject ) );

         return *this;
      }

   template <>
      CAbstractIODevice& CAbstractIODevice::operator << < CByteArray >( const CByteArray& anObject )
      {
         if( !isOpen() )
         {
            open( omWriteOnly );
         }

         write( anObject.data(), anObject.size() );

         return *this;
      }

   template <>
      CAbstractIODevice& CAbstractIODevice::operator >> < CByteArray >( CByteArray& anObject )
      {
         if( !isOpen() )
         {
            open( omReadOnly );
         }

         if( !anObject.size() || anObject.size() != size() - position() )
         {
            anObject.Resize( size() - position() );
         }

         int ret( read( anObject.data(), anObject.size() ) );

         if( ret < 0 )
         {
            ret = 0;
         }

         anObject.Resize( ret );

         return *this;
      }

   template CAbstractIODevice& CAbstractIODevice::operator << < unsigned char >( const unsigned char& anObject );
   template CAbstractIODevice& CAbstractIODevice::operator << < CByteArray >( const CByteArray& anObject );

   template CAbstractIODevice& CAbstractIODevice::operator >> < unsigned char >( unsigned char& anObject );
   template CAbstractIODevice& CAbstractIODevice::operator >> < CByteArray >( CByteArray& anObject );
}
