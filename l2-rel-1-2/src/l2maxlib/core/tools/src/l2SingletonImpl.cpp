/*
 * l2SingletonImpl.cpp
 *
 *  Created on: 13.01.2009
 *      Author: mars
 */

#include "l2SingletonImpl.h"

#include <l2Context.h>
#include <l2SingletonRepository.h>

namespace l2max
{
   CMutex CSingletonImpl::_mutex;
   
   CSingletonImpl::CSingletonImpl(){}
   CSingletonImpl::~CSingletonImpl(){}
   
   void CSingletonImpl::Register( ISingleton& aSingleton )
   {
      CContext::GetInstance()->singletonRepository().Register( aSingleton );
   }
   
   ISingleton* CSingletonImpl::singleton( const std::string& anObjName )
   {
      return CContext::GetInstance()->singletonRepository().singleton( anObjName );
   }
}
