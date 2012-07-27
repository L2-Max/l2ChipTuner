#include "l2String.h"

#include <l2Trace.h>

namespace l2max
{
   CString::CString() : _data( "" ){}
   
   CString::CString( const char* aString ) : _data( aString ){}
	CString::CString( const std::string& aString ) : _data( aString.c_str() ){}
	
	CString::~CString(){}
	
	CString::operator std::string()const
	{
	   return toStdString();
	}
	
   CString CString::operator + ( const CString& aString )const
   {
      return toStdString() + aString.toStdString();
   }
   
   void CString::operator += ( const CString& aString )
   {
      *this = *this + aString;
   }
	
	unsigned int CString::size()const
	{
	   unsigned int ret( _data.size() );
	   
	   if( ret )
	   {
	      --ret;
	   }
	   
	   return ret;
	}
	
   CString CString::List2String( const TStringList& aList, const CString& aSeparator )
   {
      TRACE_FUN( Frequently, "CString::List2String" );
      
      CString ret;
      
      for( TStringList::const_iterator i( aList.begin() ); i != aList.end(); ++i )
      {
         if( ret.size() )
         {
            ret += aSeparator;
         }

         ret += *i;
      }
      
      return ret;
   }
   
   TStringList CString::String2List( const CString& aString, const CString& aSeparator )
   {
      TRACE_FUN( Frequently, "CString::String2List" );
      
      TStringList ret;
      
      std::string::size_type firstSynonymPos( 0 );
      std::string::size_type lastSynonymPos( std::string::npos );
      
      std::string theString( aString.toStdString() );
      
      do
      {
         lastSynonymPos = theString.find( aSeparator.toStdString(), firstSynonymPos );

         if( lastSynonymPos != std::string::npos || theString.size() )
         {
            ret.push_back( theString.substr( firstSynonymPos, lastSynonymPos - firstSynonymPos ) );

            firstSynonymPos = lastSynonymPos + aSeparator.size();
         }
      }
      while( lastSynonymPos != std::string::npos );
      
      return ret;
   }
   
   std::string CString::toStdString()const
   {
      return reinterpret_cast< const char* >( _data.data() );
   }
}
