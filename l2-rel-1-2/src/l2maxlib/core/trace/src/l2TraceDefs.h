/*
 * l2TraceDefs.h
 *
 *  Created on: 22.01.2009
 *      Author: mars
 */

#ifndef L2TRACEDEFS_H_
#define L2TRACEDEFS_H_

#define MAX_TRACE_LEVEL l2max::Cycled

#define TRACE( aLevel, aFile, aLine ) \
   if( l2max::CTrace::IsLevelMatch( aLevel ) ) \
      l2max::CTrace( __FILE__, __LINE__ ).impl()

#define TraceCritical   TRACE( l2max::Critical, __FILE__, __LINE__ )
#define TraceException  TRACE( l2max::Exception, __FILE__, __LINE__ )
#define TraceRoutine    TRACE( l2max::Routine, __FILE__, __LINE__ )
#define TraceFrequently TRACE( l2max::Frequently, __FILE__, __LINE__ )
#define TraceCycled     TRACE( l2max::Cycled, __FILE__, __LINE__ )
#define TRACE_FUN( aLevel, aName ) l2max::CTraceFn __traceFn__##aLevel( l2max::aLevel, aName, __FILE__, __LINE__ );

#define TRACE_VAR( aLevel, aVar ) Trace##aLevel << #aVar << ": " << ( aVar ) << std::endl
#define TRACE_LINE TraceCritical << __FILE__ << ":" << __LINE__ << std::endl;

namespace l2max
{
   enum ETraceLevel
   {
      Critical,
      Exception,
      Routine,
      Frequently,
      Cycled
   };
}

#endif /* L2TRACEDEFS_H_ */
