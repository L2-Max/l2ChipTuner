/*
 * l2Profiler.cpp
 *
 *  Created on: 22.01.2009
 *      Author: mars
 */

#include "l2Profiler.h"
#include "l2Trace.h"

#include <l2MutexLocker.h>

#include <sstream>

#define ENV_PROFILER_SORTTYPE_VAR "L2MAXLIB_PROFILER_SORTING_TYPE"

namespace l2max
{
   struct CProfiler::SEntryInfo
   {
      unsigned long long _entryCount;
   };

   struct CProfiler::SFunctionInfo : public CProfiler::SEntryInfo
   {
      double _totalCallTime;
      double _minCallTime;
      double _maxCallTime;

      double _totalThreadTime;
      double _minThreadTime;
      double _maxThreadTime;

      SFunctionInfo()
      {
         _entryCount = 0;

         _totalCallTime = .0;
         _minCallTime = -1.;
         _maxCallTime = .0;

         _totalThreadTime = .0;
         _minThreadTime = -1.;
         _maxThreadTime = .0;
      }
   };
   
   ///////////////////////////////////////////////////////////////////////////////////
   struct CProfiler::SSortPred
   {
      ESortType _type;
      
      SSortPred( ESortType aType )
      {
         _type = aType;
      }
      
      bool operator()( const TSortedInfoContainer::value_type& aFirst, const TSortedInfoContainer::value_type& aSecond )const
      {
         bool ret( false );
         
         if( _type == stName )
         {
            ret = aFirst.first < aSecond.first;
         }
         else if( _type == stEntryCount )
         {
            ret = aFirst.second._entryCount < aSecond.second._entryCount;
         }
         else if( _type == stMinCallTime )
         {
            ret = aFirst.second._minCallTime < aSecond.second._minCallTime;
         }
         else if( _type == stMaxCallTime )
         {
            ret = aFirst.second._maxCallTime < aSecond.second._maxCallTime;
         }
         else if( _type == stAvrageCallTime )
         {
            ret = aFirst.second._totalCallTime / aFirst.second._entryCount < aSecond.second._totalCallTime / aSecond.second._entryCount;
         }
         else if( _type == stTotalCallTime )
         {
            ret = aFirst.second._totalCallTime < aSecond.second._totalCallTime;
         }
         else if( _type == stMinThreadTime )
         {
            ret = aFirst.second._minThreadTime < aSecond.second._minThreadTime;
         }
         else if( _type == stMaxThreadTime )
         {
            ret = aFirst.second._maxThreadTime < aSecond.second._maxThreadTime;
         }
         else if( _type == stAvrageThreadTime )
         {
            ret = aFirst.second._totalThreadTime / aFirst.second._entryCount < aSecond.second._totalThreadTime / aSecond.second._entryCount;
         }
         else if( _type == stTotalThreadTime )
         {
            ret = aFirst.second._totalThreadTime < aSecond.second._totalThreadTime;
         }
         
         return ret;
      }
   };

   ///////////////////////////////////////////////////////////////////////////////////
   CProfiler::ESortType CProfiler::_maxSortType( stTotalThreadTime );
   
   CProfiler::CProfiler(){}
   CProfiler::~CProfiler(){}
   
   CProfiler::ESortType CProfiler::SortType()
   {
      ESortType ret( stName );
      
      const char* envSort( getenv( ENV_PROFILER_SORTTYPE_VAR ) );

      if( envSort )
      {
         unsigned int tmpSort(0);
         sscanf( envSort, "%ud", &tmpSort );

         if( tmpSort <= _maxSortType )
         {
            ret = static_cast< ESortType >( tmpSort );
         }
         else
         {
            TraceCritical << std::endl
                          << "Unknown sorting type specified ( check L2MAXLIB_PROFILER_SORTING_TYPE environment variable ) ..." << std::endl
                          << "Sorting by default." << std::endl;
         }
      }
      
      return ret;
   }

   CProfiler::TSortedInfoContainer CProfiler::Sort( ESortType aType )const
   {
      TSortedInfoContainer ret;
      
      for( TFunctionInfoMap::const_iterator I_info( _functionInfoMap.begin() ); I_info != _functionInfoMap.end(); ++I_info )
      {
         ret.push_back( *I_info );
      }
      
      ////////////////////////////////////////////////////////////////////////////
      std::sort( ret.begin(), ret.end(), SSortPred( aType ) );
      
      return ret;
   }

   void CProfiler::Print()const
   {
      CMutexLocker locker( _mutex );
      
      std::map< ESortType, std::string > theSortType2String;
      
      theSortType2String[ stName ]              = "name";
      theSortType2String[ stEntryCount ]        = "entry count";
      theSortType2String[ stMinCallTime ]       = "min call time";
      theSortType2String[ stMaxCallTime ]       = "max call time";
      theSortType2String[ stAvrageCallTime ]    = "avr call time";
      theSortType2String[ stTotalCallTime ]     = "total call time";
      theSortType2String[ stMinThreadTime ]     = "min thread time";
      theSortType2String[ stMaxThreadTime ]     = "max thread time";
      theSortType2String[ stAvrageThreadTime ]  = "avr thread time";
      theSortType2String[ stTotalThreadTime ]   = "total thread time";
      
      ESortType theSortType( CProfiler::SortType() );

      TraceCritical << "*** Profiler report ***" << std::endl;
      TraceCritical << "Sort By: " << theSortType2String[ theSortType ] << std::endl;
      
      TSortedInfoContainer sortedInfo( Sort( theSortType ) );
      
      std::ostringstream profRepStream;
      profRepStream.width( 6 );
      profRepStream.precision( 6 );
      profRepStream.setf( std::ios_base::fixed, std::ios_base::floatfield );

      for( TSortedInfoContainer::const_iterator i( sortedInfo.begin() ); i != sortedInfo.end(); ++i )
      {
         profRepStream << "Function name: "  << i->first                                           << std::endl;
         profRepStream << " Calls: "         << i->second._entryCount                              << std::endl;
         
         profRepStream << " Real/Proc time ( sec ): "                                              << std::endl
                       << " min: "            << i->second._minCallTime                             << "/"
                                             << i->second._minThreadTime                           << ", "
                       << " max: "            << i->second._maxCallTime                             << "/"
                                             << i->second._maxThreadTime                           << ", "
                       << " avr: "            << i->second._totalCallTime / i->second._entryCount   << "/"
                                             << i->second._totalThreadTime / i->second._entryCount << ", "
                       << " total: "          << i->second._totalCallTime                           << "/"
                                             << i->second._totalThreadTime                         << std::endl
                                                                                                   << std::endl;
      }
      
      TraceCritical << profRepStream.str();

      for( TEntryInfoMap::const_iterator i( _entryInfoMap.begin() ); i != _entryInfoMap.end(); ++i )
      {
         TraceCritical << "Entry name: "     << i->first << std::endl;
         TraceCritical << "   Count: "       << i->second._entryCount << std::endl;
      }
   }

   void CProfiler::RegisterFunction( const std::string& aName, unsigned long aCallTime, unsigned long long aThreadTime )
   {
      CMutexLocker locker( _mutex );

      double theCallTime( aCallTime / 1000. );
		double theThreadTime( aThreadTime / 10000000. );

      SFunctionInfo& functionInfo( _functionInfoMap[ aName ] );

      ++functionInfo._entryCount;

      /////////////////////////////////////////////////////////////////////////////////////////////
      functionInfo._totalCallTime += theCallTime;

      if( functionInfo._minCallTime == -1. || theCallTime < functionInfo._minCallTime )
      {
         functionInfo._minCallTime = theCallTime;
      }

      if( theCallTime > functionInfo._maxCallTime )
      {
         functionInfo._maxCallTime = theCallTime;
      }

      /////////////////////////////////////////////////////////////////////////////////////////////
      functionInfo._totalThreadTime += theThreadTime;

      if( functionInfo._minThreadTime == -1. || theThreadTime < functionInfo._minThreadTime )
      {
         functionInfo._minThreadTime = theThreadTime;
      }

      if( theThreadTime > functionInfo._maxThreadTime )
      {
         functionInfo._maxThreadTime = theThreadTime;
      }
   }

   void CProfiler::RegisterEntry( const std::string& aName )
   {
      CMutexLocker locker( _mutex );

      SEntryInfo& entryInfo( _entryInfoMap[ aName ] );

      ++entryInfo._entryCount;
   }
}
