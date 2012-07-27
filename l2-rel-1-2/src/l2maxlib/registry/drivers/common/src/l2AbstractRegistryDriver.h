/*
 * l2AbstractRegistryDriver.h
 *
 *  Created on: 04.09.2009
 *      Author: L2-Max
 */

#ifndef L2ABSTRACTREGISTRYDRIVER_H_
#define L2ABSTRACTREGISTRYDRIVER_H_

#include "l2ParserErrorRepository.h"
#include "l2CategoryParserDriver.h"
#include "l2ParserDriverSynchronization.h"

#include <l2RegistryDriverInterface.h>
#include <l2Thread.h>
#include <l2RefObject.h>

#include <fstream>

namespace l2max
{
   namespace Registry
   {
      namespace Driver
      {
         class CAbstractRegistryDriver : public IInterface,
                                         public CRefObject,
                                         protected CThread
         {
            CParserDriverSynchronization _synchronization;
            CParserErrorRepository _errorRepository;

            std::string _file;
            std::ifstream _fstream;

            HCategory _category;
            
            virtual void parse() = 0;
            virtual unsigned int run();

         protected:
            bool _isModified;
            
            void WaitForDriverStop()const;

            bool isRun()const;
            bool isModified()const;

            const CParserDriverSynchronization& synchronization()const;
            CParserErrorRepository& errorRepository();

            const std::string& file()const;
            std::ifstream& stream();

            HCategory category()const;

         public:
            CAbstractRegistryDriver();
            virtual ~CAbstractRegistryDriver();

            virtual bool hasError()const;
            virtual std::string errorString()const;

            virtual bool Open( const std::string& aFile );
            virtual bool Read( const std::string& aKey, CVariant& aValue )const;
            virtual bool Write( const std::string& aKey, const CVariant& aValue );
            virtual void Clear();

            virtual const std::string& parsedFileName()const;

            virtual TKeyListIterator CategoryList( const std::string& aKey, TKeyList& aList )const;
            virtual TKeyListIterator VariableList( const std::string& aKey, TKeyList& aList )const;
         };
      }
   }
}

#endif /* L2ABSTRACTREGISTRYDRIVER_H_ */
