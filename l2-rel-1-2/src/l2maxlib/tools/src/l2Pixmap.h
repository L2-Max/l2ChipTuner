#ifndef L2PIXMAP_H_
#define L2PIXMAP_H_

#include <string>

#include "l2ByteArray.h"

struct png_struct_def;
struct png_info_struct;

namespace l2max
{
	struct SCallbackContext
	{
		CByteArray* _data;
		unsigned int _offset;
	};
		
	class CImage;
	class CFile;
	
	class CPixmap
	{
	   png_struct_def* _png_read_ptr;
	   png_info_struct* _png_read_info;
	   
	   png_struct_def* _png_write_ptr;
	   png_info_struct* _png_write_info;
	   
	   CByteArray _data;
	   mutable CImage* _cachedImage;
	   
	   SCallbackContext _readContext;
	   
	   CPixmap( const CPixmap& );
	   CPixmap& operator = ( const CPixmap& );
	   
	public:
		CPixmap();
	   CPixmap( const std::string& file );
		CPixmap( const unsigned char* data, unsigned int len );
		virtual ~CPixmap();
		
		bool load( const CByteArray& ba );
		bool load( const unsigned char* data, unsigned int len );
		bool load( const std::string& file );
		
		bool save( const std::string& file );
		
		void clear();
		
		const CImage& toImage()const;
		bool fromImage( const CImage& img );
		
		const CByteArray& data()const;
	};
}

#endif /*L2PIXMAP_H_*/
