#include "l2Bytearray.h"

#include "l2Exception.h"
#include "l2Trace.h"

#define L2MAX_BYTEARRAY_ALLOCATION_PAGE_SIZE 0x1000

namespace l2max
{
   CByteArray::CByteArray()
   {
   	_data = 0;
   	_size = 0;
   	_allocatedSize = 0;
   }

   CByteArray::CByteArray( const CByteArray& aValue )
   {
   	_data = 0;
   	_size = 0;
   	_allocatedSize = 0;
   	
   	*this = aValue;
   }

   CByteArray::CByteArray( const void* aValue, unsigned int aSize )
   {
   	_data = 0;
   	_size = 0;
   	_allocatedSize = 0;
   	
   	SetData( aValue, aSize );
   }

   CByteArray::CByteArray( const char* aValue )
   {
      _data = 0;
      _size = 0;
      _allocatedSize = 0;
      
      SetData( aValue, strlen( aValue ) + 1 );
   }

   CByteArray::~CByteArray()
   {
   	Clear();
   }

   CByteArray& CByteArray::operator = ( const CByteArray& aValue )
   {
   	SetData( aValue._data, aValue._size );

   	return *this;
   }

   unsigned char CByteArray::operator[]( unsigned int anIndex )const
   {
   	return reinterpret_cast< const unsigned char* >( _data )[ anIndex ];
   }

   unsigned char& CByteArray::operator[]( unsigned int anIndex )
   {
   	return reinterpret_cast< unsigned char* >( _data )[ anIndex ];
   }

   void* CByteArray::data()
   {
   	return _data;
   }

   const void* CByteArray::data()const
   {
   	return _data;
   }

   unsigned int CByteArray::size()const
   {
   	return _size;
   }

   void CByteArray::SetData( const void* aValue, unsigned int aSize, bool anAlloc )
   {
      TRACE_FUN( Frequently, "CByteArray::SetData" );
      
   	if( anAlloc && ( !_allocatedSize ||
                       aSize < ( _allocatedSize - L2MAX_BYTEARRAY_ALLOCATION_PAGE_SIZE ) ||
                       aSize > _allocatedSize ) )
   	{
   		delete _data;

   		_allocatedSize = aSize + L2MAX_BYTEARRAY_ALLOCATION_PAGE_SIZE - ( aSize % L2MAX_BYTEARRAY_ALLOCATION_PAGE_SIZE );
   		_data = new unsigned char[ _allocatedSize ];
   	}
   	
   	_size = aSize;

   	memcpy( _data, aValue, _size );
   }

   void CByteArray::Clear()
   {
   	delete[] _data;

   	_data = 0;
   	_size = 0;
   	_allocatedSize = 0;
   }

   void CByteArray::Resize( unsigned int aSize )
   {
      TRACE_FUN( Frequently, "CByteArray::Resize" );

      if( !_allocatedSize ||
          aSize < ( _allocatedSize - L2MAX_BYTEARRAY_ALLOCATION_PAGE_SIZE ) ||
          aSize > _allocatedSize )
      {
         unsigned char* oldData( _data );
         
         _allocatedSize = aSize + L2MAX_BYTEARRAY_ALLOCATION_PAGE_SIZE - ( aSize % L2MAX_BYTEARRAY_ALLOCATION_PAGE_SIZE );
         _data = new unsigned char[ _allocatedSize ];
         
         if( oldData )
         {
            if( _size > aSize )
            {
               _size = aSize;
            }
            
            SetData( oldData, _size, false );
            
            delete[] oldData;
         }
      }
      
      _size = aSize;
   }

   void CByteArray::Append( const void* aValue, unsigned int aSize, bool atEnd )
   {
      TRACE_FUN( Frequently, "CByteArray::Append" );

   	unsigned int oldSize( _size );

   	Resize( _size + aSize );

   	if( atEnd )
   	{
   		memcpy( _data + oldSize, aValue, aSize );
   	}
   	else
   	{
   		memmove( reinterpret_cast< char* >( _data ) + aSize, reinterpret_cast< char* >( _data ), _size - aSize );
   		memcpy( reinterpret_cast< char* >( _data ), aValue, aSize );
   	}
   }

   void CByteArray::Append( const CByteArray& aValue, bool atEnd )
   {
   	Append( aValue._data, aValue._size, atEnd );
   }

   void CByteArray::Reverse()
   {
      TRACE_FUN( Frequently, "CByteArray::Reverse" );

   	char* ptr( reinterpret_cast< char* >( _data ) );

   	for( int i( 0 ), j( _size - 1 ); i < j; ++i, --j )
   	{
   		char t( ptr[ i ] );
   		ptr[ i ] = ptr[ j ];
   		ptr[ j ] = t;
   	}
   }

   void CByteArray::Fill( unsigned char aPattern )
   {
      TRACE_FUN( Frequently, "CByteArray::Fill" );

      unsigned int pattern( aPattern << 24 | aPattern << 16 | aPattern << 8 | aPattern );

      for( int i( 0 ); i < _size + 4 >> 2; ++i )
      {
         reinterpret_cast< unsigned int* >( _data )[ i ] = pattern;
      }
   }

   void CByteArray::Push_back( unsigned char aByte )
   {
      TRACE_FUN( Frequently, "CByteArray::Push_back" );

      Resize( size() + 1 );

      ( *this )[ size() - 1 ] = aByte;
   }

   void CByteArray::Pop_back()
   {
      TRACE_FUN( Frequently, "CByteArray::Pop_back" );

      if( size() )
      {
         Resize( size() - 1 );
      }
   }

   unsigned char& CByteArray::back()
   {
      unsigned char* ret( 0 );

      if( size() )
      {
         ret = &( *this )[ size() - 1 ];
      }

      return *ret;
   }

   unsigned char CByteArray::back()const
   {
      unsigned char ret( 0 );

      if( size() )
      {
         ret = ( *this )[ size() - 1 ];
      }

      return ret;
   }

   bool CByteArray::operator == ( const CByteArray& aValue )const
   {
      TRACE_FUN( Frequently, "CByteArray::operator ==" );

      bool ret( _size == aValue._size );

      for( int i( 0 ); ret && i < _size >> 2; ++i )
      {
         if( reinterpret_cast< unsigned int* >( _data )[ i ] != reinterpret_cast< unsigned int* >( aValue._data )[ i ] )
         {
            ret = false;
         }
      }

      for( int i( _size - _size % sizeof( unsigned int ) ); ret && i < _size; ++i )
      {
         if( reinterpret_cast< unsigned char* >( _data )[ i ] != reinterpret_cast< unsigned char* >( aValue._data )[ i ] )
         {
            ret = false;
         }
      }

      return ret;
   }

   bool CByteArray::operator != ( const CByteArray& aValue )const
   {
      return !operator == ( aValue );
   }
}
