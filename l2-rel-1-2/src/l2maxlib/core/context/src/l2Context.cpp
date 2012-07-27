/*
 * l2Context.cpp
 *
 *  Created on: 12.01.2009
 *      Author: mars
 */

#include "l2Context.h"
#include "l2SingletonRepository.h"
#include "l2RefObjectRepository.h"
#include "l2ExceptionRepository.h"

#include <l2Exception.h>
#include <l2TraceStream.h>
#include <l2Trace.h>
#include <l2Profiler.h>

#include <iostream>

namespace l2max
{
   CContext* CContext::_instance( 0 );

   CContext::CContext( int anArgc, char* anArgv[] )
   {
      if( CContext::_instance )
      {
         throw CException( "CContext::CContext: more than one instance not supported" );
      }

      CContext::_instance = this;

      std::set_unexpected( CContext::UnexpectedHandler );
      std::set_terminate( CContext::TerminateHandler );

      _singletonRepository = new CSingletonRepository();
      _refObjectRepository = new CRefObjectRepository();
      _exceptionRepository = new CExceptionRepository();

      l2max::CTraceStream::GetInstance()->Initialize( anArgc, anArgv );

      TRACE_FUN( Routine, "CContext::CContext" );
   }

   CContext::~CContext()throw()
   {
      CProfiler::GetInstance()->Print();

      TraceRoutine << " !!! Context about to destroy !!!" << std::endl;
      TraceRoutine << "Objects in context: " << std::endl;

      delete _exceptionRepository;
      delete _refObjectRepository;
      delete _singletonRepository;

      CContext::_instance = 0;
   }

   CContext* CContext::GetInstance()
   {
      if( !_instance )
      {
         std::cout << "CContext::GetInstance return NULL pointer" << std::endl;

         throw CException( "CContext::GetInstance return NULL pointer" );
      }

      return CContext::_instance;
   }

   CSingletonRepository& CContext::singletonRepository()const
   {
      return *_singletonRepository;
   }

   CRefObjectRepository& CContext::refObjectRepository()const
   {
      return *_refObjectRepository;
   }
   
   CExceptionRepository& CContext::exceptionRepository()const
   {
      return *_exceptionRepository;
   }

   void CContext::UnexpectedHandler()
   {
      TraceCritical << "Unexpected exception has been thrown" << std::endl;
   }

   void CContext::TerminateHandler()
   {
      TraceCritical << "Program abnormaly terminated" << std::endl;
   }
}
