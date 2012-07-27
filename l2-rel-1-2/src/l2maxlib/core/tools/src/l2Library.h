#ifndef L2LIBRARY_H_
#define L2LIBRARY_H_

#include <string>

namespace l2max
{
   typedef void ( *pfnType )();
   
   class CLibrary
   {
      std::string _file;
      void* _module;
      
      CLibrary( const CLibrary& );
      CLibrary& operator = ( const CLibrary& );
      
   public:
      CLibrary();
      CLibrary( const std::string& aFile );
      virtual ~CLibrary();
      
      void SetFile( const std::string& aFile );
      
      bool Load();
      bool UnLoad();
      
      bool IsLoaded()const;
      
      pfnType Resolve( const std::string& anEntry )const;
   };
}

#endif /*L2LIBRARY_H_*/
