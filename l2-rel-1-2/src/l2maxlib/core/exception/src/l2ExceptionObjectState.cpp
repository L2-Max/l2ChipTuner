/*
 * l2ExceptionObjectState.cpp
 *
 *  Created on: 02.02.2009
 *      Author: mars
 */

#include "l2ExceptionObjectState.h"

namespace l2max
{
   CExceptionObjectState::CExceptionObjectState( const std::string& aWhat ) throw() :
      CException( "CExceptionObjectState: " + aWhat ){}
   
   CExceptionObjectState::~CExceptionObjectState()throw(){}
   
   const char* CExceptionObjectState::what()const throw()
   {
      return CException::what();
   }
}
