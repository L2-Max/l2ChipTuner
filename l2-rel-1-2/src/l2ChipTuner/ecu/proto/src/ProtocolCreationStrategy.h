/*
 * ProtocolCreationStrategy.h
 *
 *  Created on: 25.03.2009
 *      Author: L2-Max
 */

#ifndef PROTOCOLCREATIONSTRATEGY_H_
#define PROTOCOLCREATIONSTRATEGY_H_

#include <ProtocolTable.h>
#include <VehicleTable.h>

#include <l2TextTreeRegistry.h>

//////////////////////////////////////////////////////
class CAbstractProtocolCreationStrategy
{
protected:
   Protocol::EProtocolType _protocolType;

public:
   enum EType
   {
      tRegistry,
      tFile
   };

private:
   EType _type;

public:
   CAbstractProtocolCreationStrategy( EType aType );
   virtual ~CAbstractProtocolCreationStrategy() = 0;

   virtual const l2max::CRegistry& regstry()const = 0;

   EType type()const;
   Protocol::EProtocolType protocolType()const;
};

//////////////////////////////////////////////////////
class CRegistryProtocolCreationStrategy : public CAbstractProtocolCreationStrategy
{
public:
   CRegistryProtocolCreationStrategy();
   virtual ~CRegistryProtocolCreationStrategy();

   virtual const l2max::CRegistry& regstry()const;
};

//////////////////////////////////////////////////////
class CFileProtocolCreationStrategy : public CAbstractProtocolCreationStrategy
{
   l2max::CTextTreeRegistry _registry;

public:
   CFileProtocolCreationStrategy();
   virtual ~CFileProtocolCreationStrategy();

   virtual const l2max::CRegistry& regstry()const;
};

//////////////////////////////////////////////////////
class CALDLProtocolCreationStrategy
{
   CAbstractProtocolCreationStrategy* _strategy;

protected:
   Protocol::ALDL::EECUId _ecuId;

public:
   CALDLProtocolCreationStrategy( CAbstractProtocolCreationStrategy* aStrategy );
   virtual ~CALDLProtocolCreationStrategy();

   const l2max::CRegistry& registry()const;

   Protocol::ALDL::EECUId ecuId()const;

   CAbstractProtocolCreationStrategy::EType type()const;
};

//////////////////////////////////////////////////////
class CALDLRegistryProtocolCreationStrategy : public CALDLProtocolCreationStrategy
{
public:
   CALDLRegistryProtocolCreationStrategy( CRegistryProtocolCreationStrategy* aStrategy );
   virtual ~CALDLRegistryProtocolCreationStrategy();
};

//////////////////////////////////////////////////////
class CALDLFileProtocolCreationStrategy : public CALDLProtocolCreationStrategy
{
public:
   CALDLFileProtocolCreationStrategy( CFileProtocolCreationStrategy* aStrategy );
   virtual ~CALDLFileProtocolCreationStrategy();
};

#endif /* PROTOCOLCREATIONSTRATEGY_H_ */
