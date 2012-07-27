/*
 * l2WaitCondition.h
 *
 *  Created on: 08.10.2008
 *      Author: mars
 */

#ifndef L2WAITCONDITION_H_
#define L2WAITCONDITION_H_

namespace l2max
{
   class CMutex;

   class CWaitCondition
   {
      struct SWaitConditionPrivateHandle;

      SWaitConditionPrivateHandle* _waitConditionPrivateHandle;

      CWaitCondition( const CWaitCondition& );
      CWaitCondition& operator = ( const CWaitCondition& );

   public:
      enum EWaitTime
      {
         wtInfinite = -1
      };

      CWaitCondition( bool aManual = false );
      virtual ~CWaitCondition();

      bool wait( CMutex& aMutex, unsigned int aTime = wtInfinite );
      bool waitForEvent( unsigned int aTime = wtInfinite );

      void wake();

      bool reset();
   };

}

#endif /* L2WAITCONDITION_H_ */
