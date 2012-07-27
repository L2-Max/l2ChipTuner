/*
 * ECURequest.cpp
 *
 *  Created on: 25.01.2009
 *      Author: mars
 */

#include "ECURequest.h"

namespace ECU
{
   CRequest::CRequest()
   {
      _id = iUnknown;
   }
   
   CRequest::CRequest( EId anId )
   {
      _id = anId;
   }
   
   CRequest::~CRequest()
   {
   }
   
   CRequest::EId CRequest::id()const
   {
      return _id;
   }
}
