/*
 * l2CategoryParserDriver.cpp
 *
 *  Created on: 30.03.2009
 *      Author: L2-Max
 */

#include "l2CategoryParserDriver.h"
#include "l2CategoryKeyParser.h"
#include "l2ParserErrorRepository.h"
#include "l2AbstractRegistryDriver.h"

#include <l2Variant.h>
#include <l2Trace.h>
#include <l2ExceptionThrower.h>
#include <l2MutexLocker.h>

#include <iostream>

namespace l2max
{
   namespace Registry
   {
      namespace Driver
      {
         /////////////////////////////////////////////////////////////////////////////////////////
         CCategory::CCategory(){}
         CCategory::CCategory( const SCategoryMembers& aMembers ) : SCategoryMembers( aMembers ){}

         bool CCategory::Merge( const CCategory& aMembers )
         {
            bool ret( false );

            if( aMembers.members()._variables.size() && ( ret = ( _variables.find( aMembers.members()._variables.begin()->first ) == _variables.end() ) ) )
            {
               _variables.insert( *aMembers.members()._variables.begin() );
            }
            else if( aMembers.members()._categories.size() && ( ret = ( _categories.find( aMembers.members()._categories.begin()->first ) == _categories.end() ) ) )
            {
               _categories.insert( *aMembers.members()._categories.begin() );
            }

            return ret;
         }

         bool CCategory::MergeRecursive( const CCategory& aMembers )
         {
            bool ret( false );

            if( aMembers.members()._variables.size() && ( ret = ( _variables.find( aMembers.members()._variables.begin()->first ) == _variables.end() ) ) )
            {
               _variables.insert( *aMembers.members()._variables.begin() );
            }
            else if( aMembers.members()._categories.size() )
            {
               TCategorySeq::iterator i( _categories.find( aMembers.members()._categories.begin()->first ) );

               if( ret = ( i == _categories.end() ) )
               {
                  _categories.insert( *aMembers.members()._categories.begin() );
               }
               else if( aMembers.members()._categories.begin()->second.second->members()._variables.size() )
               {
                  ret = i->second.second->MergeRecursive( aMembers.members()._categories.begin()->second.second->members() );
               }
            }

            return ret;
         }

         void CCategory::dump( int anIndentation )const
         {
            std::string indentation( anIndentation * 3, ' ' );

            for( TVariableSeq::const_iterator i( _variables.begin() ); i != _variables.end(); ++i )
            {
               std::cout << indentation;
               std::cout << "Variable: " << i->first << std::endl;

               TraceRoutine << indentation << "Variable: " << i->first << std::endl;
            }

            for( TCategorySeq::const_iterator i( _categories.begin() ); i != _categories.end(); ++i )
            {
               std::cout << indentation;
               std::cout << "Category: " << i->first << std::endl;

               TraceRoutine << indentation << "Category: " << i->first << std::endl;

               i->second.second->dump( anIndentation + 1 );
            }
         }

         const SCategoryMembers& CCategory::members()const
         {
            return *this;
         }

         bool CCategory::Read( CCategoryKeyParser& aKey, CVariant& aValue )const
         {
            bool ret( false );

            std::string categoryName;

            if( aKey.NextCategory( categoryName ) )
            {
               TCategorySeq::const_iterator i( _categories.find( categoryName ) );

               if( i != _categories.end() )
               {
                  ret = i->second.second->Read( aKey, aValue );
               }
            }
            else
            {
               std::string variableName;

               if( aKey.Variable( variableName ) )
               {
                  TVariableSeq::const_iterator i( _variables.find( variableName ) );

                  if( i != _variables.end() )
                  {
                     aValue = *i->second.second;

                     ret = true;
                  }
               }
            }

            return ret;
         }

         bool CCategory::Write( CCategoryKeyParser& aKey, const CVariant& aValue )
         {
            bool ret( false );

            std::string categoryName;

            if( aKey.NextCategory( categoryName ) )
            {
               TCategorySeq::iterator i( _categories.find( categoryName ) );

               if( i == _categories.end() )
               {
                  i = _categories.insert( TCategorySeq::value_type( categoryName, TCategory( categoryName, *new CCategory() ) ) ).first;
               }

               ret = i->second.second->Write( aKey, aValue );
            }
            else
            {
               std::string variableName;

               if( aKey.Variable( variableName ) )
               {
                  TVariableSeq::iterator i( _variables.find( variableName ) );

                  if( i == _variables.end() )
                  {
                     i = _variables.insert( TVariableSeq::value_type( variableName, TVariable( variableName, *new CVariable() ) ) ).first;
                  }

                  *i->second.second = aValue;

                  ret = true;
               }
            }

            return ret;
         }
         
         void CCategory::Clear()
         {
            _categories.clear();
            _variables.clear();
         }
         
         TKeyListIterator CCategory::CategoryList( CCategoryKeyParser& aKey, TKeyList& aKeys )const
         {
            TKeyListIterator ret( aKeys.end() );

            std::string categoryName;

            if( !aKey.NextCategory( categoryName ) )
            {
               aKey.Variable( categoryName );
            }

            if( categoryName.size() )
            {
               TCategorySeq::const_iterator i( _categories.find( categoryName ) );

               if( i != _categories.end() )
               {
                  if( aKey.IsCategoryValid() )
                  {
                     ret = i->second.second->CategoryList( aKey, aKeys );
                  }
                  else
                  {
                     ret = i->second.second->CategoryList( aKeys );
                  }
               }
            }
            else
            {
               ret = CategoryList( aKeys );
            }

            return ret;
         }
         
         TKeyListIterator CCategory::VariableList( CCategoryKeyParser& aKey, TKeyList& aKeys )const
         {
            TKeyListIterator ret( aKeys.end() );

            std::string categoryName;

            if( !aKey.NextCategory( categoryName ) )
            {
               aKey.Variable( categoryName );
            }

            if( categoryName.size() )
            {
               TCategorySeq::const_iterator i( _categories.find( categoryName ) );

               if( i != _categories.end() )
               {
                  if( aKey.IsCategoryValid() )
                  {
                     ret = i->second.second->VariableList( aKey, aKeys );
                  }
                  else
                  {
                     ret = i->second.second->VariableList( aKeys );
                  }
               }
            }
            else
            {
               ret = VariableList( aKeys );
            }

            return ret;
         }
         
         TKeyListIterator CCategory::CategoryList( TKeyList& aKeys )const
         {
            TKeyListIterator ret( aKeys.end() );

            TCategorySeq::const_iterator I_categories( _categories.begin() );

            if( aKeys.size() )
            {
               I_categories = _categories.find( aKeys.back() );

               if( I_categories != _categories.end() )
               {
                  ++I_categories;
               }
            }

            for( ; I_categories != _categories.end(); ++I_categories )
            {
               if( ret == aKeys.end() )
               {
                  ret = aKeys.insert( ret, I_categories->first );
               }
               else
               {
                  aKeys.push_back( I_categories->first );
               }
            }

            return ret;
         }
         
         TKeyListIterator CCategory::VariableList( TKeyList& aKeys )const
         {
            TKeyListIterator ret( aKeys.end() );

            TVariableSeq::const_iterator I_variables( _variables.begin() );

            if( aKeys.size() )
            {
               I_variables = _variables.find( aKeys.back() );

               if( I_variables != _variables.end() )
               {
                  ++I_variables;
               }
            }

            for( ; I_variables != _variables.end(); ++I_variables )
            {
               if( ret == aKeys.end() )
               {
                  ret = aKeys.insert( ret, I_variables->first );
               }
               else
               {
                  aKeys.push_back( I_variables->first );
               }
            }

            return ret;
         }

         /////////////////////////////////////////////////////////////////////////////////////////
         CCategoryParserDriver::CCategoryParserDriver( const std::string& aFile,
                                                       CLexScaner& aLexer,
                                                       const HCategory& aCategory,
                                                       const CParserDriverSynchronization& aSynchronization,
                                                       CParserErrorRepository& anErrorRepository ) :
            CParserDriver( aFile, aLexer ),
            _synchronization( aSynchronization ),
            _errorRepository( anErrorRepository )
         {
            _categoryStack.push( aCategory );
            
            _categoryBuffSize = 0;
         }

         CCategoryParserDriver::~CCategoryParserDriver()
         {
            CHECK_OBJECT_STATE( categoryStackSize() == 1 || isTerminate(), "Category stack is broken" );
         }

         bool CCategoryParserDriver::PushCategory( const TCategory& aCategory )
         {
            TRACE_FUN( Frequently, "CAbstractRegistryDriver::PushCategory" );

            if( _categoryBuffSize > 100 )
            {
               _synchronization.WaitForContinue();
               
               _categoryBuffSize = 0;
            }
            
            CMutexLocker locker( _synchronization.mutex() );

            bool ret( categoryStackSize() );

            if( ret )
            {
               _categoryStack.top()->Merge( *aCategory.second );
               
               if( aCategory.second->members()._categories.size() )
               {
                  _categoryStack.push( aCategory.second->members()._categories.begin()->second.second );
               }
            }

            _synchronization.dataRead();
            
            ++_categoryBuffSize;
            
            return ret;
         }

         bool CCategoryParserDriver::PopCategory()
         {
            TRACE_FUN( Frequently, "CAbstractRegistryDriver::PopCategory" );

            CMutexLocker locker( _synchronization.mutex() );

            bool ret( categoryStackSize() );

            if( ret )
            {
               _categoryStack.pop();
            }

            return ret;
         }

         void CCategoryParserDriver::RegisterError( const std::string& anError )const
         {
            _errorRepository.RegisterError( anError );

            CParserDriver::RegisterError( anError );
         }

         CCategoryParserDriver::TCategoryStack::size_type CCategoryParserDriver::categoryStackSize()const
         {
            CMutexLocker locker( _synchronization.mutex() );

            return _categoryStack.size();
         }
         
         bool CCategoryParserDriver::isTerminate()const
         {
            return _synchronization.isTerminate();
         }
      }
   }
}
