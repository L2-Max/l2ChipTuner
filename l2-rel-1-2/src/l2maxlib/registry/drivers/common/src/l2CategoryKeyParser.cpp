#include "l2CategoryKeyParser.h"

#include <l2Trace.h>
#include <l2String.h>

namespace l2max
{
   namespace Registry
   {
      namespace Driver
      {
         const char* CCategoryKeyParser::_categorySeparator = "::";

         CCategoryKeyParser::CCategoryKeyParser()
         {
            SetComplex( "" );
         }

         CCategoryKeyParser::CCategoryKeyParser( const std::string& aComplex )
         {
            SetComplex( aComplex );
         }

         CCategoryKeyParser::~CCategoryKeyParser(){}

         bool CCategoryKeyParser::SetComplex( const std::string& aComplex )
         {
            bool ret( false );
            
            _absolute = false;
            _currentCategory = _categoryList.end();

            _complexCategory = aComplex;

            if( ret = parse() )
            {
               _currentCategory = _categoryList.begin();
            }
            
            return ret;
         }

         bool CCategoryKeyParser::NextCategory( std::string& aCategory )const
         {
            bool ret( IsCategoryValid() );

            if( ret )
            {
               aCategory = *_currentCategory;
               
               ++_currentCategory;
            }

            return ret && aCategory.size();
         }

         bool CCategoryKeyParser::Variable( std::string& aVariable )const
         {
            aVariable = _variable;

            return _variable.size();
         }

         bool CCategoryKeyParser::IsAbsolute()const
         {
            return _absolute;
         }

         bool CCategoryKeyParser::parse()
         {
            bool ret( false );

            _categoryList.clear();
            _variable.clear();

            _absolute = false;

            TStringList stringList( CString::String2List( _complexCategory, CCategoryKeyParser::_categorySeparator ) );

            if( stringList.size() )
            {
               _variable = stringList.back();

               stringList.pop_back();

               if( !stringList.front().size() )
               {
                  _absolute = true;

                  stringList.pop_front();
               }

               _categoryList.insert( _categoryList.end(), stringList.begin(), stringList.end() );
               
               ret = true;
            }

            return ret;
         }

         int CCategoryKeyParser::CategoryCount()const
         {
            return _categoryList.size();
         }

         bool CCategoryKeyParser::IsCategoryValid()const
         {
            return _currentCategory != _categoryList.end();
         }
      }
   }
}
