/*
 * l2ParserDriverSynchronization.h
 *
 *  Created on: 05.09.2009
 *      Author: L2-Max
 */

#ifndef L2PARSERDRIVERSYNCHRONIZATION_H_
#define L2PARSERDRIVERSYNCHRONIZATION_H_

#include <l2Mutex.h>
#include <l2WaitCondition.h>

namespace l2max
{
   namespace Registry
   {
      namespace Driver
      {
         class CParserDriverSynchronization
         {
            mutable CMutex _mutex;
            mutable CWaitCondition _dataReadCondition;
            mutable CWaitCondition _parserStopCondition;
            mutable CWaitCondition _readContinueCondition;
            mutable CWaitCondition _terminateCondition;

         public:
            CParserDriverSynchronization();
            virtual ~CParserDriverSynchronization();

            void parserStopped()const;
            void dataRead()const;
            void continueRead()const;
            void terminate()const;
            void ResetTerminate()const;
            
            bool isRun()const;
            bool isTerminate()const;

            bool WaitForData()const;
            bool WaitForContinue()const;

            CMutex& mutex()const;
         };
      }
   }
}

#endif /* L2PARSERDRIVERSYNCHRONIZATION_H_ */
