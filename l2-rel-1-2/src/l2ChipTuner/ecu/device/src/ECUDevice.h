/*
 * ECUDevice.h
 *
 *  Created on: 05.10.2008
 *      Author: L2-Max
 */

#ifndef ECUDEVICE_H_
#define ECUDEVICE_H_

#include "ECURequestQueue.h"
#include "ECUDeviceReadDataTimer.h"

#include <l2Thread.h>
#include <l2Mutex.h>
#include <l2WaitCondition.h>

#include <map>

class CAbstractProto;
class CAbstractAdapter;
class CAbstractSensor;

namespace l2max
{
   struct IIODevice;
}

namespace ECU
{
   struct IDeviceNotify;

   class CUnitsTable;
   class CErrorsTable;

   namespace FlowMeter
   {
      class CDevice;
   }

   class CDevice : public l2max::CThread
   {
      static unsigned char _msgProcessAttemptsCount;
      static unsigned char _initialRequestQueueSize;

      bool _isAutoAdjustQueue;

      unsigned int _requestQueueTime;

      l2max::CMutex _mutex;
      l2max::CWaitCondition _stopCondition;

      CDeviceReadDataTimer _readDataTimer;

      CUnitsTable* _diagnosticUnitsTable;
      CErrorsTable* _diagnosticErrorsTable;
      
      FlowMeter::CDevice* _flowMeterDevice;

      bool AdjustReadDataTimeo();
      bool ProcessRequest( CRequest::EId anId );

      void AdjustRequestQueue();
      void StopTimers();

   public:
      enum EErrorCode
      {
         ecNoError,
         ecAdapterError,
         ecProtoError,
         ecDeviceError
      };

   private:
      CRequestQueue _requestQueue;

      IDeviceNotify* _ecuNotify;

      CAbstractAdapter* _adapter;
      CAbstractProto* _proto;

      EErrorCode _errorCode;

      typedef std::map< EErrorCode, std::string > TErrorText;
      mutable TErrorText _errorText;

      bool _isInitialized;

      virtual unsigned int run();

   public:
      CDevice( IDeviceNotify* aNotify );
      virtual ~CDevice();

      bool Initialize();
      void UnInitialize();

      bool StartDiagnosticSession();
      void StopDiagnosticSession();
      void ReadDiagnosticData();
      void TransmitData();

      void SetPeriodicTransmissionEnabled( bool anEnable );
      bool IsPeriodicTransmissionEnabled()const;

      CUnitsTable& unitsTable();
      CErrorsTable& errorsTable();

      EErrorCode errorCode()const;
      std::string errorText()const;

      bool isInitialized()const;

      FlowMeter::CDevice* flowMeter();
      
      CAbstractProto* protocol();
   };
}

#endif /* ECUDEVICE_H_ */
