#ifndef __L2MEMMGR_H__
#define __L2MEMMGR_H__

#include <stdio.h>
#include <memory.h>

namespace l2max
{
	class CByteArray
	{
		unsigned char* _data;
		unsigned int _size;
		
		unsigned int _allocatedSize;

	public:
		CByteArray();
		CByteArray( const CByteArray& aValue );
		CByteArray( const void* aValue, unsigned int aSize );
		CByteArray( const char* aValue );
		~CByteArray();

		CByteArray& operator = ( const CByteArray& aValue );

		unsigned char operator[]( unsigned int anIndex )const;
		unsigned char& operator[]( unsigned int anIndex );

		void Append( const CByteArray& aValue, bool atEnd = true );
		void Append( const void* aValue, unsigned int aSize, bool atEnd = true );
		void SetData( const void* aValue, unsigned int aSize, bool anAlloc = true );
		void Fill( unsigned char aPattern );
		void Clear();
		void Resize( unsigned int aSize );
		void Reverse();
		void Push_back( unsigned char aByte );
		void Pop_back();

		void* data();
		const void* data()const;

		unsigned int size()const;

		unsigned char& back();
		unsigned char back()const;

		bool operator == ( const CByteArray& aValue )const;
		bool operator != ( const CByteArray& aValue )const;
	};
}

#endif
