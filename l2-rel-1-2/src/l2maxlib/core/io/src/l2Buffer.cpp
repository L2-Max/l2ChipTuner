/*
 * l2Buffer.cpp
 *
 *  Created on: 27.10.2008
 *      Author: mars
 */

#include "l2Buffer.h"

#include <l2ByteArray.h>

namespace l2max
{
   CBuffer::CBuffer( CByteArray* aBuffer )
   {
      _buffer = aBuffer;
      _position = 0;
   }

   CBuffer::~CBuffer()
   {
      close();
   }

   bool CBuffer::open( TOpenMode aOpenMode )
   {
      bool ret( false );

      if( !isOpen() )
      {
         _position = 0;

         ret = CAbstractIODevice::open( aOpenMode );
      }

      return ret;
   }

   int CBuffer::read( void* aData, unsigned int aSize )
   {
      int ret( -1 );

      if( isOpen() )
      {
         if( int( aSize ) > int( _buffer->size() - _position ) )
         {
            ret = _buffer->size() - _position;
         }
         else
         {
            ret = aSize;
         }

         ::memcpy( aData, reinterpret_cast< unsigned char* >( _buffer->data() ) + _position, ret );

         _position += ret;
      }

      return ret;
   }

   int CBuffer::write( const void* aData, unsigned int aSize )
   {
      int ret( -1 );

      if( isOpen() )
      {
         if( int( aSize ) > int( _buffer->size() - _position ) )
         {
            _buffer->Resize( _buffer->size() + ( aSize - ( _buffer->size() - _position ) ) );
         }

         ret = aSize;

         ::memcpy( reinterpret_cast< unsigned char* >( _buffer->data() ) + _position, aData, ret );

         _position += ret;
      }

      return ret;
   }

   bool CBuffer::seek( int offset, ESeekMode seekMode )
   {
      bool ret( false );

      if( isOpen() )
      {
         if( seekMode == smFromBegining )
         {
            _position = 0;
         }
         else if( seekMode == smFromEnd )
         {
            _position = _buffer->size() - 1;
         }

         _position += offset;

         if( int( _position ) < 0 )
         {
            _position = 0;
         }

         ret = true;
      }

      return ret;
   }

   bool CBuffer::close()
   {
      _buffer = 0;
      _position = 0;

      return CAbstractIODevice::close();
   }

   unsigned int CBuffer::position()const
   {
      return _position;
   }

   unsigned int CBuffer::size()const
   {
      unsigned int ret( 0 );

      if( isOpen() )
      {
         ret = _buffer->size();
      }

      return ret;
   }
}
