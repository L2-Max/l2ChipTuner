/*
 * l2ExceptionRepository.h
 *
 *  Created on: 02.10.2009
 *      Author: mars
 */

#ifndef L2EXCEPTIONREPOSITORY_H_
#define L2EXCEPTIONREPOSITORY_H_

#include <l2Mutex.h>

#include <string>
#include <set>

namespace l2max
{
   class CExceptionRepository
   {
      CMutex _mutex;
      
      typedef std::set< std::string > TExceptionContainer;
      TExceptionContainer _exceptionContainer;
      
   public:
      CExceptionRepository();
      virtual ~CExceptionRepository();
      
      void Register( const std::string& aWhat );
      
      void dump()const;
   };
}

#endif /* L2EXCEPTIONREPOSITORY_H_ */
