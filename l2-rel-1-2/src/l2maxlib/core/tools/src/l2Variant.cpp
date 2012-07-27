#include "l2Variant.h"

#include <l2ExceptionThrower.h>
#include <l2Trace.h>

#include <math.h>

namespace l2max
{
   CVariant::CVariant()
   {
      _type = tInvalid;

      _width = wtInfinitely;
      _precission = ptInfinitely;

      _isFixed = false;
   }

   CVariant::CVariant( const CVariant& aVariant )
   {
      *this = aVariant;
   }

   CVariant::CVariant( bool aValue )
   {
      _width = wtInfinitely;
      _precission = ptInfinitely;

      _isFixed = false;

       SetBool( aValue );
   }

   CVariant::CVariant( int aValue )
   {
      _width = wtInfinitely;
      _precission = ptInfinitely;

      _isFixed = false;

       SetInt( aValue );
   }

   CVariant::CVariant( unsigned int aValue )
   {
      _width = wtInfinitely;
      _precission = ptInfinitely;

      _isFixed = false;

       SetInt( aValue );
   }

   CVariant::CVariant( double aValue, unsigned char aPrecission )
   {
      _width = wtInfinitely;
      _precission = aPrecission;

      _isFixed = false;

       SetDouble( aValue );
   }

   CVariant::CVariant( const std::string& aValue )
   {
      _width = wtInfinitely;
      _precission = ptInfinitely;

      _isFixed = false;

       SetString( aValue );
   }

   CVariant::CVariant( const char* aValue )
   {
      _width = wtInfinitely;
      _precission = ptInfinitely;

      _isFixed = false;

       SetString( aValue );
   }

   CVariant::CVariant( const void* aValue, unsigned int aSize )
   {
      _width = wtInfinitely;
      _precission = ptInfinitely;

      _isFixed = false;

       SetData( aValue, aSize );
   }

   CVariant::CVariant( const CByteArray& aValue )
   {
      _width = wtInfinitely;
      _precission = ptInfinitely;

      _isFixed = false;

      SetByteArray( aValue );
   }

   CVariant::~CVariant(){}

   void CVariant::SetBool( bool aValue )
   {
      _type = tBool;
      _data.SetData( &aValue, sizeof( aValue ) );
   }

   void CVariant::SetInt( int aValue )
   {
      _type = tInteger;
      _data.SetData( &aValue, sizeof( aValue ) );
   }

   void CVariant::SetDouble( double aValue )
   {
      _type = tDouble;
      _data.SetData( &aValue, sizeof( aValue ) );
   }

   void CVariant::SetString( const std::string& aValue )
   {
      _type = tString;
      _data.SetData( aValue.c_str(), aValue.size() + 1 );
   }

   void CVariant::SetByteArray( const CByteArray& aValue )
   {
      _type = tBinary;
      _data = aValue;
   }

   void CVariant::SetData( const void* aValue, unsigned int aSize )
   {
      _type = tBinary;
      _data.SetData( aValue, aSize );
   }

   template< class TIntegral >
      TIntegral CVariant::CastIntegralType()const
      {
         TIntegral ret;

         if( _type == tBool )
         {
            ret = *reinterpret_cast< const bool* >( _data.data() );
         }
         else if( _type == tInteger )
         {
            ret = *reinterpret_cast< const int* >( _data.data() );
         }
         else if( _type == tDouble )
         {
            ret = static_cast< TIntegral >( *reinterpret_cast< const double* >( _data.data() ) );
         }

         return ret;
      }

   bool CVariant::toBool()const
   {
      TRACE_FUN( Frequently, "CVariant::toBool" );

      CHECK_OBJECT_STATE( isValid(), "Invalid type" );

      bool res( false );

      if( _type == tString )
      {
         int ret( sscanf( reinterpret_cast< const char* >( _data.data() ), "%d", &res ) );

         CHECK_OBJECT_STATE( ret && ret != EOF, "Cast from string to integer type" );
      }
      else if( _type == tBinary )
      {
         CHECK_OBJECT_STATE( _data.size() == sizeof( res ), "Cast from binary to integer type" );

         memcpy( &res, _data.data(), _data.size() );
      }
      else if( _type == tDouble )
      {
         res = rint( *reinterpret_cast< const double* >( _data.data() ) );
      }
      else if( _type != tInvalid )
      {
         res = CastIntegralType< bool >();
      }

      return res;
   }

   int CVariant::toInt()const
   {
      TRACE_FUN( Frequently, "CVariant::toInt" );

      CHECK_OBJECT_STATE( isValid(), "Invalid type" );

      int res( -1 );

      if( _type == tString )
      {
         int ret( sscanf( reinterpret_cast< const char* >( _data.data() ), "%d", &res ) );

         CHECK_OBJECT_STATE( ret && ret != EOF, "Cast from string to integer type" );
      }
      else if( _type == tBinary )
      {
         CHECK_OBJECT_STATE( _data.size() == sizeof( res ), "Cast from binary to integer type" );

         memcpy( &res, _data.data(), _data.size() );
      }
      else if( _type == tDouble )
      {
         res = static_cast< int >( rint( *reinterpret_cast< const double* >( _data.data() ) ) );
      }
      else if( _type != tInvalid )
      {
         res = CastIntegralType< int >();
      }

      return res;
   }

   double CVariant::toDouble()const
   {
      TRACE_FUN( Frequently, "CVariant::toDouble" );

      CHECK_OBJECT_STATE( isValid(), "Invalid type" );

      double res( .0 );

      if( _type == tString )
      {
         int ret( sscanf( reinterpret_cast< const char* >( _data.data() ), "%lf", &res ) );

         CHECK_OBJECT_STATE( ret && ret != EOF, "Cast from string to double type" );
      }
      else if( _type == tBinary )
      {
         CHECK_OBJECT_STATE( _data.size() == sizeof( res ), "Cast from binary to double type" );

         memcpy( &res, _data.data(), _data.size() );
      }
      else if( _type != tInvalid )
      {
         res = CastIntegralType< double >();
      }

      return res;
   }

   std::string CVariant::toString()const
   {
      TRACE_FUN( Frequently, "CVariant::toString" );

      CHECK_OBJECT_STATE( isValid(), "Invalid type" );

      std::string res;

      if( _type == tBool )
      {
         if( *reinterpret_cast< const bool* >( _data.data() ) )
         {
            res = "true";
         }
         else
         {
            res = "false";
         }
      }
      else if( _type == tInteger )
      {
         std::string format( "%" );

         if( _width != wtInfinitely )
         {
            char buf[ 32 ] = { 0 };

            sprintf( buf, "0%d", _width );

            format += buf;
         }

         if( _precission != ptInfinitely )
         {
            char buf[ 32 ] = { 0 };

            sprintf( buf, ".%d", _precission );

            format += buf;
         }

         format += "d";
         
         char buf[ 1024 ] = { 0 };

         sprintf( buf, format.c_str(), *reinterpret_cast< const int* >( _data.data() ) );

         res = buf;
      }
      else if( _type == tDouble )
      {
         std::string format( "%" );

         if( _width != wtInfinitely )
         {
            char buf[ 32 ] = { 0 };

            sprintf( buf, "%d", _width );

            format += buf;
         }

         if( _precission != ptInfinitely )
         {
            char buf[ 32 ] = { 0 };

            sprintf( buf, ".%d", _precission );

            format += buf;
         }

         format += "lf";
         
         char buf[ 1024 ] = { 0 };

         sprintf( buf, format.c_str(), *reinterpret_cast< const double* >( _data.data() ) );

         res = buf;

         if( res.size() )
         {
            if( _precission == ptInfinitely || !_isFixed )
            {
               for( std::string::iterator i( res.end() ); i != res.begin(); )
               {
                  --i;

                  if( *i == '0' )
                  {
                     i = res.erase( i );
                  }
                  else
                  {
                     break;
                  }
               }
            }
         }
      }
      else if( _type == tString )
      {
         res = reinterpret_cast< const char* >( _data.data() );
      }
      else if( _type == tBinary )
      {
         char* buf( new char[ _data.size() * 2 + 1 ] );

         for( int i( 0 ); i < _data.size(); ++i )
         {
            sprintf( buf + i * 2, "%2.2X", _data[ i ] );
         }

         res = buf;

         delete[] buf;
      }

      return res;
   }

   CByteArray CVariant::toByteArray()const
   {
      TRACE_FUN( Frequently, "CVariant::toByteArray" );

      CHECK_OBJECT_STATE( isValid(), "Invalid type" );

      CByteArray res;

      if( _type == tString )
      {
         int dataSize( _data.size() - 1 );

         if( dataSize < 0 )
         {
            dataSize = 0;
         }
         else
         {
            if( dataSize % 2 )
            {
               ++dataSize;
            }

            dataSize /= 2;
         }

         res.Resize( dataSize );

         for( int i( 0 ); i < dataSize; ++i )
         {
            unsigned int binData( 0 );

            sscanf( reinterpret_cast< const char* >( _data.data() ) + i * 2, "%2X", &binData );

            res[ i ] = binData;
         }
      }
      else
      {
         res = _data;
      }

      return res;
   }

   bool CVariant::SetType(EType t)
   {
      TRACE_FUN( Frequently, "CVariant::SetType" );

      try
      {
         switch( t )
         {
            case tBool:
               SetBool( toBool() );
               break;

            case tInteger:
               SetInt( toInt() );
               break;

            case tDouble:
               SetDouble( toDouble() );
               break;

            case tString:
               SetString( toString() );
               break;

            case tBinary:
               SetByteArray( toByteArray() );
               break;
         }
      }
      catch( const CException& e )
      {
         return false;
      }

      return true;
   }

   CByteArray CVariant::byteArray()const
   {
      return _data;
   }

   CVariant::EType CVariant::type()const
   {
      return _type;
   }

   bool CVariant::operator == ( const CVariant& aVariant )const
   {
      bool ret( false );

      if( type() == aVariant.type() )
      {
         if( _data == aVariant._data )
         {
            ret = true;
         }
      }

      return ret;
   }

   bool CVariant::operator != ( const CVariant& aVariant )const
   {
      return !operator == ( aVariant );
   }

   bool CVariant::isValid()const
   {
      return type() != tInvalid;
   }

   bool CVariant::isPrecissionFixed()const
   {
      return _isFixed;
   }

   void CVariant::SetWidth( int aWidth )
   {
      _width = aWidth;
   }

   void CVariant::SetPrecission( int aPrecission, bool aFixed )
   {
      _precission = aPrecission;
      _isFixed = aFixed;
   }

   int CVariant::width()const
   {
      return _width;
   }

   int CVariant::precission()const
   {
      return _precission;
   }
}
