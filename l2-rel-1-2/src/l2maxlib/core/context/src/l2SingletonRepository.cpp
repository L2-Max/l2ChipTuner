/*
 * l2SingletonRepository.cpp
 *
 *  Created on: 12.01.2009
 *      Author: mars
 */

#include "l2SingletonRepository.h"

#include <l2ISingleton.h>
#include <l2Exception.h>
#include <l2Trace.h>

#include <typeinfo>

namespace l2max
{
   CSingletonRepository::CSingletonRepository()
   {
   }
   
   CSingletonRepository::~CSingletonRepository()
   {
      dump();
      
      for( TSingletoneVector::reverse_iterator i( _singletoneVector.rbegin() ); i != _singletoneVector.rend(); ++i )
      {
         TraceRoutine << "Destructing singletone: " << typeid( **i ).name() << std::endl;
         
         delete *i;
      }
   }
   
   void CSingletonRepository::Register( ISingleton& aSingleton )
   {
      std::string objName( typeid( aSingleton ).name() );
      
      TSingletoneToIndexMap::iterator i( _singletoneToIndexMap.find( objName ) );
      
      if( i != _singletoneToIndexMap.end() )
      {
         throw CException( "Try to register singletone with same object" );
      }
      
      _singletoneToIndexMap[ objName ] = _singletoneVector.size();
      _singletoneVector.push_back( &aSingleton );
   }
   
   ISingleton* CSingletonRepository::singleton( const std::string& anObjName )
   {
      ISingleton* ret( 0 );
      
      TSingletoneToIndexMap::iterator i( _singletoneToIndexMap.find( anObjName ) );
      
      if( i != _singletoneToIndexMap.end() )
      {
         ret = _singletoneVector[ i->second ];
      }
      
      return ret;
   }
   
   void CSingletonRepository::dump()
   {
      TraceRoutine << _singletoneToIndexMap.size() << " registered singletones" << std::endl;
      
      for( TSingletoneToIndexMap::iterator i( _singletoneToIndexMap.begin() ); i != _singletoneToIndexMap.end(); ++i )
      {
         TraceRoutine << "Name: " << i->first << std::endl;
      }
   }
}
