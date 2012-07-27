/*
 * AbstractProto.cpp
 *
 *  Created on: 05.10.2008
 *      Author: L2-Max
 */

#include "AbstractProto.h"

#include <l2AbstractIODevice.h>

#include <memory.h>

CAbstractProto::CAbstractProto( CAbstractAdapter& anAdapter, Protocol::EProtocolType aType ) :
   _adapter( anAdapter ),
   _type( aType ){}

CAbstractProto::~CAbstractProto(){}

CAbstractProto::EWriteResultCode CAbstractProto::writeResultCode()const
{
   return _writeResultCode;
}

Protocol::EProtocolType CAbstractProto::type()const
{
   return _type;
}

const std::string& CAbstractProto::registryNamespace()const
{
   return _namespace;
}
