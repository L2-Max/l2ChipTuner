#include "l2Image.h"

#include "l2Trace.h"

namespace l2max
{
   void CImage::internalAlloc()
   {
      TRACE_FUN( Frequently, "CImage::internalAlloc" );

      int reqSize( _width * _height * bpp() );

      if( reqSize != _data.size() )
      {
         _data.Resize( reqSize );
      }
   }

   CImage::CImage( EFormat format )
   {
      TRACE_FUN( Routine, "CImage::CImage" );

      _width = 0;
      _height = 0;
      _format = format;
   }

	CImage::CImage( int width, int height, EFormat format )
	{
	   TRACE_FUN( Routine, "CImage::CImage( int, int, EFormat )" );

	   _width = width;
	   _height = height;
	   _format = format;

	   internalAlloc();
	}

	CImage::CImage( unsigned char* data, int width, int height, EFormat format )
	{
		TRACE_FUN( Routine, "CImage::CImage( unsigned char*, int, int, EFormat )" );

	   _width = width;
	   _height = height;
	   _format = format;

	   internalAlloc();

	   memcpy( _data.data(), data, _data.size() );
	}

	CImage::~CImage()
	{
	   TRACE_FUN( Routine, "CImage::~CImage" );
	}

	unsigned char* CImage::data()
	{
	   return reinterpret_cast< unsigned char* >( _data.data() );
	}

	const unsigned char* CImage::data()const
	{
	   return reinterpret_cast< const unsigned char* >( _data.data() );
	}

	unsigned char CImage::bpp()const
	{
	   unsigned char bpp( 0 );

	   switch( _format )
	   {
	      case fRGB32:
	         bpp = 4;
	         break;
	   }

	   return bpp;
	}

	void CImage::SetData( const unsigned char* data )
	{
	   TRACE_FUN( Frequently, "CImage::SetData( const unsigned char* )" );

	   internalAlloc();

	   memcpy( _data.data(), data, _data.size() );
	}

	bool CImage::SetData( const CByteArray& data )
	{
	   TRACE_FUN( Frequently, "CImage::SetData( const CByteArray& )" );

	   bool ret( false );

	   if( data.size() == _width * _height * bpp() )
	   {
	      _data = data;

	      ret = true;
	   }

	   return ret;
	}

	void CImage::SetWidth( unsigned int width )
	{
	   _width = width;
	}

	void CImage::SetHeight( unsigned int height )
	{
	   _height = height;
	}

	void CImage::SetFormat( EFormat format )
	{
	   _format = format;
	}

	unsigned int CImage::width()const
	{
	   return _width;
	}

	unsigned int CImage::height()const
	{
	   return _height;
	}

	void CImage::SetPixel( unsigned int at, unsigned int px )
	{
		reinterpret_cast< unsigned int* >( _data.data() )[ at ] = px;
	}

	void CImage::SetPixel( unsigned int x, unsigned int y, unsigned int px )
	{
		SetPixel( y * width() + x, px );
	}

	unsigned int CImage::pixel( unsigned int at )const
	{
		return reinterpret_cast< const unsigned int* >( _data.data() )[ at ];
	}

	unsigned int CImage::pixel( unsigned int x, unsigned int y )const
	{
		return pixel( y * width() + x );
	}

	void CImage::fill( unsigned int color )
	{
		for( int i( 0 ); i < width() * height(); ++i )
		{
			reinterpret_cast< unsigned int* >( _data.data() )[ i ] = color;
		}
	}

	bool CImage::isNull()const
	{
		return !width() || !height() || !bpp() || !_data.size();
	}
}
