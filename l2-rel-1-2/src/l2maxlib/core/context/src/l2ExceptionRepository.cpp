/*
 * l2ExceptionRepository.cpp
 *
 *  Created on: 02.10.2009
 *      Author: mars
 */

#include "l2ExceptionRepository.h"

#include <l2MutexLocker.h>
#include <l2Trace.h>

namespace l2max
{
   CExceptionRepository::CExceptionRepository(){}
   CExceptionRepository::~CExceptionRepository()
   {
      dump();
   }
   
   void CExceptionRepository::Register( const std::string& aWhat )
   {
      CMutexLocker locker( _mutex );
      
      _exceptionContainer.insert( aWhat );
   }
   
   void CExceptionRepository::dump()const
   {
      TraceCritical << " *** Exception report ***" << std::endl;
      TraceCritical << "Exceptions thrown: " << _exceptionContainer.size() << std::endl;
      
      for( TExceptionContainer::const_iterator I_exception( _exceptionContainer.begin() );
            I_exception != _exceptionContainer.end(); ++I_exception )
      {
         TraceCritical << "What: " << *I_exception << std::endl;
      }
   }
}
