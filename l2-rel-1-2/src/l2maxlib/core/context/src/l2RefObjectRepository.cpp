/*
 * l2RefObjectRepository.cpp
 *
 *  Created on: 25.02.2009
 *      Author: L2-Max
 */

#include "l2RefObjectRepository.h"

#include <l2ExceptionThrower.h>
#include <l2Trace.h>
#include <l2IRefObject.h>
#include <l2MutexLocker.h>

#include <typeinfo>

namespace l2max
{
   CRefObjectRepository::CRefObjectRepository()
   {
      _regCount = 0;
      _unRegCount = 0;
   }

   CRefObjectRepository::~CRefObjectRepository()
   {
      dump();
   }

   void CRefObjectRepository::Register( const IRefObject& anObject )
   {
      CMutexLocker locker( _mutex );

      CHECK_OBJECT_STATE( _refObjectSet.find( &anObject ) == _refObjectSet.end(), "Object already exist in repository" );

      _refObjectSet.insert( &anObject );

      ++_regCount;
   }

   void CRefObjectRepository::UnRegister( const IRefObject& anObject )
   {
      CMutexLocker locker( _mutex );

      CHECK_OBJECT_STATE( _refObjectSet.find( &anObject ) != _refObjectSet.end(), "Object not exist in repository" );

      _refObjectSet.erase( &anObject );

      ++_unRegCount;
   }

   void CRefObjectRepository::dump()const
   {
      TraceCritical << " *** IRefObject report ***" << std::endl;

      TraceCritical << "Registration requests: " << _regCount << std::endl;
      TraceCritical << "Unregistration requests: " << _unRegCount << std::endl;
      TraceCritical << _refObjectSet.size() << " registered IRefObjects" << std::endl;

      for( TRefObjectSet::const_iterator i( _refObjectSet.begin() ); i != _refObjectSet.end(); ++i )
      {
         TraceCritical << "Name: " << typeid( **i ).name() << ", refCount: " << ( *i )->refCount() << std::endl;
      }
   }
}
