/*
 * l2Profiler.h
 *
 *  Created on: 22.01.2009
 *      Author: mars
 */

#ifndef L2PROFILER_H_
#define L2PROFILER_H_

#include <l2Singleton.hxx>
#include <l2Mutex.h>

#include <map>
#include <vector>

namespace l2max
{
   class CProfiler : public CSingleton< CProfiler >
   {
      friend class CSingleton< CProfiler >;

      struct SEntryInfo;
      struct SFunctionInfo;

      typedef std::map< std::string, SEntryInfo > TEntryInfoMap;
      TEntryInfoMap _entryInfoMap;

      typedef std::map< std::string, SFunctionInfo > TFunctionInfoMap;
      TFunctionInfoMap _functionInfoMap;
      
      typedef std::vector< std::pair< std::string, SFunctionInfo > > TSortedInfoContainer;

      mutable CMutex _mutex;

      CProfiler();
      virtual ~CProfiler();

      CProfiler( const CProfiler& );
      CProfiler& operator = ( const CProfiler& );
      
      enum ESortType
      {
         stName               = 0x00,
         stEntryCount         = 0x01,
         stMinCallTime        = 0x02,
         stMaxCallTime        = 0x03,
         stAvrageCallTime     = 0x04,
         stTotalCallTime      = 0x05,
         stMinThreadTime      = 0x06,
         stMaxThreadTime      = 0x07,
         stAvrageThreadTime   = 0x08,
         stTotalThreadTime    = 0x09
      };
      
      struct SSortPred;
      
      static ESortType _maxSortType;
      
      static ESortType SortType();
      
      TSortedInfoContainer Sort( ESortType aType )const;

   public:
      void Print()const;

      void RegisterFunction( const std::string& aName, unsigned long aCallTime, unsigned long long aThreadTime );
      void RegisterEntry( const std::string& aName );
   };
}

#endif /* L2PROFILER_H_ */
