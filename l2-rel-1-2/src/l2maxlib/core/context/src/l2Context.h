/*
 * l2Context.h
 *
 *  Created on: 12.01.2009
 *      Author: mars
 */

#ifndef L2CONTEXT_H_
#define L2CONTEXT_H_

namespace l2max
{
   class CSingletonRepository;
   class CRefObjectRepository;
   class CExceptionRepository;
   
   class CContext
   {
      static CContext* _instance;

      CSingletonRepository* _singletonRepository;
      CRefObjectRepository* _refObjectRepository;
      CExceptionRepository* _exceptionRepository;

      static void UnexpectedHandler();
      static void TerminateHandler();

   public:
      CContext( int anArgc, char* anArgv[] );
      virtual ~CContext()throw();

      static CContext* GetInstance();

      CSingletonRepository& singletonRepository()const;
      CRefObjectRepository& refObjectRepository()const;
      CExceptionRepository& exceptionRepository()const;
   };
}

#endif /* L2CONTEXT_H_ */
