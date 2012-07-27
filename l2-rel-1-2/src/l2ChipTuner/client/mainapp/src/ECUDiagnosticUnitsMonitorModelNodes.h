/*
 * ECUDiagnosticUnitsMonitorModelNodes.h
 *
 *  Created on: 26.11.2008
 *      Author: mars
 */

#ifndef ECUDIAGNOSTICUNITSMONITORMODELNODES_H_
#define ECUDIAGNOSTICUNITSMONITORMODELNODES_H_

#include "ECUDiagnosticUnitModelUtils.h"

#include <DiagnosticTable.h>
#include <ECUDataUnit.h>

#include <QVariant>

#include <string>
#include <vector>
#include <list>

/////////////////////////////////////////////////////////////////////////////////////////////////////////
class CECUDiagnosticUnitsMonitorModelNode
{
   typedef std::vector< CECUDiagnosticUnitsMonitorModelNode* > TECUDiagnosticUnitsMonitorModelNodeChildren;
   TECUDiagnosticUnitsMonitorModelNodeChildren _ecuDiagnosticUnitsManagerModelNodeChildren;

   typedef std::map< const CECUDiagnosticUnitsMonitorModelNode*, int > TECUDiagnosticUnitsMonitorModelNode2Index;
   TECUDiagnosticUnitsMonitorModelNode2Index _ecuDiagnosticUnitsMonitorModelNode2Index;

   CECUDiagnosticUnitsMonitorModelNode* _parent;

   CECUDiagnosticUnitsMonitorModelNode( const CECUDiagnosticUnitsMonitorModelNode& );
   CECUDiagnosticUnitsMonitorModelNode& operator = ( const CECUDiagnosticUnitsMonitorModelNode& );

   void RebuildChildIndexes();
   
public:
   enum EType
   {
      tRoot,
      tGroupUnit,
      tGroupUnitState,
      tUnit
   };

private:
   EType _type;

public:
   CECUDiagnosticUnitsMonitorModelNode( EType aType );
   virtual ~CECUDiagnosticUnitsMonitorModelNode();

   virtual const std::string& name()const = 0;
   virtual const std::string& description()const = 0;
   virtual const std::string& synonym()const=0;
   
   virtual std::string text()const = 0;
   
   virtual bool operator == ( const CECUDiagnosticUnitsMonitorModelNode& aNode )const;
   bool operator != ( const CECUDiagnosticUnitsMonitorModelNode& aNode )const;

   CECUDiagnosticUnitsMonitorModelNode* parent();
   CECUDiagnosticUnitsMonitorModelNode* parent()const;
   CECUDiagnosticUnitsMonitorModelNode* child( int anIndex );
   CECUDiagnosticUnitsMonitorModelNode* child( int anIndex )const;

   int index()const;
   int childCount()const;

   EType type()const;

   void AppendChild( CECUDiagnosticUnitsMonitorModelNode* aChild );
   void Insert( CECUDiagnosticUnitsMonitorModelNode* aChild, int aWhere = -1 );
   void Remove( int anIndex, int aCount = 1 );
   void Remove( CECUDiagnosticUnitsMonitorModelNode* aNode );
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////
class CECUDiagnosticUnitsMonitorModelRootNode : public CECUDiagnosticUnitsMonitorModelNode
{
public:
   CECUDiagnosticUnitsMonitorModelRootNode( const ECU::TDataUnitContainer& aContainer );

   virtual const std::string& name()const;
   virtual const std::string& description()const;
   virtual const std::string& synonym()const;
   
   virtual std::string text()const;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////
class CECUDiagnosticUnitsMonitorModelGroupUnitNode : public CECUDiagnosticUnitsMonitorModelNode
{
   std::string _name;

   Diagnostic::EGroupType _groupType;

public:
   CECUDiagnosticUnitsMonitorModelGroupUnitNode( Diagnostic::EGroupType aType, const ECU::TDataUnitContainer& aContainer );

   virtual const std::string& name()const;
   virtual const std::string& description()const;
   virtual const std::string& synonym()const;
   
   virtual std::string text()const;

   Diagnostic::EGroupType groupType()const;

   Diagnostic::TUnitKeys keys()const;
   
   virtual bool operator == ( const CECUDiagnosticUnitsMonitorModelNode& aNode )const;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////
class CECUDiagnosticUnitsMonitorModelGroupUnitStateNode : public CECUDiagnosticUnitsMonitorModelNode
{
   std::string _name;

public:
   CECUDiagnosticUnitsMonitorModelGroupUnitStateNode( const ECU::TDataUnitContainer& aContainer );

   virtual const std::string& name()const;
   virtual const std::string& description()const;
   virtual const std::string& synonym()const;

   virtual std::string text()const;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace ECU
{
   class CDataUnit;
}

class CECUDiagnosticUnitsMonitorModelUnitNode : public CECUDiagnosticUnitsMonitorModelNode
{
   ECU::HDataUnit _unit;

public:
   CECUDiagnosticUnitsMonitorModelUnitNode( ECU::HDataUnit anUnit );

   virtual const std::string& name()const;
   virtual const std::string& description()const;
   virtual const std::string& synonym()const;
   
   virtual std::string text()const;

   Diagnostic::CUnitKey key()const;
   
   virtual bool operator == ( const CECUDiagnosticUnitsMonitorModelNode& aNode )const;
};

#endif /* ECUDIAGNOSTICUNITSMONITORMODELNODES_H_*/
