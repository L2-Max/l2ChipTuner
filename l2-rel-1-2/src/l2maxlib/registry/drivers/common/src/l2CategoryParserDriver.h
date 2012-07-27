/*
 * l2CategoryParserDriver.h
 *
 *  Created on: 30.03.2009
 *      Author: L2-Max
 */

#ifndef L2CATEGORYPARSERDRIVER_H_
#define L2CATEGORYPARSERDRIVER_H_

#include "l2ParserDriver.h"

#include <l2RegistryDriverTypes.h>
#include <l2RefObject.h>
#include <l2RefHandle.hxx>
#include <l2Variant.h>

#include <map>
#include <stack>

namespace l2max
{
   namespace Registry
   {
      namespace Driver
      {
         ////////////////////////////////////////////////////////////////////////////////////////////////
         class CCategory;

         class CVariable : public CVariant,
                           public CRefObject
         {
         public:
            CVariable(){}
            CVariable( const CVariant& aVariant )
            {
               operator = ( aVariant );
            }

            CVariable& operator = ( const CVariant& aVariant )
            {
               CVariant::operator = ( aVariant );
            }
         };

         typedef CRefHandle< CCategory > HCategory;
         typedef CRefHandle< CVariable > HVariable;

         template< class TUniqueObject >
            struct SUniqueSeqPredicate
            {
               bool operator()( const TUniqueObject& aFirst, const TUniqueObject& aNext )const
               {
                  return aFirst.first < aNext.first;
               }
            };

         typedef std::string TKeyType;

         typedef std::pair< TKeyType, HVariable > TVariable;
         typedef std::map< TKeyType, TVariable > TVariableSeq;

         typedef std::pair< TKeyType, HCategory > TCategory;
         typedef std::map< TKeyType, TCategory > TCategorySeq;

         struct SCategoryMembers
         {
            TVariableSeq _variables;
            TCategorySeq _categories;
         };

         class CCategoryKeyParser;

         class CCategory : public CRefObject,
                           private SCategoryMembers
         {
            TKeyListIterator CategoryList( TKeyList& aKeys )const;
            TKeyListIterator VariableList( TKeyList& aKeys )const;
            
            CCategory( const CCategory& );
            CCategory& operator = ( const CCategory& );
            
         public:
            CCategory();
            CCategory( const SCategoryMembers& aMembers );

            bool Merge( const CCategory& aMembers );
            bool MergeRecursive( const CCategory& aMembers );

            void dump( int anIndentation = 0 )const;

            const SCategoryMembers& members()const;

            bool Read( CCategoryKeyParser& aKey, CVariant& aValue )const;
            bool Write( CCategoryKeyParser& aKey, const CVariant& aValue );
            
            void Clear();
            
            TKeyListIterator CategoryList( CCategoryKeyParser& aKey, TKeyList& aKeys )const;
            TKeyListIterator VariableList( CCategoryKeyParser& aKey, TKeyList& aKeys )const;
         };

         ////////////////////////////////////////////////////////////////////////////////////////////////
         class CParserDriverSynchronization;
         class CParserErrorRepository;

         class CCategoryParserDriver : public CParserDriver
         {
            typedef std::stack< HCategory > TCategoryStack;
            TCategoryStack _categoryStack;

            const CParserDriverSynchronization& _synchronization;
            CParserErrorRepository& _errorRepository;
            
            unsigned int _categoryBuffSize;

         public:
            CCategoryParserDriver( const std::string& aFile,
                                   CLexScaner& aLexer,
                                   const HCategory& aCategory,
                                   const CParserDriverSynchronization& aSynchronization,
                                   CParserErrorRepository& anErrorRepository );

            virtual ~CCategoryParserDriver();

            virtual void RegisterError( const std::string& anError )const;

            bool PushCategory( const TCategory& aCategory );
            bool PopCategory();
            
            bool isTerminate()const;

            TCategoryStack::size_type categoryStackSize()const;
         };
      }
   }
}

#endif /* L2CATEGORYPARSERDRIVER_H_ */
