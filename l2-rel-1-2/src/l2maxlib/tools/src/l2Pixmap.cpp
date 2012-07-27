#include "l2Pixmap.h"

#include <png.h>

#include "l2Trace.h"
#include "l2Image.h"
#include "l2File.h"

namespace l2max
{
	void PNGAPI png_rw_callback( png_structp png_s, png_bytep data, png_size_t size )
	{
	   if( png_s->read_data_fn )
	   {
		   SCallbackContext* _context( reinterpret_cast< SCallbackContext* >( png_s->io_ptr ) );

		   memcpy( data, reinterpret_cast< const unsigned char* >( _context->_data->data() ) + _context->_offset, size );

		   _context->_offset += size;
	   }
	   else
	   {
	   	SCallbackContext* _context( reinterpret_cast< SCallbackContext* >( png_s->io_ptr ) );
	   	_context->_data->Append( data, size );
	   }
	}

	CPixmap::CPixmap()
	{
	   TRACE_FUN( Routine, "CPixmap::CPixmap" );

		_png_read_ptr = 0;
		_png_read_info = 0;

		_png_write_ptr = 0;
		_png_write_info = 0;

		_cachedImage = 0;
	}

	CPixmap::CPixmap( const std::string& file )
	{
	   TRACE_FUN( Routine, "CPixmap::CPixmap( const std::string& )" );

		_png_read_ptr = 0;
		_png_read_info = 0;

		_png_write_ptr = 0;
		_png_write_info = 0;

		_cachedImage = 0;

	   load( file );
	}

	CPixmap::CPixmap( const unsigned char* data, unsigned int len )
	{
		TRACE_FUN( Routine, "CPixmap::CPixmap( const unsigned char*, unsigned int )" );

		_png_read_ptr = 0;
		_png_read_info = 0;

		_png_write_ptr = 0;
		_png_write_info = 0;

		_cachedImage = 0;

		load( data, len );
	}

	CPixmap::~CPixmap()
	{
	   TRACE_FUN( Routine, "CPixmap::~CPixmap" );

	   clear();

	   delete _cachedImage;
	}

	bool CPixmap::load( const CByteArray& ba )
	{
	   TRACE_FUN( Frequently, "CPixmap::load( const CByteArray& )" );

		bool ret( false );

		clear();

		_png_read_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING, 0, 0, 0 );
		if( _png_read_ptr )
		{
		   _png_read_info = png_create_info_struct( _png_read_ptr );

		   if( _png_read_info )
		   {
		   	_readContext._data = &_data;
		   	_readContext._offset = 0;

		   	png_set_read_fn( _png_read_ptr, &_readContext, png_rw_callback );

		   	_data = ba;

		   	ret = true;
			}
		}

		return ret;
	}

	bool CPixmap::load( const unsigned char* data, unsigned int len )
	{
	   TRACE_FUN( Frequently, "CPixmap::load( const unsigned char*, unsigned int )" );

		bool ret( false );

		ret = load( CByteArray( data, len ) );

		return ret;
	}

	bool CPixmap::load( const std::string& file )
	{
	   TRACE_FUN( Frequently, "CPixmap::load( const std::string& )" );

	   bool ret( false );

	   CFile f( file );
	   if( f.open( CFile::omReadOnly ) )
	   {
	      CByteArray data;
	      data.Resize( f.size() );

	      f.read( data );

	      ret = load( data );

	      f.close();
	   }

	   return ret;
	}

	bool CPixmap::save( const std::string& file )
	{
		TRACE_FUN( Frequently, "CPixmap::save" );

		bool ret( false );

		CFile f( file );
		if( f.open( CFile::omWriteOnly ) )
		{
			if( f.write( _data ) == _data.size() )
			{
				ret = true;
			}

			f.close();
		}

		return ret;
	}

	void CPixmap::clear()
	{
		TRACE_FUN( Frequently, "CPixmap::clear" );

		if( _png_read_ptr && _png_read_info )
		{
			png_destroy_read_struct( &_png_read_ptr, &_png_read_info, png_infopp_NULL );

			_png_read_ptr = 0;
			_png_read_info = 0;
		}

		if( _png_write_ptr && _png_write_info )
		{
			png_destroy_write_struct( &_png_write_ptr, &_png_write_info );

			_png_write_ptr = 0;
			_png_write_info = 0;
		}

		_data.Clear();
	}

	const CImage& CPixmap::toImage()const
	{
	   TRACE_FUN( Frequently, "CPixmap::toImage" );

	   if( !_png_read_info->row_pointers )
	   {
	   	png_read_png( _png_read_ptr, _png_read_info, 0, png_voidp_NULL );
	   }

	   if( _cachedImage )
	   {
	   	delete _cachedImage;
	   }

	   _cachedImage = new CImage( _png_read_info->width, _png_read_info->height, CImage::fRGB32 );

	   for( int row( 0 ); row < _png_read_info->height; ++row )
	   {
	      for( int col( 0 ); col < _png_read_info->width; ++col )
	      {
	      	unsigned int colIndex( col * 3 );

	      	_cachedImage->SetPixel( col, row, _png_read_info->row_pointers[ row ][ colIndex ] << 16 |
	      	              							 _png_read_info->row_pointers[ row ][ colIndex + 1 ] << 8 |
	      	              							 _png_read_info->row_pointers[ row ][ colIndex + 2 ] );
	      }
	   }

		return *_cachedImage;
	}

	bool CPixmap::fromImage( const CImage& img )
	{
	   TRACE_FUN( Frequently, "CPixmap::fromImage" );

	   bool ret( false );

	   _png_write_ptr = png_create_write_struct( PNG_LIBPNG_VER_STRING, 0, 0, 0 );
	   if( _png_write_ptr )
	   {
	   	_png_write_info = png_create_info_struct( _png_write_ptr );
	   	if( _png_write_info )
	   	{
	   		CByteArray ba;

	   		SCallbackContext context;
	   		context._data = &ba;
	   		context._offset = 0;

	   		png_set_write_fn( _png_write_ptr, &context, png_rw_callback, 0 );

	   	   png_set_IHDR( _png_write_ptr, _png_write_info, img.width(), img.height(), 8, PNG_COLOR_TYPE_RGB,
	   	      			  PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE );

	   	   png_write_info( _png_write_ptr, _png_write_info );

	   	   png_bytep dstImgRow( new png_byte[ _png_write_info->width * 3 ] );

	   	   for( int row( 0 ); row < _png_write_info->height; ++row )
	   	   {
	   	   	png_bytep srcImgRow( const_cast< png_bytep >( img.data() ) + row * _png_write_info->width * 4 );

	   	   	for( int col( 0 ); col < _png_write_info->width; ++col )
	   	   	{
	   	   		dstImgRow[ col * 3 ] = srcImgRow[ col * 4 + 2];
	   	   		dstImgRow[ col * 3 + 1 ] = srcImgRow[ col * 4 + 1 ];
	   	   		dstImgRow[ col * 3 + 2 ] = srcImgRow[ col * 4 ];
	   	   	}

	   	   	png_write_rows( _png_write_ptr, &dstImgRow, 1 );
	   	   }

	   	   delete dstImgRow;

	   	   png_write_end( _png_write_ptr, _png_write_info );

	   	   ret = load( ba );
	   	}
	   }

	   return ret;
	}

	const CByteArray& CPixmap::data()const
	{
	   TRACE_FUN( Frequently, "CPixmap::data" );

	   return _data;
	}
}
