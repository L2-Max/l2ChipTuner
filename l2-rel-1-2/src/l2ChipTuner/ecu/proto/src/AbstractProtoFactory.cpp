/*
 * AbstractProtoFactory.cpp
 *
 *  Created on: 25.03.2009
 *      Author: L2-Max
 */

#include "AbstractProtoFactory.h"
#include "ProtocolCreationStrategy.h"
#include "ALDLProto.h"
#include "KWP2kProto.h"

#include <AppDefs.h>
#include <AppContext.h>

#include <l2ExceptionThrower.h>

CAbstractProtoFactory::CAbstractProtoFactory(){}
CAbstractProtoFactory::~CAbstractProtoFactory(){}

CAbstractProto* CAbstractProtoFactory::CreateProtocol( CAbstractAdapter& anAdapter )
{
   CAbstractProto* ret( 0 );

   l2max::CVariant theType;

   l2max::CVariant isReadFromFile;
   CAppContext::GetInstance()->MainRegistry().Read( V_ECU_DIAG_SESS_DATAFILER_E, isReadFromFile );

   CAbstractProtocolCreationStrategy* strategy( 0 );

   if( isReadFromFile.toInt() )
   {
      strategy = new CFileProtocolCreationStrategy();
   }
   else
   {
      strategy = new CRegistryProtocolCreationStrategy();
   }

   if( strategy->protocolType() == Protocol::ptALDL )
   {
      CALDLProtocolCreationStrategy* aldlStrategy( 0 );

      if( strategy->type() == CAbstractProtocolCreationStrategy::tFile )
      {
         aldlStrategy = new CALDLFileProtocolCreationStrategy( static_cast< CFileProtocolCreationStrategy* >( strategy ) );
      }
      else if( strategy->type() == CAbstractProtocolCreationStrategy::tRegistry )
      {
         aldlStrategy = new CALDLRegistryProtocolCreationStrategy( static_cast< CRegistryProtocolCreationStrategy* >( strategy ) );
      }

      ret = new ALDL::CProto( anAdapter, aldlStrategy );
   }
   else if( strategy->protocolType() == Protocol::ptKWP2k )
   {
      ret = new KWP2k::CProto( anAdapter );
   }

   return ret;
}
