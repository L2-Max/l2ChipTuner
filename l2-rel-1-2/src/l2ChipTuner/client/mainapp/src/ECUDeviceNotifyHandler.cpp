/*
 * ECUDeviceNotifyHandler.cpp
 *
 *  Created on: 07.10.2008
 *      Author: mars
 */

#include "ECUDeviceNotifyHandler.h"

CECUDeviceNotifyHandler::CECUDeviceNotifyHandler( QObject* aSuccessor )
{
   connect( this, SIGNAL( diagnosticSessionStartedEvent() ), aSuccessor, SLOT( OnDiagnosticSessionStarted() ) );
   connect( this, SIGNAL( diagnosticSessionStopedEvent() ), aSuccessor, SLOT( OnDiagnosticSessionStoped() ) );
   connect( this, SIGNAL( dataReadEvent() ), aSuccessor, SLOT( OnDataRead() ) );
   connect( this, SIGNAL( dataWritenEvent( int ) ), aSuccessor, SLOT( OnDataWriten( int ) ) );
   connect( this, SIGNAL( stoppedEvent() ), aSuccessor, SLOT( OnStopped() ) );
   connect( this, SIGNAL( statUpdatedEvent() ), aSuccessor, SLOT( OnStatUpdated() ) );
}

CECUDeviceNotifyHandler::~CECUDeviceNotifyHandler()
{
}

void CECUDeviceNotifyHandler::diagnosticSessionStarted()
{
   diagnosticSessionStartedEvent();
}

void CECUDeviceNotifyHandler::diagnosticSessionStoped()
{
   diagnosticSessionStopedEvent();
}

void CECUDeviceNotifyHandler::dataRead()
{
   dataReadEvent();
}

void CECUDeviceNotifyHandler::dataWriten( CAbstractProto::EWriteResultCode aCode )
{
   dataWritenEvent( aCode );
}

void CECUDeviceNotifyHandler::stopped()
{
   stoppedEvent();
}

void CECUDeviceNotifyHandler::statUpdated()
{
   statUpdatedEvent();
}
