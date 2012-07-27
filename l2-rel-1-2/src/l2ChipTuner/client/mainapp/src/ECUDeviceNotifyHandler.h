/*
 * ECUDeviceNotifyHandler.h
 *
 *  Created on: 07.10.2008
 *      Author: mars
 */

#ifndef ECUDEVICENOTIFYHANDLER_H_
#define ECUDEVICENOTIFYHANDLER_H_

#include <ECUDeviceNotify.h>

#include <QObject>

class CECUDeviceNotifyHandler : public QObject,
                                public ECU::IDeviceNotify
{
   Q_OBJECT;

   virtual void diagnosticSessionStarted();
   virtual void diagnosticSessionStoped();
   virtual void dataRead();
   virtual void dataWriten( CAbstractProto::EWriteResultCode aCode );
   virtual void stopped();
   virtual void statUpdated();

public:
   CECUDeviceNotifyHandler( QObject* aSuccessor );
   virtual ~CECUDeviceNotifyHandler();

signals:
   void diagnosticSessionStartedEvent();
   void diagnosticSessionStopedEvent();
   void dataReadEvent();
   void dataWritenEvent( int aCode );
   void stoppedEvent();
   void statUpdatedEvent();
};

#endif /* ECUSCANERNOTIFYHANDLER_H_ */
