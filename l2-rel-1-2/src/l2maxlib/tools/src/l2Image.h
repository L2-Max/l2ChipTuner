#ifndef L2IMAGE_H_
#define L2IMAGE_H_

#include <l2ByteArray.h>

namespace l2max
{
	class CImage
	{
		CByteArray _data;

		int _width;
		int _height;

		unsigned char _bytesPerPixel;

	public:
		enum EFormat
		{
			/* fRGB24 */
			fRGB32,
			fUnformatted
		};

	private:
		EFormat _format;

		void internalAlloc();

	public:
	   CImage( EFormat format = fUnformatted );
		CImage( int width, int height, EFormat format = fRGB32 );
		CImage( unsigned char* data, int width, int height, EFormat format = fRGB32 );

		virtual ~CImage();

		unsigned char* data();
		const unsigned char* data()const;

		unsigned char bpp()const;

		void SetData( const unsigned char* data );
		bool SetData( const CByteArray& data );
		void SetWidth( unsigned int width );
		void SetHeight( unsigned int height );
		void SetFormat( EFormat format );

		unsigned int width()const;
		unsigned int height()const;

		void SetPixel( unsigned int at, unsigned int px );
		void SetPixel( unsigned int x, unsigned int y, unsigned int px );

		unsigned int pixel( unsigned int at )const;
		unsigned int pixel( unsigned int x, unsigned int y )const;

		void fill( unsigned int color );

		bool isNull()const;
	};
}

#endif /*L2BITMAP_H_*/
