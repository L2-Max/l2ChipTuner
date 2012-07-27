#ifndef L2CATEGORYKEYPARSER_H_
#define L2CATEGORYKEYPARSER_H_

#include <string>
#include <list>

namespace l2max
{
   namespace Registry
   {
      namespace Driver
      {
         class CCategoryKeyParser
         {
            static const char* _categorySeparator;

            std::string _complexCategory;

            typedef std::list< std::string > TCategoryList;
            TCategoryList _categoryList;

            mutable TCategoryList::const_iterator _currentCategory;

            bool _absolute;
            std::string _variable;

            bool parse();

         public:
            CCategoryKeyParser();
            CCategoryKeyParser( const std::string& aComplex );
            ~CCategoryKeyParser();

            bool SetComplex( const std::string& aComplex );

            bool NextCategory( std::string& aCategory )const;
            bool Variable( std::string& aVariable )const;

            bool IsAbsolute()const;
            bool IsCategoryValid()const;

            int CategoryCount()const;
         };
      }
   }
}

#endif /*L2CATEGORYKEYPARSER_H_*/
