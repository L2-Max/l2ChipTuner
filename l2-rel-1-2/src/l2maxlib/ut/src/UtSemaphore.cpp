/*
 * UtSemaphore.cpp
 *
 *  Created on: 28.07.2009
 *      Author: mars
 */

#include "UtSemaphore.h"

#include <l2Trace.h>
#include <l2Semaphore.h>
#include <l2Thread.h>

#include <iostream>

namespace l2max
{
   namespace ut
   {
      namespace
      {
         class CUtThread : public CThread
         {
            static unsigned int _counter;
            static CSemaphore _semaphore;
            
            virtual unsigned int run()
            {
               unsigned int ret( 0 );
               
               CUtThread::_semaphore.Acquire();
               
               ++CUtThread::_counter;
               
               std::cout << "New thread started" << std::endl;
               std::cout << CUtThread::_counter << " currently running thread(s)" << std::endl;
               
               if( !CUtThread::_stop )
               {
                  ( new CUtThread() )->Start();
               }
               
               CUtThread::Sleep( 500 );
               
               std::cout << "Thread exited" << std::endl;
               
               --CUtThread::_counter;
               
               CUtThread::_semaphore.Release();
               
               delete this;
               
               return ret;
            }
            
         public:
            static bool _stop;
            
            CUtThread(){}            
            ~CUtThread(){}
         };
         
         bool CUtThread::_stop( false );
         unsigned int CUtThread::_counter( 0 );
         
         CSemaphore CUtThread::_semaphore( 5 );
      }
      
      CUtSemaphore::CUtSemaphore()
      {
         TRACE_FUN( Routine, "CUtSemaphore::CUtSemaphore" );
         
         ( new CUtThread() )->Start();
         
         CUtThread::Sleep( 5000 );
         
         CUtThread::_stop = true;
         
         CUtThread::Sleep( 500 * 5 );
      }
   
      CUtSemaphore::~CUtSemaphore()
      {
         TRACE_FUN( Routine, "CUtSemaphore::~CUtSemaphore" );
      }
   }
}
