#ifndef L2FILE_H_
#define L2FILE_H_

#include "l2AbstractIODevice.h"

#include <string>

namespace l2max
{
   class CByteArray;

	class CFile : public CAbstractIODevice
	{
		std::string _name;

		int _fd;

		CFile( const CFile& );
		CFile& operator = ( const CFile& );

	public:
		CFile( const std::string& name );
		virtual ~CFile();
		
      virtual unsigned int position()const;
      virtual unsigned int size()const;

      virtual int read( void* aData, unsigned int aSize );
      virtual int write( const void* aData, unsigned int aSize );

      int read( CByteArray& aByteArray );
      int write( const CByteArray& aByteArray );

      virtual bool open( TOpenMode aOpenMode );
      virtual bool close();
		virtual bool seek( int offset, ESeekMode seekMode );
	};
}

#endif /*L2FILE_H_*/
