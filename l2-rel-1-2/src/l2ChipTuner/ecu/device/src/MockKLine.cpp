/*
 * MockKLine.cpp
 *
 *  Created on: 07.11.2008
 *      Author: mars
 */

#include "MockKLine.h"

namespace Mock
{
   CKLine::CKLine( unsigned char aNumber ) : l2max::CSerialLine( aNumber )
   {
      _klineState = klsReady;
   }

   CKLine::~CKLine()
   {
   }

   int CKLine::read( void* aData, unsigned int aSize )
   {
      int ret( 0 );

      if( _klineState == klsEcho )
      {
         ::memcpy( aData, _echo.data(), _echo.size() );

         ret = _echo.size();

         _klineState = klsReady;
      }

      return ret;
   }

   int CKLine::write( const void* aData, unsigned int aSize )
   {
      int ret( -1 );

      if( _klineState == klsReady )
      {
         _echo.SetData( aData, aSize );

         ret = aSize;

         _klineState = klsEcho;
      }

      return ret;
   }
}
