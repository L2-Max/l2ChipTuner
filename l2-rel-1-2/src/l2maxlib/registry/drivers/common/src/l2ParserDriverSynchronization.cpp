/*
 * l2ParserDriverSynchronization.cpp
 *
 *  Created on: 05.09.2009
 *      Author: L2-Max
 */

#include "l2ParserDriverSynchronization.h"

namespace l2max
{
   namespace Registry
   {
      namespace Driver
      {
         CParserDriverSynchronization::CParserDriverSynchronization() : _terminateCondition( true ){}
         CParserDriverSynchronization::~CParserDriverSynchronization(){}

         bool CParserDriverSynchronization::isRun()const
         {
            return !_parserStopCondition.waitForEvent( 0 );
         }

         bool CParserDriverSynchronization::WaitForData()const
         {
            return _dataReadCondition.wait( mutex() );
         }
         
         bool CParserDriverSynchronization::WaitForContinue()const
         {
            return _readContinueCondition.waitForEvent();
         }

         CMutex& CParserDriverSynchronization::mutex()const
         {
            return _mutex;
         }
         
         void CParserDriverSynchronization::continueRead()const
         {
            _readContinueCondition.wake();
         }

         void CParserDriverSynchronization::parserStopped()const
         {
            _parserStopCondition.wake();

            dataRead();
         }

         void CParserDriverSynchronization::dataRead()const
         {
            _dataReadCondition.wake();
         }
         
         void CParserDriverSynchronization::terminate()const
         {
            _terminateCondition.wake();
         }
         
         void CParserDriverSynchronization::ResetTerminate()const
         {
            _terminateCondition.reset();
         }
         
         bool CParserDriverSynchronization::isTerminate()const
         {
            return _terminateCondition.waitForEvent( 0 );
         }
      }
   }
}
