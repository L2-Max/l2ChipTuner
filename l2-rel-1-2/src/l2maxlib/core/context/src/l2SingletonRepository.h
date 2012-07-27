/*
 * l2SingletonRepository.h
 *
 *  Created on: 12.01.2009
 *      Author: mars
 */

#ifndef L2SINGLETONREPOSITORY_H_
#define L2SINGLETONREPOSITORY_H_

#include <string>
#include <map>
#include <vector>

namespace l2max
{
   struct ISingleton;
   
   class CSingletonRepository
   {
      typedef std::vector< ISingleton* > TSingletoneVector;
      TSingletoneVector _singletoneVector;
      
      typedef std::map< std::string, unsigned int > TSingletoneToIndexMap;
      TSingletoneToIndexMap _singletoneToIndexMap;
      
      void dump();
      
   public:
      CSingletonRepository();
      virtual ~CSingletonRepository();
      
      void Register( ISingleton& aSingleton );
      ISingleton* singleton( const std::string& anObjName );
   };
}

#endif /* L2SINGLETONREPOSITORY_H_ */
