/*
 * UtRegistry.cpp
 *
 *  Created on: 28.07.2009
 *      Author: mars
 */

#include "UtRegistry.h"

#include <l2Trace.h>
#include <l2TextTreeRegistry.h>
#include <l2TextIniRegistry.h>
#include <l2Exception.h>
#include <l2Thread.h>

#include <iostream>

namespace l2max
{
   namespace ut
   {
      CUtRegistry::CUtRegistry()
      {
         TRACE_FUN( Routine, "CUtRegistry::CUtRegistry" );
      
         /*
          * Registry drivers UT
          * */
         l2max::CByteArray ba;
         ba.Resize( 0x100f );
         ba.Fill( 0xfe );
         
         reinterpret_cast< char* >( ba.data() )[ 0x0 ] = 0xba;
      
         std::pair< std::string, l2max::CVariant > registryTextIniData[] =
            {
             std::pair< std::string, l2max::CVariant >( "::integer", 69 ),
             std::pair< std::string, l2max::CVariant >( "::real", .8496 ),
             std::pair< std::string, l2max::CVariant >( "::string", "DeadBeef\nDeadBeefDeadBeef\nDeadBeefDeadBeefDeadBeef" ),
             std::pair< std::string, l2max::CVariant >( "::binary", ba ),
             std::pair< std::string, l2max::CVariant >( "::group1::integer", 69 ),
             std::pair< std::string, l2max::CVariant >( "::group1::real", .8496 ),
             std::pair< std::string, l2max::CVariant >( "::group1::string", "DeadBeef\nDeadBeefDeadBeef\nDeadBeefDeadBeefDeadBeef" ),
             std::pair< std::string, l2max::CVariant >( "::group1::binary", ba ),
             std::pair< std::string, l2max::CVariant >( "::group2::integer", 69 ),
             std::pair< std::string, l2max::CVariant >( "::group2::real", .8496 ),
             std::pair< std::string, l2max::CVariant >( "::group2::string", "DeadBeef\nDeadBeefDeadBeef\nDeadBeefDeadBeefDeadBeef" ),
             std::pair< std::string, l2max::CVariant >( "::group2::binary", ba ),
            };
      
         std::pair< std::string, l2max::CVariant > registryTextTreeData[] =
            {
             std::pair< std::string, l2max::CVariant >( "::global_integer", 69 ),
             std::pair< std::string, l2max::CVariant >( "::global_real", .8496 ),
             std::pair< std::string, l2max::CVariant >( "::global_string", "DeadBeef\nDeadBeefDeadBeef\nDeadBeefDeadBeefDeadBeef" ),
             std::pair< std::string, l2max::CVariant >( "::global_binary", ba ),
             std::pair< std::string, l2max::CVariant >( "::group1::integer", 69 ),
             std::pair< std::string, l2max::CVariant >( "::group1::real", .8496 ),
             std::pair< std::string, l2max::CVariant >( "::group1::string", "DeadBeef\nDeadBeefDeadBeef\nDeadBeefDeadBeefDeadBeef" ),
             std::pair< std::string, l2max::CVariant >( "::group1::binary", ba ),
             std::pair< std::string, l2max::CVariant >( "::group1::group2::integer", 69 ),
             std::pair< std::string, l2max::CVariant >( "::group1::group2::real", .8496 ),
             std::pair< std::string, l2max::CVariant >( "::group1::group2::string", "DeadBeef\nDeadBeefDeadBeef\nDeadBeefDeadBeefDeadBeef" ),
             std::pair< std::string, l2max::CVariant >( "::group1::group2::binary", ba ),
             std::pair< std::string, l2max::CVariant >( "::group2::integer", 69 ),
             std::pair< std::string, l2max::CVariant >( "::group2::real", .8496 ),
             std::pair< std::string, l2max::CVariant >( "::group2::string", "DeadBeef\nDeadBeefDeadBeef\nDeadBeefDeadBeefDeadBeef" ),
             std::pair< std::string, l2max::CVariant >( "::group2::binary", ba ),
            };
      
         try
         {
            std::cout << "Checking basic registry functionality" << std::endl;
            
            for( int i( 0 ); i < 3; ++i )
            {
               std::cout << "Iteration: " << i << std::endl;
         
               /*
                * Text ini driver UT
                * */
               l2max::CTextIniRegistry iniReg;
               if( !iniReg.Open( "RUTTextIni.ini" ) )
               {
                  throw l2max::CException( "Failed to open registry. " + iniReg.errorString() );
               }

               for( int i( 0 ); i < sizeof( registryTextIniData ) / sizeof( *registryTextIniData ); ++i )
               {
                  if( !iniReg.Write( registryTextIniData[ i ].first, registryTextIniData[ i ].second ) )
                  {
                     throw l2max::CException( "Failed to write text ini value" );
                  }
               }
         
               if( !iniReg.Close() )
               {
                  throw l2max::CException( "Failed to close text ini file" );
               }
         
               if( !iniReg.Open( "RUTTextIni.ini" ) )
               {
                  throw l2max::CException( "Failed to open text ini file" );
               }
         
               for( int i( 0 ); i < sizeof( registryTextIniData ) / sizeof( *registryTextIniData ); ++i )
               {
                  l2max::CVariant val;
                  if( !iniReg.Read( registryTextIniData[ i ].first, val ) )
                  {
                     throw l2max::CException( "Failed to read text ini value from storage" );
                  }
         
                  if( val != registryTextIniData[ i ].second )
                  {
                     throw l2max::CException( "\nFrom registry: \n" + val.toString() + "\n Text ini read value doesn't correspond to written" );
                  }
               }
         
               if( !iniReg.Close() )
               {
                  throw l2max::CException( "Failed to close text ini file" );
               }
         
         
               /*
                * Text tree driver UT
                * */
               l2max::CTextTreeRegistry treeReg;
               if( !treeReg.Open( "RUTTextTree.rg" ) )
               {
                  throw l2max::CException( "Failed to open registry. " + treeReg.errorString() );
               }

               for( int i( 0 ); i < sizeof( registryTextTreeData ) / sizeof( *registryTextTreeData ); ++i )
               {
                  if( !treeReg.Write( registryTextTreeData[ i ].first, registryTextTreeData[ i ].second ) )
                  {
                     throw l2max::CException( "Failed to write text tree value" );
                  }
               }
         
               if( !treeReg.Close() )
               {
                  throw l2max::CException( "Failed to close text tree file" );
               }
         
               if( !treeReg.Open( "RUTTextTree.rg" ) )
               {
                  throw l2max::CException( "Failed to open text tree file" );
               }
         
               for( int i( 0 ); i < sizeof( registryTextTreeData ) / sizeof( *registryTextTreeData ); ++i )
               {
                  l2max::CVariant val;
                  if( !treeReg.Read( registryTextTreeData[ i ].first, val ) )
                  {
                     throw l2max::CException( "Failed to read text tree value from storage" );
                  }
         
                  if( val != registryTextTreeData[ i ].second )
                  {
                     throw l2max::CException( "\nFrom registry: \n" + val.toString() + "\n Text tree read value doesn't correspond to written" );
                  }
               }
         
               if( !treeReg.Close() )
               {
                  throw l2max::CException( "Failed to close text tree file" );
               }
            }
            
            {
               std::cout << "Checking category iteration" << std::endl;
               
               l2max::CTextIniRegistry iniReg;
               iniReg.Open( "RUTTextIni.ini" );
               
               {
                  l2max::Registry::TKeyList keys;
                  l2max::Registry::TKeyListIterator I_keys;
                  
                  while( ( I_keys = iniReg.CategoryList( "::", keys ) ) != keys.end() )
                  {
                     for( ; I_keys != keys.end(); ++I_keys )
                     {
                        std::cout << "Category: " << *I_keys << std::endl;
                     }
                  }
               }
               
               {
                  l2max::Registry::TKeyList keys;
                  l2max::Registry::TKeyListIterator I_keys;
                  
                  while( ( I_keys = iniReg.VariableList( "::", keys ) ) != keys.end() )
                  {
                     for( ; I_keys != keys.end(); ++I_keys )
                     {
                        std::cout << "Variable: " << *I_keys << std::endl;
                     }
                  }
               }

               iniReg.Close();
            }
            
            {
               std::cout << "Checking category iteration" << std::endl;
               
               l2max::CTextTreeRegistry treeReg;
               treeReg.Open( "RUTTextTree.rg" );
               
               {
                  l2max::Registry::TKeyList keys;
                  l2max::Registry::TKeyListIterator I_keys;
                  
                  while( ( I_keys = treeReg.CategoryList( "::", keys ) ) != keys.end() )
                  {
                     for( ; I_keys != keys.end(); ++I_keys )
                     {
                        std::cout << "Category: " << *I_keys << std::endl;
                     }
                  }
               }
               
               {
                  l2max::Registry::TKeyList keys;
                  l2max::Registry::TKeyListIterator I_keys;
                  
                  while( ( I_keys = treeReg.VariableList( "::", keys ) ) != keys.end() )
                  {
                     for( ; I_keys != keys.end(); ++I_keys )
                     {
                        std::cout << "Variable: " << *I_keys << std::endl;
                     }
                  }
               }
               
               treeReg.Close();
            }
         }
         catch( const l2max::CException& e )
         {
            std::cout << e.what() << std::endl;
         }
         catch( ... )
         {
            TraceCritical << "Unknown Exception" << std::endl;
         }
      }
      
      CUtRegistry::~CUtRegistry()
      {
         TRACE_FUN( Routine, "CUtRegistry::~CUtRegistry" );
      }
   }
}
