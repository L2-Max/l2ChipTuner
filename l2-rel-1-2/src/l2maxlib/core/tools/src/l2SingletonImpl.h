/*
 * l2SingletonImpl.h
 *
 *  Created on: 13.01.2009
 *      Author: mars
 */

#ifndef L2SINGLETONIMPL_H_
#define L2SINGLETONIMPL_H_

#include <l2Mutex.h>

#include <string>

namespace l2max
{
   struct ISingleton;
   
   class CSingletonImpl
   {
   protected:
      static CMutex _mutex;
      
      static void Register( ISingleton& aSingleton );
      static ISingleton* singleton( const std::string& anObjName );
      
      CSingletonImpl();
      virtual ~CSingletonImpl();
   };
}

#endif /* L2SINGLETONIMPL_H_ */
