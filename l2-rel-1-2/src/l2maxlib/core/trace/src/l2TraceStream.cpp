/*
 * l2TraceStream.cpp
 *
 *  Created on: 11.01.2009
 *      Author: L2-Max
 */

#include "l2TraceStream.h"
#include "l2Trace.h"

#include <stdio.h>
#include <sys/time.h>

#define ENV_TRACE_LEVEL_VAR "L2MAXLIB_TRACE_LEVEL"

namespace l2max
{
   CTraceStream::CTraceStream()
   {
      _envLevel = 0;

      _traceLevelToStringMap[ Critical ] = "Critical";
      _traceLevelToStringMap[ Exception ] = "Exception";
      _traceLevelToStringMap[ Routine ] = "Routine";
      _traceLevelToStringMap[ Frequently ] = "Frequently";
      _traceLevelToStringMap[ Cycled ] = "Cycled";
   }

   CTraceStream::~CTraceStream()
   {
      _stream << " *** Trace end at: " << dateTime() << " ***" << std::endl << std::endl;
      _stream.close();
   }

   bool CTraceStream::Initialize( int anArgc, char* anArgv[] )
   {
      if( anArgc )
      {
         std::string file( *anArgv );
         file += ".trace";

         _stream.open( file.c_str(), std::ios_base::out | std::ios_base::app );
         _stream << " *** Trace initialized at: " << dateTime() << " ***" << std::endl;

         const char* envLevel( getenv( ENV_TRACE_LEVEL_VAR ) );

         if( envLevel )
         {
            unsigned int tmpLvl(0);
            sscanf( envLevel, "%d", &tmpLvl );

            _envLevel = tmpLvl;

            if( _envLevel > MAX_TRACE_LEVEL )
            {
               _envLevel = MAX_TRACE_LEVEL;
            }
         }

         _stream << " *** Current trace level is: " << _traceLevelToStringMap[ static_cast< ETraceLevel >( _envLevel ) ] << " ***" << std::endl;
         _stream << " *** NOTE: To change trace level set environment variable "ENV_TRACE_LEVEL_VAR" to one of: " << std::endl;
         _stream << " ***          Critical:   0" << std::endl;
         _stream << " ***          Exception:  1" << std::endl;
         _stream << " ***          Routine:    2" << std::endl;
         _stream << " ***          Frequently: 3" << std::endl;
         _stream << " ***          Cycled:     4" << std::endl;
      }

      return _stream.is_open();
   }

   std::string CTraceStream::dateTime()
   {
      time_t simpleTime( time( 0 ) );

      char timeBuf[ 64 ] = {0};
      strftime( timeBuf, sizeof( timeBuf ), "%H:%M:%S %d/%m/%y ", localtime( &simpleTime ) );

      return std::string( timeBuf );
   }

   std::string CTraceStream::dateTimeMsecs()
   {
      timeval simpleTime;
      gettimeofday( &simpleTime, 0 );

      char msecBuf[16]={0};
      sprintf( msecBuf,"%3d", CTraceFn::CallTime() % 1000 );

      char timeBuf[64] = {0};
      strftime( timeBuf, sizeof(timeBuf), "%H%M%S:", localtime( &simpleTime.tv_sec ) );

      return std::string( timeBuf ) + std::string( msecBuf );
   }

   bool CTraceStream::IsLevelMatch( ETraceLevel aLevel )const
   {
      return aLevel <= envLevel();
   }

   unsigned char CTraceStream::envLevel()const
   {
      return _envLevel;
   }

   std::fstream& CTraceStream::stream()
   {
      return _stream;
   }
}
