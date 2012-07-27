/*
 * ECUDiagnosticUnitsMonitorModelNodes.cpp
 *
 *  Created on: 26.11.2008
 *      Author: mars
 */

#include "ECUDiagnosticUnitsMonitorModelNodes.h"

#include <ECUDataUnit.h>

#include <l2Trace.h>
#include <l2ExceptionThrower.h>
#include <l2Variant.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////
CECUDiagnosticUnitsMonitorModelNode::CECUDiagnosticUnitsMonitorModelNode( EType aType )
{
   _parent = 0;
   _type = aType;
}

CECUDiagnosticUnitsMonitorModelNode::~CECUDiagnosticUnitsMonitorModelNode()
{
   Remove( 0, childCount() );
}

CECUDiagnosticUnitsMonitorModelNode* CECUDiagnosticUnitsMonitorModelNode::parent()
{
   return _parent;
}

CECUDiagnosticUnitsMonitorModelNode* CECUDiagnosticUnitsMonitorModelNode::parent()const
{
   return _parent;
}

CECUDiagnosticUnitsMonitorModelNode* CECUDiagnosticUnitsMonitorModelNode::child( int anIndex )
{
   return _ecuDiagnosticUnitsManagerModelNodeChildren[ anIndex ];
}

CECUDiagnosticUnitsMonitorModelNode* CECUDiagnosticUnitsMonitorModelNode::child( int anIndex )const
{
   return _ecuDiagnosticUnitsManagerModelNodeChildren[ anIndex ];
}

void CECUDiagnosticUnitsMonitorModelNode::AppendChild( CECUDiagnosticUnitsMonitorModelNode* aChild )
{
   Insert( aChild );
}

void CECUDiagnosticUnitsMonitorModelNode::Insert( CECUDiagnosticUnitsMonitorModelNode* aChild, int aWhere )
{
   if( aWhere == -1 )
   {
      aWhere = childCount();
   }

   CHECK_OBJECT_STATE( aWhere <= childCount(), "Index is out of bounds" );
   
   if( aChild->parent() )
   {
      aChild->parent()->Remove( aChild );
   }
   
   aChild->_parent = this;

   _ecuDiagnosticUnitsManagerModelNodeChildren.insert( _ecuDiagnosticUnitsManagerModelNodeChildren.begin() + aWhere, aChild );

   RebuildChildIndexes();
}

int CECUDiagnosticUnitsMonitorModelNode::index()const
{
   TRACE_FUN( Frequently, "CECUDiagnosticUnitsMonitorModelNode::index" );

   int ret( -1 );

   CHECK_OBJECT_STATE( _parent, "Child index has no parent" );

   TECUDiagnosticUnitsMonitorModelNode2Index::const_iterator i( _parent->_ecuDiagnosticUnitsMonitorModelNode2Index.find( this ) );

   if( i != _parent->_ecuDiagnosticUnitsMonitorModelNode2Index.end() )
   {
      ret = i->second;
   }

   CHECK_OBJECT_STATE( ret != -1, "Child index is not found in own parent" );

   return ret;
}

int CECUDiagnosticUnitsMonitorModelNode::childCount()const
{
   return _ecuDiagnosticUnitsManagerModelNodeChildren.size();
}

CECUDiagnosticUnitsMonitorModelNode::EType CECUDiagnosticUnitsMonitorModelNode::type()const
{
   return _type;
}

void CECUDiagnosticUnitsMonitorModelNode::Remove( int anIndex, int aCount )
{
   TRACE_FUN( Frequently, "CECUDiagnosticUnitsMonitorModelNode::Remove" );

   for( TECUDiagnosticUnitsMonitorModelNodeChildren::iterator i( _ecuDiagnosticUnitsManagerModelNodeChildren.begin() + anIndex );
        i != _ecuDiagnosticUnitsManagerModelNodeChildren.end() && aCount; --aCount )
   {
      delete *i;

      i = _ecuDiagnosticUnitsManagerModelNodeChildren.erase( i );
   }

   RebuildChildIndexes();
}

void CECUDiagnosticUnitsMonitorModelNode::Remove( CECUDiagnosticUnitsMonitorModelNode* aNode )
{
   for( TECUDiagnosticUnitsMonitorModelNodeChildren::iterator I_child( _ecuDiagnosticUnitsManagerModelNodeChildren.begin() );
         I_child != _ecuDiagnosticUnitsManagerModelNodeChildren.end(); ++I_child )
   {
      if( aNode == *I_child )
      {
         _ecuDiagnosticUnitsManagerModelNodeChildren.erase( I_child );
         
         aNode->_parent = 0;
         
         break;
      }
   }
   
   RebuildChildIndexes();
}

void CECUDiagnosticUnitsMonitorModelNode::RebuildChildIndexes()
{
   _ecuDiagnosticUnitsMonitorModelNode2Index.clear();

   for( TECUDiagnosticUnitsMonitorModelNodeChildren::iterator i( _ecuDiagnosticUnitsManagerModelNodeChildren.begin() );
        i != _ecuDiagnosticUnitsManagerModelNodeChildren.end(); ++i )
   {
      _ecuDiagnosticUnitsMonitorModelNode2Index[ *i ] = i - _ecuDiagnosticUnitsManagerModelNodeChildren.begin();
   }
}

bool CECUDiagnosticUnitsMonitorModelNode::operator == ( const CECUDiagnosticUnitsMonitorModelNode& aNode )const
{
   return type() == aNode.type();
}

bool CECUDiagnosticUnitsMonitorModelNode::operator != ( const CECUDiagnosticUnitsMonitorModelNode& aNode )const
{
   return !operator == ( aNode );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
CECUDiagnosticUnitsMonitorModelRootNode::CECUDiagnosticUnitsMonitorModelRootNode( const ECU::TDataUnitContainer& aContainer ) :
   CECUDiagnosticUnitsMonitorModelNode( tRoot )
{
   typedef std::map< Diagnostic::EGroupType, ECU::TDataUnitContainer > TGroupType2UnitContainer;
   TGroupType2UnitContainer groupType2UnitContainer;
   
   for( ECU::TDataUnitContainer::const_iterator I_unit( aContainer.begin() ); I_unit != aContainer.end(); ++I_unit )
   {
      groupType2UnitContainer[ Diagnostic::unitKey2GroupMap()->find( ( *I_unit )->key() )->second ].push_back( *I_unit );
   }
   
   for( TGroupType2UnitContainer::const_iterator I_group( groupType2UnitContainer.begin() );
         I_group != groupType2UnitContainer.end(); ++I_group )
   {
      AppendChild( new CECUDiagnosticUnitsMonitorModelGroupUnitNode( I_group->first, I_group->second ) );
   }
}

const std::string& CECUDiagnosticUnitsMonitorModelRootNode::name()const
{
   static std::string ret( "rootNodeName" );
   
   return ret;
}

std::string CECUDiagnosticUnitsMonitorModelRootNode::text()const
{
   return std::string( "rootNodeText" );
}

const std::string& CECUDiagnosticUnitsMonitorModelRootNode::description()const
{
   static std::string ret( "rootNodeDescription" );
   
   return ret;
}

const std::string& CECUDiagnosticUnitsMonitorModelRootNode::synonym()const
{
   static std::string ret( "root" );
   
   return ret;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
CECUDiagnosticUnitsMonitorModelGroupUnitNode::CECUDiagnosticUnitsMonitorModelGroupUnitNode( Diagnostic::EGroupType aType,
                                                                                            const ECU::TDataUnitContainer& aContainer ) :
   CECUDiagnosticUnitsMonitorModelNode( tGroupUnit )
{
   _groupType = aType;

   _name = Diagnostic::groupDescriptionMap()->find( _groupType )->second._name;
   
   
   ECU::TDataUnitContainer unitStateContainer;
   
   for( ECU::TDataUnitContainer::const_iterator I_unit( aContainer.begin() ); I_unit != aContainer.end(); ++I_unit )
   {
      if( ( *I_unit )->value( Diagnostic::uBinary ).isValid() )
      {
         unitStateContainer.push_back( *I_unit );
      }
      else
      {
         AppendChild( new CECUDiagnosticUnitsMonitorModelUnitNode( *I_unit ) );
      }
   }
   
   if( unitStateContainer.size() )
   {
      AppendChild( new CECUDiagnosticUnitsMonitorModelGroupUnitStateNode( unitStateContainer ) );
   }
}

const std::string& CECUDiagnosticUnitsMonitorModelGroupUnitNode::name()const
{
   return _name;
}

std::string CECUDiagnosticUnitsMonitorModelGroupUnitNode::text()const
{
   return std::string();
}

const std::string& CECUDiagnosticUnitsMonitorModelGroupUnitNode::description()const
{
   static std::string ret;
   
   return ret;
}

const std::string& CECUDiagnosticUnitsMonitorModelGroupUnitNode::synonym()const
{
   static std::string ret;
   
   return ret;
}

Diagnostic::EGroupType CECUDiagnosticUnitsMonitorModelGroupUnitNode::groupType()const
{
   return _groupType;
}

Diagnostic::TUnitKeys CECUDiagnosticUnitsMonitorModelGroupUnitNode::keys()const
{
   Diagnostic::TUnitKeys ret;

   for( int i( 0 ); i < childCount(); ++i )
   {
      ret.insert( static_cast< CECUDiagnosticUnitsMonitorModelUnitNode* >( child( i ) )->key() );
   }

   return ret;
}

bool CECUDiagnosticUnitsMonitorModelGroupUnitNode::operator == ( const CECUDiagnosticUnitsMonitorModelNode& aNode )const
{
   bool ret( CECUDiagnosticUnitsMonitorModelNode::operator == ( aNode ) );
   
   if( ret )
   {
      ret = static_cast< const CECUDiagnosticUnitsMonitorModelGroupUnitNode& >( aNode )._groupType == _groupType;
   }
   
   return ret;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
CECUDiagnosticUnitsMonitorModelGroupUnitStateNode::CECUDiagnosticUnitsMonitorModelGroupUnitStateNode( const ECU::TDataUnitContainer& aContainer ) :
   CECUDiagnosticUnitsMonitorModelNode( tGroupUnitState )
{
   _name = "State Units";
   
   for( ECU::TDataUnitContainer::const_iterator I_unit( aContainer.begin() ); I_unit != aContainer.end(); ++I_unit )
   {
      AppendChild( new CECUDiagnosticUnitsMonitorModelUnitNode( *I_unit ) );
   }
}

const std::string& CECUDiagnosticUnitsMonitorModelGroupUnitStateNode::name()const
{
   return _name;
}

std::string CECUDiagnosticUnitsMonitorModelGroupUnitStateNode::text()const
{
   return std::string();
}

const std::string& CECUDiagnosticUnitsMonitorModelGroupUnitStateNode::description()const
{
   static std::string ret;
   
   return ret;
}

const std::string& CECUDiagnosticUnitsMonitorModelGroupUnitStateNode::synonym()const
{
   static std::string ret;
   
   return ret;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
CECUDiagnosticUnitsMonitorModelUnitNode::CECUDiagnosticUnitsMonitorModelUnitNode( ECU::HDataUnit anUnit ) :
   CECUDiagnosticUnitsMonitorModelNode( tUnit )
{
   _unit = anUnit;
}

const std::string& CECUDiagnosticUnitsMonitorModelUnitNode::name()const
{
   return _unit->name();
}

std::string CECUDiagnosticUnitsMonitorModelUnitNode::text()const
{
   return _unit->text();
}

const std::string& CECUDiagnosticUnitsMonitorModelUnitNode::description()const
{
   return _unit->description();
}

const std::string& CECUDiagnosticUnitsMonitorModelUnitNode::synonym()const
{
   return _unit->synonym();
}

Diagnostic::CUnitKey CECUDiagnosticUnitsMonitorModelUnitNode::key()const
{
   return _unit->key();
}

bool CECUDiagnosticUnitsMonitorModelUnitNode::operator == ( const CECUDiagnosticUnitsMonitorModelNode& aNode )const
{
   bool ret( CECUDiagnosticUnitsMonitorModelNode::operator == ( aNode ) );
   
   if( ret )
   {
      ret = static_cast< const CECUDiagnosticUnitsMonitorModelUnitNode& >( aNode )._unit == _unit;
   }
   
   return ret;
}
