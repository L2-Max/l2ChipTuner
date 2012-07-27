/*
 * l2ExceptionThrower.h
 *
 *  Created on: 02.02.2009
 *      Author: mars
 */

#ifndef L2EXCEPTIONTHROWER_H_
#define L2EXCEPTIONTHROWER_H_

#include "l2ExceptionObjectState.h"
#include "l2ExceptionDefs.h"

namespace l2max
{
   class CExceptionThrower
   {
      CExceptionThrower();
      virtual ~CExceptionThrower();
      
   public:
      static void ThrowObjectState( const std::string& anObject,
                                    const std::string& aMethod,
                                    const std::string& aCondition,
                                    const std::string& aDescription,
                                    const std::string& aFile,
                                    int aLine )throw( const CExceptionObjectState& );
   };
}

#endif /* L2EXCEPTIONTHROWER_H_ */
