/*
 * l2RefObject.cpp
 *
 *  Created on: 05.01.2009
 *      Author: mars
 */

#include "l2RefObject.h"

#include <l2Trace.h>
#include <l2Context.h>
#include <l2RefObjectRepository.h>

namespace l2max
{
   CRefObject::CRefObject()
   {
      _count = 0;

      CContext::GetInstance()->refObjectRepository().Register( *this );
   }

   CRefObject::~CRefObject()
   {
      CContext::GetInstance()->refObjectRepository().UnRegister( *this );
   }

   unsigned int CRefObject::AddRef()
   {
      return ++_count;
   }

   unsigned int CRefObject::Release()
   {
      if( _count )
      {
         --_count;
      }
      
      unsigned int ret( _count );

      if( !_count )
      {
         delete this;
      }

      return ret;
   }

   unsigned int CRefObject::refCount()const
   {
      return _count;
   }
}
