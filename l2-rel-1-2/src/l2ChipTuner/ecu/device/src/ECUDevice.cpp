/*
 * ECUDevice.cpp
 *
 *  Created on: 05.10.2008
 *      Author: L2-Max
 */
#include "ECUDevice.h"
#include "ECUDeviceNotify.h"
#include "ECUFlowMeterDevice.h"
#include "ECUDataUnit.h"
#include "ECUUnitsTable.h"
#include "ECUErrorsTable.h"
#include "ECUDeviceStatistic.h"
#include "KLineAdapter.h"

#include <AppDefs.h>
#include <AppContext.h>
#include <AbstractProto.h>
#include <AbstractProtoFactory.h>
#include <DiagnosticDataAdapter.h>
#include <DiagnosticControlAdapter.h>
#include <DiagnosticErrorAdapter.h>

#include <l2Trace.h>
#include <l2ByteArray.h>
#include <l2MutexLocker.h>

namespace ECU
{
   unsigned char CDevice::_msgProcessAttemptsCount( 5 );
   unsigned char CDevice::_initialRequestQueueSize( 5 );

   CDevice::CDevice( IDeviceNotify* aNotify ) : _readDataTimer( *this )
   {
      TRACE_FUN( Routine, "CDevice::CDevice" );

      _errorText[ ecNoError ] = "No error";
      _errorText[ ecAdapterError ] = "Adapter error: ";
      _errorText[ ecProtoError ] = "Protocol error: ";
      _errorText[ ecDeviceError ] = "Device error";

      _ecuNotify = aNotify;
      _errorCode = ecNoError;

      _isInitialized = false;

      _diagnosticUnitsTable = 0;
      _diagnosticErrorsTable = 0;

      _proto = 0;
      _adapter = 0;

      _flowMeterDevice = 0;
   }

   CDevice::~CDevice()
   {
      TRACE_FUN( Routine, "CDevice::~CDevice" );

      UnInitialize();
   }

   bool CDevice::Initialize()
   {
      TRACE_FUN( Routine, "CDevice::Initialize" );

      if( !isInitialized() && !isRun() )
      {
         _errorCode = ecNoError;

         l2max::CVariant val;

         CAppContext::GetInstance()->MainRegistry().Read( V_ECU_ADAPT_ADAPTYPE, val );

         CAbstractAdapter::EAdapterType adapterType( static_cast< CAbstractAdapter::EAdapterType >( val.toInt() ) );

         if( adapterType == CAbstractAdapter::atKLine )
         {
            _adapter = new CKLineAdapter();
         }

         _proto = CAbstractProtoFactory::CreateProtocol( *_adapter );

         CAppContext::GetInstance()->MainRegistry().Read( V_ECU_DIAG_SESS_REQAUTO, val );
         _isAutoAdjustQueue = val.toBool();

         CAppContext::GetInstance()->MainRegistry().Read( V_ECU_DIAG_SESS_REQQUEUE, val );
         _requestQueueTime = val.toInt();

         _stopCondition.reset();

         _isInitialized = ( _errorCode == ecNoError );
      }

      return isInitialized();
   }

   void CDevice::UnInitialize()
   {
      TRACE_FUN( Routine, "CDevice::UnInitialize" );

      if( isRun() )
      {
         _stopCondition.wake();

         _requestQueue.push( CRequest::iIdle );

         Wait();
      }

      if( isInitialized() )
      {
         delete _flowMeterDevice;
         _flowMeterDevice = 0;
         
         delete _diagnosticUnitsTable;
         _diagnosticUnitsTable = 0;

         delete _diagnosticErrorsTable;
         _diagnosticErrorsTable = 0;

         delete _proto;
         _proto = 0;

         delete _adapter;
         _adapter = 0;

         _isInitialized = false;
      }
   }

   unsigned int CDevice::run()
   {
      TRACE_FUN( Routine, "CDevice::run" );

      unsigned int ret( 0 );
      unsigned int retryCount( CDevice::_msgProcessAttemptsCount );

      CRequest req;

      try
      {
         while( retryCount && !_stopCondition.waitForEvent( 0 ) )
         {
            bool msgOk( false );

            _adapter->ResetError();
            _proto->ResetError();

            CDeviceStatistic::GetInstance()->SetReqQueueSize( _requestQueue.size() );

            if( _requestQueue.front( req ) )
            {
               CDeviceStatistic::GetInstance()->IncReqCount();

               if( msgOk = ProcessRequest( req.id() ) )
               {
                  _requestQueue.pop();
               }
            }

            if( msgOk )
            {
               if( req.id() == CRequest::iStopDiagnosticSession )
               {
                  break;
               }

               retryCount = CDevice::_msgProcessAttemptsCount;
            }
            else
            {
               CDeviceStatistic::GetInstance()->IncReqErrorCount();

               --retryCount;
            }

            _ecuNotify->statUpdated();
         }

         if( !retryCount )
         {
            TraceRoutine << "CDevice::run: Attempts to process message exceed. Message id: "
                         << ( int )req.id() << std::endl;
         }
      }
      catch( const l2max::CException& anException )
      {
         TraceCritical << "Exception caught in CDevice::run. Coud not be processed." << std::endl;

         _errorCode = ecDeviceError;
      }

      StopTimers();

      if( _adapter->errorCode() != CAbstractAdapter::ecNoError )
      {
         _errorCode = ecAdapterError;
      }
      else if( _proto->errorCode() != CAbstractProto::ecNoError )
      {
         _errorCode = ecProtoError;
      }

      _ecuNotify->stopped();

      CDeviceStatistic::GetInstance()->SetReqQueueSize( _requestQueue.size() );
      CDeviceStatistic::GetInstance()->ResetReqCount();
      CDeviceStatistic::GetInstance()->ResetReqErrorCount();

      _ecuNotify->statUpdated();

      // to prevent coredump if no errors table has been read trough diagnostic session
      // end device stopped
      // See CECUDeviceView::OnDataRead
      errorsTable();

      return ret;
   }

   bool CDevice::StartDiagnosticSession()
   {
      TRACE_FUN( Routine, "CDevice::StartDiagnosticSession" );

      if( isInitialized() && !isRun() )
      {
         if( !_adapter->Initialize() )
         {
            _errorCode = ecAdapterError;
         }

         if( _errorCode == ecNoError )
         {
            _requestQueue.clear();

            Start();

            _requestQueue.push( CRequest::iStartDiagnosticSession );
         }
      }

      return _errorCode == ecNoError;
   }

   void CDevice::StopDiagnosticSession()
   {
      TRACE_FUN( Routine, "CDevice::StopDiagnosticSession" );

      if( isInitialized() && isRun() )
      {
         _requestQueue.push( CRequest::iStopDiagnosticSession );
      }
   }

   void CDevice::ReadDiagnosticData()
   {
      TRACE_FUN( Routine, "CDevice::ReadDiagnosticData" );

      if( isInitialized() && isRun() )
      {
         _requestQueue.push( CRequest::iReadDataByLocalIdentifier );
      }
   }

   void CDevice::TransmitData()
   {
      TRACE_FUN( Routine, "CDevice::TransmitData" );

      if( isInitialized() && isRun() )
      {
         _requestQueue.push( CRequest::iWriteDataByLocalIdentifier );
      }
   }

   CUnitsTable& CDevice::unitsTable()
   {
      TRACE_FUN( Routine, "CDevice::unitsTable" );

      l2max::CMutexLocker locker( _mutex );

      if( isInitialized() && isRun() )
      {
         if( !_diagnosticUnitsTable )
         {
            _diagnosticUnitsTable = new CUnitsTable( _proto->registryNamespace() );
         }

         if( !_diagnosticUnitsTable->dataCount() )
         {
            _diagnosticUnitsTable->BuildDataTable( _proto->diagnosticDataAdapter() );
         }

         Diagnostic::HControlAdapter controlAdapter( _proto->diagnosticControlAdapter() );

         if( !controlAdapter.IsNull() )
         {
            if( !_diagnosticUnitsTable->controlCount() )
            {
               _diagnosticUnitsTable->BuildControlTable( controlAdapter );
            }
         }
      }

      return *_diagnosticUnitsTable;
   }

   CErrorsTable& CDevice::errorsTable()
   {
      TRACE_FUN( Routine, "CDevice::errorsTable" );

      l2max::CMutexLocker locker( _mutex );

      if( isInitialized() && isRun() )
      {
         if( !_diagnosticErrorsTable  )
         {
            _diagnosticErrorsTable = new CErrorsTable();
         }

         _diagnosticErrorsTable->BuildErrorsTable( _proto->diagnosticErrorAdapter() );
      }

      return *_diagnosticErrorsTable;
   }

   CDevice::EErrorCode CDevice::errorCode()const
   {
      return _errorCode;
   }

   std::string CDevice::errorText()const
   {
      std::string errorText( _errorText[ _errorCode ] );

      if( _errorCode == ecAdapterError )
      {
         errorText += _adapter->errorText();
      }
      else if( _errorCode == ecProtoError )
      {
         errorText += _proto->errorText();
      }

      return errorText;
   }

   bool CDevice::isInitialized()const
   {
      return _isInitialized;
   }

   FlowMeter::CDevice* CDevice::flowMeter()
   {
      l2max::CMutexLocker locker( _mutex );

      if( isInitialized() && isRun() && !_flowMeterDevice )
      {
         _flowMeterDevice = new FlowMeter::CDevice( unitsTable() );
      }

      return _flowMeterDevice;
   }

   bool CDevice::ProcessRequest( CRequest::EId anId )
   {
      TRACE_FUN( Routine, "CDevice::ProcessRequest" );

      bool ret( true );

      if( anId == CRequest::iStartDiagnosticSession )
      {
         if( ret = _proto->StartDiagnosticSession() )
         {
            CDeviceStatistic::GetInstance()->SetReqQueueMaxSize( _requestQueue.maxQueueSize() );
            
            _ecuNotify->diagnosticSessionStarted();
         }
      }
      else if( anId == CRequest::iStopDiagnosticSession )
      {
         if( ret = _proto->StopDiagnosticSession() )
         {
            _ecuNotify->diagnosticSessionStoped();
         }
      }
      else if( anId == CRequest::iReadDataByLocalIdentifier )
      {
         if( ret = _proto->ReadDataByLocalIdentifier() )
         {
            _ecuNotify->dataRead();

            flowMeter()->update();

            bool adjustQueue( true );

            if( _isAutoAdjustQueue )
            {
               adjustQueue = !AdjustReadDataTimeo();
            }

            if( adjustQueue )
            {
               AdjustRequestQueue();

               CDeviceStatistic::GetInstance()->SetReqQueueMaxSize( _requestQueue.maxQueueSize() );
            }
         }
      }
      else if( anId == CRequest::iWriteDataByLocalIdentifier )
      {
         if( ret = _proto->WriteDataByLocalIdentifier() )
         {
            _ecuNotify->dataWriten( _proto->writeResultCode() );
         }
      }
      else if( anId == CRequest::iIdle )
      {
         TraceRoutine << "CDevice::run Idle message occurs. "
                      << "Nothing to do ..." << std::endl;
      }
      else
      {
         TraceRoutine << "CDevice::run::Unknown message id: "
                      << ( int )anId << std::endl;
      }

      return ret;
   }

   void CDevice::StopTimers()
   {
      _requestQueue.IgnorePush( true );
      _requestQueue.clear();

      _readDataTimer.StopTimer();

      _requestQueue.IgnorePush( false );
   }

   bool CDevice::AdjustReadDataTimeo()
   {
      bool ret( true );

      CRequestQueue::ESizeState sizeState( sizeState = _requestQueue.sizeState() );

      if( sizeState == CRequestQueue::ssInc )
      {
         _readDataTimer.IncInterval();
      }
      else if( sizeState == CRequestQueue::ssDec )
      {
         _readDataTimer.DecInterval();
      }
      else
      {
         ret = false;
      }

      return ret;
   }

   void CDevice::AdjustRequestQueue()
   {
      if( _readDataTimer.isRun() )
      {
         _requestQueue.SetMaxQueueSize( _requestQueueTime * 1000 / _readDataTimer.interval() );
      }
      else
      {
         _requestQueue.SetMaxQueueSize( CDevice::_initialRequestQueueSize );
      }
   }

   void CDevice::SetPeriodicTransmissionEnabled( bool anEnable )
   {
      if( anEnable )
      {
         _readDataTimer.StartTimer();
      }
      else
      {
         _readDataTimer.StopTimer();
      }
   }

   bool CDevice::IsPeriodicTransmissionEnabled()const
   {
      return _readDataTimer.isRun();
   }
   
   CAbstractProto* CDevice::protocol()
   {
      return _proto;
   }
}
