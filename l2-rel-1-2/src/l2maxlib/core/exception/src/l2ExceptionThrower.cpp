/*
 * l2ExceptionThrower.cpp
 *
 *  Created on: 02.02.2009
 *      Author: mars
 */

#include "l2ExceptionThrower.h"

#include <l2Trace.h>
#include <l2Context.h>
#include <l2ExceptionRepository.h>

#include <sstream>

namespace l2max
{
   void CExceptionThrower::ThrowObjectState( const std::string& anObject,
                                             const std::string& aMethod,
                                             const std::string& aCondition,
                                             const std::string& aDescription,
                                             const std::string& aFile,
                                             int aLine )throw( const CExceptionObjectState& )
   {
      TRACE_FUN( Critical, "CExceptionThrower::ThrowObjectState" );
      
      std::ostringstream what;
      
      what << std::endl
           << "   " << anObject << "::" << aMethod << std::endl
           << "   Condition: " << aCondition << std::endl;
           
      if( aDescription.size() )
      {
         what << "   Description: " << aDescription << std::endl;
      }
      
      what << "   File: " << aFile << std::endl
           << "   Line: " << aLine << std::endl;
      
      TraceCritical << "Try to throw exception ... " << what.str();
      
      CContext::GetInstance()->exceptionRepository().Register( what.str() );
      
      throw CExceptionObjectState( what.str() );
   }
}
