#include "l2File.h"

#include <l2Trace.h>
#include <l2ByteArray.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

namespace l2max
{
	CFile::CFile( const std::string& name )
	{
	   TRACE_FUN( Routine, "CFile::CFile(const std::string&)" );

		_name = name;
		_fd = -1;
	}

	CFile::~CFile()
	{
	   TRACE_FUN( Routine, "CFile::~CFile" );

		close();
	}

   bool CFile::open( TOpenMode aOpenMode )
   {
      TRACE_FUN( Frequently, "CFile::open" );

      bool ret( false );

      unsigned int flags( 0
#ifdef __WIN32__
                          | O_BINARY
#endif
      );

      if( aOpenMode & omReadOnly )
      {
         flags |= O_RDONLY;
      }

      if( aOpenMode & omWriteOnly )
      {
         flags |= O_WRONLY | O_CREAT;
      }

      if( aOpenMode & omAppend )
      {
         flags |= O_APPEND | O_CREAT | O_WRONLY;
      }

      if( aOpenMode & omTruncate )
      {
         flags |= O_TRUNC | O_CREAT;
      }

      int createMode( S_IRUSR | S_IWUSR /*|
                S_IRGRP | S_IWGRP |
                S_IROTH | S_IWOTH */ );

      _fd = ::open( _name.c_str(), flags, createMode );

      if( _fd > 0)
      {
         ret = CAbstractIODevice::open( aOpenMode );
      }

      return ret;
   }

   int CFile::read( void* aData, unsigned int aSize )
   {
      TRACE_FUN( Frequently, "CFile::read( unsigned char*, unsigned int )" );

      unsigned int ret( 0 );

      if( isOpen() )
      {
         ret = ::read( _fd, aData, aSize );
      }

      return ret;
   }

   int CFile::read( CByteArray& aByteArray )
   {
      return read( aByteArray.data(), aByteArray.size() );
   }

   int CFile::write( const void* aData, unsigned int aSize )
   {
      TRACE_FUN( Frequently, "CFile::write( unsigned char*, unsigned int )" );

      unsigned int ret( 0 );

      if( isOpen() )
      {
         ret = ::write( _fd, aData, aSize );
      }

      return ret;
   }

   int CFile::write( const CByteArray& aByteArray )
   {
      return write( aByteArray.data(), aByteArray.size() );
   }

   bool CFile::close()
   {
      TRACE_FUN( Frequently, "CFile::close" );

      bool ret( false );

      if( _fd > 0 )
      {
         _fd = ::close( _fd );

         if( !_fd )
         {
            _fd = -1;

            ret = CAbstractIODevice::close();
         }
      }

      return ret;
   }

   bool CFile::seek( int offset, ESeekMode seekMode )
	{
	   TRACE_FUN( Frequently, "CFile::seek" );

		bool ret( false );

		if( isOpen() )
		{
			unsigned int whence( 0 );

			if( seekMode == smFromBegining )
			{
				whence = SEEK_SET;
			}
			else if( seekMode == smFromCurrent )
			{
				whence = SEEK_CUR;
			}
			else if( seekMode == smFromEnd )
			{
				whence = SEEK_END;
			}

			ret = ::lseek( _fd, offset, whence ) != -1;
		}

		return ret;
	}

	unsigned int CFile::size()const
	{
	   TRACE_FUN( Frequently, "CFile::size" );

		unsigned int ret( 0 );

		if( isOpen() )
		{
			struct stat st;

			if( !::fstat( _fd, &st ) )
			{
				ret = st.st_size;
			}
		}

		return ret;
	}

   unsigned int CFile::position()const
   {
      TRACE_FUN( Frequently, "CFile::position" );

      unsigned int ret( 0 );

      if( isOpen() )
      {
         ret = ::lseek( _fd, 0, SEEK_CUR );
      }

      return ret;
   }
}
