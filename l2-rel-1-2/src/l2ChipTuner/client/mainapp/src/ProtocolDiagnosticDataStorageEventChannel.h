/*
 * ProtocolDiagnosticDataStorageEventChannel.h
 *
 *  Created on: 07.09.2009
 *      Author: mars
 */

#ifndef PROTOCOLDIAGNOSTICDATASTORAGEEVENTCHANNEL_H_
#define PROTOCOLDIAGNOSTICDATASTORAGEEVENTCHANNEL_H_

#include <IProtocolDiagnosticDataStorageEventSupplier.h>

#include <QObject>

class CProtocolDiagnosticDataStorageEventChannel : public QObject,
                                                   public IProtocolDiagnosticDataStorageEventSupplier
{
   Q_OBJECT;
   
public:
   CProtocolDiagnosticDataStorageEventChannel( QObject* aConsumer );
   virtual ~CProtocolDiagnosticDataStorageEventChannel();
   
   virtual void packetRead();

signals:
   void packetReadEvent();
};

#endif /* PROTOCOLDIAGNOSTICDATASTORAGEEVENTCHANNEL_H_ */
