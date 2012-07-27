/*
 * ProtocolCreationStrategy.cpp
 *
 *  Created on: 25.03.2009
 *      Author: L2-Max
 */

#include "ProtocolCreationStrategy.h"

#include <AppDefs.h>
#include <AppContext.h>

//////////////////////////////////////////////////////////////////////////////////
CAbstractProtocolCreationStrategy::CAbstractProtocolCreationStrategy( EType aType ) : _type( aType ){}
CAbstractProtocolCreationStrategy::~CAbstractProtocolCreationStrategy(){}

CAbstractProtocolCreationStrategy::EType CAbstractProtocolCreationStrategy::type()const
{
   return _type;
}

Protocol::EProtocolType CAbstractProtocolCreationStrategy::protocolType()const
{
   return _protocolType;
}

//////////////////////////////////////////////////////////////////////////////////
CRegistryProtocolCreationStrategy::CRegistryProtocolCreationStrategy() :
   CAbstractProtocolCreationStrategy( CAbstractProtocolCreationStrategy::tRegistry )
{
   l2max::CVariant theType;
   CAppContext::GetInstance()->MainRegistry().Read( V_ECU_PROTO_TYPE, theType );

   _protocolType = static_cast< Protocol::EProtocolType >( theType.toInt() );
}

CRegistryProtocolCreationStrategy::~CRegistryProtocolCreationStrategy(){}

const l2max::CRegistry& CRegistryProtocolCreationStrategy::regstry()const
{
   return CAppContext::GetInstance()->MainRegistry();
}

//////////////////////////////////////////////////////////////////////////////////
CFileProtocolCreationStrategy::CFileProtocolCreationStrategy() : CAbstractProtocolCreationStrategy( CAbstractProtocolCreationStrategy::tFile )
{
   l2max::CVariant dataDir;
   CAppContext::GetInstance()->MainRegistry().Read( V_ECU_DIAG_SESS_DATADIR, dataDir );

   l2max::CVariant readFromFile;
   CAppContext::GetInstance()->MainRegistry().Read( V_ECU_DIAG_SESS_DATAFILER, readFromFile );

   _registry.Open( dataDir.toString() + "/" + readFromFile.toString() );

   l2max::CVariant theType;
   _registry.Read( V_DIAGF_CONF_PROTOTYPE, theType );

   _protocolType = static_cast< Protocol::EProtocolType >( theType.toInt() );
}

CFileProtocolCreationStrategy::~CFileProtocolCreationStrategy(){}

const l2max::CRegistry& CFileProtocolCreationStrategy::regstry()const
{
   return _registry;
}

//////////////////////////////////////////////////////////////////////////////////
CALDLProtocolCreationStrategy::CALDLProtocolCreationStrategy( CAbstractProtocolCreationStrategy* aStrategy ) :
   _strategy( aStrategy ){}

CALDLProtocolCreationStrategy::~CALDLProtocolCreationStrategy()
{
   delete _strategy;
}

const l2max::CRegistry& CALDLProtocolCreationStrategy::registry()const
{
   return _strategy->regstry();
}

Protocol::ALDL::EECUId CALDLProtocolCreationStrategy::ecuId()const
{
   return _ecuId;
}

CAbstractProtocolCreationStrategy::EType CALDLProtocolCreationStrategy::type()const
{
   return _strategy->type();
}

//////////////////////////////////////////////////////////////////////////////////
CALDLRegistryProtocolCreationStrategy::CALDLRegistryProtocolCreationStrategy( CRegistryProtocolCreationStrategy* aStrategy ) :
   CALDLProtocolCreationStrategy( aStrategy )
{
   l2max::CVariant theEcuId;

   registry().Read( V_ECU_PROTO_ALDL_ECUID, theEcuId );

   _ecuId =  static_cast< Protocol::ALDL::EECUId >( theEcuId.toInt() );
}

CALDLRegistryProtocolCreationStrategy::~CALDLRegistryProtocolCreationStrategy(){}

//////////////////////////////////////////////////////////////////////////////////
CALDLFileProtocolCreationStrategy::CALDLFileProtocolCreationStrategy( CFileProtocolCreationStrategy* aStrategy ) :
   CALDLProtocolCreationStrategy( aStrategy )
{
   l2max::CVariant theEcuId;

   registry().Read( V_DIAGF_CONF_ALDLECUID, theEcuId );

   _ecuId =  static_cast< Protocol::ALDL::EECUId >( theEcuId.toInt() );
}

CALDLFileProtocolCreationStrategy::~CALDLFileProtocolCreationStrategy(){}
