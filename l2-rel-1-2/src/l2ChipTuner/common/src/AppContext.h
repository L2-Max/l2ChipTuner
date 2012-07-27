/*
 * AppContext.h
 *
 *  Created on: 03.10.2008
 *      Author: mars
 */

#ifndef APPCONTEXT_H_
#define APPCONTEXT_H_

#include <l2Context.h>
#include <l2TextTreeRegistry.h>

class CAppContext : public l2max::CContext
{
   l2max::CTextTreeRegistry _mainRegistry;

   void InitRegistry();
   void UinitRegistry();

   CAppContext( const CAppContext& );
   CAppContext& operator=( const CAppContext& );

public:
   CAppContext( int anArgc, char* anArgv[] );
   virtual ~CAppContext()throw();

   static CAppContext* GetInstance();

   l2max::CTextTreeRegistry& MainRegistry();
};

#endif /* MAINAPPCONTEXT_H_ */
