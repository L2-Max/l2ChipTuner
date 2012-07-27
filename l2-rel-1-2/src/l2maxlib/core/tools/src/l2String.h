#ifndef L2STRING_H_
#define L2STRING_H_

#include "l2ByteArray.h"

#include <list>
#include <string>

namespace l2max
{
   class CString;

   typedef std::list< CString > TStringList;

	class CString
	{
	   CByteArray _data;

	public:
	   CString();
	   CString( const char* aString );
		CString( const std::string& aString );
		virtual ~CString();

		static CString List2String( const TStringList& aList, const CString& aSeparator );
		static TStringList String2List( const CString& aString, const CString& aSeparator );

		operator std::string()const;

		CString operator + ( const CString& aString )const;
		void operator += ( const CString& aString );

		unsigned int size()const;

		std::string toStdString()const;
	};
}

#endif /*L2STRING_H_*/
