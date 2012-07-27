#include "UtRegistry.h"
#include "UtSemaphore.h"
#include "UtRefHandle.h"

#include <l2Context.h>
#include <l2Trace.h>
#include <l2ByteArray.h>

#include <iostream>

/*!
 * \namespace l2max
 * \brief All classes in the L2Max library are included in l2max namespace
 */
int main( int argc, char* argv[] )
{
   int ret( 0 );

   l2max::CContext ctx( argc, argv );

   TRACE_FUN( Routine, "::main" );
   
   l2max::ut::CUtRegistry utRegistry;
   //l2max::ut::CUtSemaphore utSemaphore;
   //l2max::ut::CUtRefHandle utRefHandle;

   return ret;
}
