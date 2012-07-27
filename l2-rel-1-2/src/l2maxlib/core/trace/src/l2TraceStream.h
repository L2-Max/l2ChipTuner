/*
 * l2TraceStream.h
 *
 *  Created on: 11.01.2009
 *      Author: L2-Max
 */

#ifndef L2TRACESTREAM_H_
#define L2TRACESTREAM_H_

#include "l2TraceDefs.h"

#include <l2Singleton.hxx>

#include <fstream>
#include <map>

namespace l2max
{
   class CTraceStream : public CSingleton< CTraceStream >
   {
      friend class CSingleton< CTraceStream >;
      
      typedef std::map< ETraceLevel, std::string > TTraceLevelToStringMap;
      TTraceLevelToStringMap _traceLevelToStringMap;
      
      std::fstream _stream;
      unsigned char _envLevel;

      CTraceStream( const CTraceStream& );
      CTraceStream& operator = ( const CTraceStream& );

      CTraceStream();
      virtual ~CTraceStream();
      
      unsigned char envLevel()const;

   public:
      static std::string dateTime();
      static std::string dateTimeMsecs();
      
      bool Initialize( int anArgc, char* anArgv[] );
      bool IsLevelMatch( ETraceLevel aLevel )const;
      
      std::fstream& stream();
   };
}

#endif /* L2TRACESTREAM_H_ */
