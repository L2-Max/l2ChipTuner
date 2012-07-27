#ifndef __L2VARIANT_H__
#define __L2VARIANT_H__

#include "l2Bytearray.h"

#include <string>

namespace l2max
{
	class CVariant
	{
		CByteArray _data;

		bool _isFixed;

	public:
		enum EType
		{
		   tBool,
			tInteger,
			tDouble,
			tString,
			tBinary,
			tInvalid = -1
		};

		enum EPrecissionType
		{
		   ptInfinitely = -1
		};

		enum EWidthType
		{
		   wtInfinitely = -1
		};

	private:
		EType _type;

		int _width;
		int _precission;

		template< class TIntegral >
         TIntegral CastIntegralType()const;

	public:
		CVariant();
		CVariant( const CVariant& aVariant );

		CVariant( bool aValue );
		CVariant( int aValue );
		CVariant( unsigned int aValue );
		CVariant( double aValue, unsigned char aPrecission = ptInfinitely );
		CVariant( const std::string& aValue );
		CVariant( const char* aValue );
		CVariant( const void* aValue, unsigned int aSize );
		CVariant( const CByteArray& aValue );

		~CVariant();

		void SetBool( bool aValue );
		void SetInt( int aValue );
		void SetDouble( double aValue );
		void SetString( const std::string& aValue );
		void SetByteArray( const CByteArray& aValue );
		void SetData( const void* aValue, unsigned int aSize );
		void SetWidth( int aWidth );
		void SetPrecission( int aPrecission, bool aFixed = false );

		bool SetType( EType aType );

		bool toBool()const;
		int toInt()const;
		double toDouble()const;
		std::string toString()const;
		CByteArray toByteArray()const;

		CByteArray byteArray()const;

		EType type()const;

		bool operator == ( const CVariant& aVariant )const;
		bool operator != ( const CVariant& aVariant )const;

		bool isValid()const;
		bool isPrecissionFixed()const;

		int width()const;
		int precission()const;
	};
}

#endif
