/*
 * l2ExceptionObjectState.h
 *
 *  Created on: 02.02.2009
 *      Author: mars
 */

#ifndef L2EXCEPTIONOBJECTSTATE_H_
#define L2EXCEPTIONOBJECTSTATE_H_

#include "l2Exception.h"

namespace l2max
{
   class CExceptionObjectState : public CException
   {
   public:
      CExceptionObjectState( const std::string& aWhat )throw();
      virtual ~CExceptionObjectState()throw();
      
      virtual const char* what()const throw();
   };
}

#endif /* L2EXCEPTIONOBJECTSTATE_H_ */
