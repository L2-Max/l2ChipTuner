#include "l2PackedArray.hxx"

#include "l2Trace.h"
#include "l2File.h"

#include <memory.h>

#include <set>

namespace l2max
{
	template class CPackedArray< float >;

	template< class TValue >
		struct CPackedArray< TValue >::SCompressionPredicate
		{
	      TValue _precission;

	      bool operator()( const TValue& l, const TValue& r )const
			{
				bool ret( false );

				float diff( l - r );

				if( diff < 0 )
				{
				   diff = -diff;
				}

				if( diff > _precission )
				{
					ret = l < r;
				}

				return ret;
			}
		};

	template< class TValue >
		CPackedArray< TValue >::CPackedArray()
		{
			_data = 0;
			_size = 0;

			_indexTable = 0;
			_indexLenght = 0;
			_indexTableSize = 0;
		}

	template< class TValue >
		CPackedArray< TValue >::CPackedArray( const CPackedArray& pa )
		{
			_data = 0;
			_size = 0;

			_indexTable = 0;
			_indexLenght = 0;
			_indexTableSize = 0;

			*this = pa;
		}

	template< class TValue >
		CPackedArray< TValue >::~CPackedArray()
		{
			delete _data;
			delete _indexTable;
		}

	template< class TValue >
		CPackedArray< TValue >& CPackedArray< TValue >::operator = ( const CPackedArray& pa )
		{
			if( IsCompressed() )
			{
			   delete _indexTable;

			   _indexTable = 0;
			   _indexLenght = 0;
			   _indexTableSize = 0;
			}

			SetSize( pa._size );

			if( pa.IsCompressed() )
			{
			   _indexLenght = pa._indexLenght;
			   _indexTableSize = pa._indexTableSize;
			   _indexTable = new unsigned char[ _indexTableSize ];

			   memcpy( _indexTable, pa._indexTable, _indexTableSize );
			}

			memcpy( _data, pa._data, sizeof( TValue ) * _size );

			return *this;
		}

	template< class TValue >
	   bool CPackedArray< TValue >::IsCompressed()const
	   {
	      return _indexTable && _indexLenght && _indexTableSize;
	   }

	template< class TValue >
		unsigned int CPackedArray< TValue >::MakeIndex( unsigned int index )const
		{
			unsigned int newIndex( index );

			if( IsCompressed() )
			{
         	unsigned char* indexPtr( _indexTable + _indexLenght * index / 8 );
         	unsigned char indexOffset( _indexLenght * index % 8 );

         	newIndex = *indexPtr++ & ( 0xff >> indexOffset );

         	for( unsigned char x( 0 ); x < ( _indexLenght + indexOffset - 8 ) / 8; ++x )
         	{
         		newIndex <<= 8;
         		newIndex |= *indexPtr++;
         	}

         	unsigned int indexTail( ( _indexLenght + indexOffset ) % 8 );

         	if( indexTail )
         	{
         	   if( _indexLenght + indexOffset > 8 )
         	   {
            	   newIndex <<= 8;
            	   newIndex |= *indexPtr++;
         	   }

         	   newIndex >>= 8 - indexTail;
         	}
			}

			return newIndex;
		}

	template< class TValue >
		TValue& CPackedArray< TValue >::operator[]( unsigned int index )
		{
			return _data[ MakeIndex( index ) ];
		}

	template< class TValue >
		const TValue& CPackedArray< TValue >::operator[]( unsigned int index )const
		{
			return _data[ MakeIndex( index ) ];
		}

	template< class TValue >
		void CPackedArray< TValue >::SetSize( unsigned int size )
		{
			delete _data;
			_data = 0;

			_size = size;

			if( _size )
			{
			   _data = new TValue[ _size ];
			}
		}

	template< class TValue >
		void CPackedArray< TValue >::Compress( TValue precission )
		{
	      TRACE_FUN( Routine, "CPackedArray< TValue >::Compress" );

	      if( IsCompressed() )
	      {
	         return;
	      }

			typedef typename std::set< TValue, SCompressionPredicate > TValueCompressor;

			SCompressionPredicate compressionPredicate;
			compressionPredicate._precission = precission;

			TValueCompressor valueCompressor( compressionPredicate );

		   for( int i( 0 ); i < _size; ++i )
		   {
		   	valueCompressor.insert( _data[ i ] );
		   }

		   _indexLenght = 32;

		   do
		   {
		      --_indexLenght;
		   }
		   while( !( valueCompressor.size() & ( 1 << _indexLenght ) ) );

		   ++_indexLenght;

		   _indexTableSize = _indexLenght * _size / 8 + 1;

		   _indexTable = new unsigned char[ _indexTableSize ];
		   memset( _indexTable, 0, _indexTableSize );

		   TValue* newData( new TValue[ valueCompressor.size() ] );
		   unsigned int newDataIndexItr( 0 );

		   for( typename TValueCompressor::iterator i( valueCompressor.begin() ); i != valueCompressor.end(); ++i )
		   {
		      newData[ newDataIndexItr++ ] = *i;
		   }

		   for( int i( 0 ); i < _size; ++i )
		   {
		      int left( 0 );
		      int right( valueCompressor.size() - 1 );
		      int mid( right / 2 );

		      while( left < right )
		      {
		         if( _data[ i ] < newData[ mid ] )
		         {
		            right = mid - 1;
		            mid = left + ( right - left ) / 2;
		         }
		         else
		         {
		            left = mid;
		            mid = right - ( right - left ) / 2;
		         }
		      }

		      unsigned char* indexPtr( _indexTable + _indexLenght * i / 8 );
		      unsigned char indexOffset( _indexLenght * i % 8 );

		      unsigned int index( mid << 32 - _indexLenght );

		      unsigned char part( index >> 24 + indexOffset );
		      *indexPtr++ |= part;

		      index <<= 8 - indexOffset;

		      for( unsigned char x( 0 ); x <= _indexLenght / 8; ++x )
		      {
		         part = index >> 24 - x * 8;

		         *indexPtr++ |= part;
		      }
		   }

		   delete _data;

		   _data = newData;
		   _size = valueCompressor.size();


		   /*unsigned int indexCount( ( _indexTableSize - 1 ) * 8 / _indexLenght );
		   unsigned int block( indexCount / 2 );

		   std::vector< unsigned int > prob;
		   prob.Resize( _size );

		   for( int i( 0 ); i < indexCount; ++i )
		   {
		      ++prob[ MakeIndex( i ) ];
		   }

		   std::sort( prob.begin(), prob.end() );

		   for( int i( 0 ); i < prob.size(); ++i )
		   {
		      TRACE_VAR( float( prob[ i ] ) / _size );
		      //TRACE_VAR( i );
		   }

		   /*while( block )
		   {
		      unsigned int indexMaxLen( 0 );

		      for( int b( 0 ); b < block; ++b )
		      {
		         unsigned int blockLen( indexCount / block );

		         unsigned int min( -1 );
		         unsigned int max( 0 );

		         for( int i( 0 ); i < blockLen; ++i )
		         {

		            unsigned int ndx( MakeIndex( blockLen * b + i ) );

		            if( ndx < min )
		            {
		               min = ndx;
		            }

		            if( ndx > max )
		            {
		               max = ndx;
		            }
		         }

		         for( int i( 31 ); i >= 0; --i )
		         {
		            unsigned int off( max - min );

		            if( ( off >> i ) & 1 )
		            {
		               if( i + 1 > indexMaxLen )
		               {
		                  indexMaxLen = i + 1;
		               }

		               break;
		            }
		         }
		      }

		      TRACE_VAR( block );
		      TRACE_VAR( indexMaxLen );

		      --block;
		   }

		   TRACE_VAR( (int)_indexLenght );*/

		   /*CFile f( "dump.txt" );
		   f.open( CFile::omWriteOnly | CFile::omTruncate );

		   for( int i( 0 ); i < indexTableSize(); ++i )
		   {
		      for( int j( 0 ); j < 8; ++j )
		      {
		         if( ( i || j ) && !( ( i * 8 + j ) % _indexLenght ) )
		         {
		            f.write( (const unsigned char*)"\n", 1 );
		         }

		         f.write( _indexTable[ i ] & 1 << 7 - j ? (const unsigned char*) "1" : (const unsigned char*)"0", 1 );
		      }
		   }

		   f.close();*/
		}

	template< class TValue >
      bool CPackedArray< TValue >::Read( CFile& f )
	   {
	      bool ret( false );

	      CPackedArray< TValue > nullArray;
	      *this = nullArray;

	      f.read( &_size, sizeof( _size ) );

	      SetSize( _size );

	      f.read( _data, sizeof( *_data ) * _size );

	      f.read( &_indexLenght, sizeof( _indexLenght ) );
	      f.read( &_indexTableSize, sizeof( _indexTableSize ) );

	      if( _indexTableSize )
	      {
	         _indexTable = new unsigned char[ _indexTableSize ];

	         f.read( _indexTable, _indexTableSize );
	      }

	      return ret;
	   }

	template< class TValue >
	   void CPackedArray< TValue >::Write( CFile& f )const
	   {
	      f.write( &_size, sizeof( _size ) );
	      f.write( _data, sizeof( *_data ) * _size );

	      f.write( &_indexLenght, sizeof( _indexLenght ) );
	      f.write( &_indexTableSize, sizeof( _indexTableSize ) );

	      if( IsCompressed() )
	      {
	         f.write( _indexTable, _indexTableSize );
	      }
	   }
}
