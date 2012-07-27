/*
 * ProtocolDiagnosticDataStorageEventChannel.cpp
 *
 *  Created on: 07.09.2009
 *      Author: mars
 */

#include "ProtocolDiagnosticDataStorageEventChannel.h"

CProtocolDiagnosticDataStorageEventChannel::CProtocolDiagnosticDataStorageEventChannel( QObject* aConsumer )
{
   connect( this, SIGNAL( packetReadEvent() ), aConsumer, SLOT( OnPacketRead() ) );
}

CProtocolDiagnosticDataStorageEventChannel::~CProtocolDiagnosticDataStorageEventChannel(){}

void CProtocolDiagnosticDataStorageEventChannel::packetRead()
{
   packetReadEvent();
}
