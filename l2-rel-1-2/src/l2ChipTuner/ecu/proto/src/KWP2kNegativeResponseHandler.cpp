/*
 * KWP2kNegativeResponseHandler.cpp
 *
 *  Created on: 09.11.2008
 *      Author: L2-Max
 */

#include "KWP2kNegativeResponseHandler.h"

#include <l2Buffer.h>
#include <l2ByteArray.h>

namespace KWP2k
{
   CNegativeResponseHandler::CNegativeResponseHandler()
   {
      _negativeResponseHandlerResponseCode = nrhrcNoError;
   }

   CNegativeResponseHandler::~CNegativeResponseHandler()
   {
   }

   bool CNegativeResponseHandler::ProcessNegativeResponse( unsigned char aServiceId, l2max::CByteArray& aResp )
   {
      bool ret( false );

      unsigned char serviceId( 0 );

      l2max::CBuffer respStream( &aResp );

      respStream >> serviceId;

      if( serviceId == nrhsiNegativeResp )
      {
         respStream >> serviceId;

         if( serviceId == aServiceId )
         {
            respStream >> _negativeResponseHandlerResponseCode;

            ret = true;
         }
      }

      return ret;
   }
}
