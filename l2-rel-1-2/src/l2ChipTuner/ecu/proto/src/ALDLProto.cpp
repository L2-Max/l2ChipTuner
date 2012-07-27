/*
 * ALDLProto.cpp
 *
 *  Created on: 05.10.2008
 *      Author: L2-Max
 */
#include "ALDLProto.h"
#include "ALDLLanos15SOHCZXJNMode4StreamImpl.h"
#include "ALDLLanos15SOHCZXJNMode1Data0StreamImpl.h"
#include "ALDLLanos16DOHCYAIHMode1Data0StreamImpl.h"
#include "ProtocolCreationStrategy.h"
#include "ALDLProtocolDiagnosticDataStorage.h"

#include <AppDefs.h>
#include <AppContext.h>

#include <AbstractAdapter.h>

#include <l2ByteArray.h>
#include <l2Buffer.h>
#include <l2MutexLocker.h>
#include <l2Variant.h>
#include <l2Trace.h>
#include <l2Thread.h>
#include <l2ExceptionThrower.h>

#include <string>

namespace ALDL
{
   CProto::CProto( CAbstractAdapter& anAdapter, CALDLProtocolCreationStrategy* aStrategy ) :
      CDataStreamLayer( anAdapter ),
      _strategy( aStrategy )
   {
      _storage = 0;

      _errorTextMap[ ecNoError ] = "No error";
      _errorTextMap[ ecMode1Data0Deserialization ] = "Mode 1 Data 0 deserialization fail";
      _errorTextMap[ ecMode4Deserialization ] = "Mode 4 deserialization fail";

      SetECUId( 0xf4 );

      _ecuId = _strategy->ecuId();

      if( _ecuId == Protocol::ALDL::eiL15sohcZXJN )
      {
         _abstractMode1Data0 = *new CLanos15SOHCZXJNMode1Data0StreamImpl();
         _abstractMode4 = *new CLanos15SOHCZXJNMode4StreamImpl( *_abstractMode1Data0 );
      }
      else if( _ecuId == Protocol::ALDL::eiL16dohcYAIH )
      {
         _abstractMode1Data0 = *new CLanos16DOHCYAIHMode1Data0StreamImpl();
      }

      _namespace += std::string( "::" ) + Protocol::protocolDescriptionMap()->
                                             find( type() )->second._synonym;
      
      _namespace += std::string( "::" ) + Vehicle::modelDescriptionMap()->
                                             find( Protocol::ALDL::ecuIdDescriptionMap()->
                                                find( _ecuId )->second._model )->second._synonym;
      
      _namespace += std::string( "::" ) + Vehicle::engineDescriptionMap()->
                                             find( Protocol::ALDL::ecuIdDescriptionMap()->
                                                find( _ecuId )->second._engine )->second._synonym;
      
      _namespace += std::string( "::" ) + Protocol::ALDL::ecuIdDescriptionMap()->
                                             find( _ecuId )->second._synonym;
      
      CHECK_OBJECT_STATE( !_storage, "Storage is not null" );

      _storage = new CProtocolDiagnosticDataStorage( this, _strategy->registry() );
   }

   CProto::~CProto()
   {
      delete _strategy;
      delete _storage;
   }

   Diagnostic::HDataAdapter CProto::diagnosticDataAdapter()
   {
      return *_abstractMode1Data0;
   }

   Diagnostic::HControlAdapter CProto::diagnosticControlAdapter()
   {
      return *_abstractMode4;
   }

   Diagnostic::HErrorAdapter CProto::diagnosticErrorAdapter()
   {
      return *_abstractMode1Data0;
   }

   bool CProto::StartDiagnosticSession()
   {
      TRACE_FUN( Routine, "CProto::StartDiagnosticSession" );

      bool ret( false );

      ResetError();

      l2max::CByteArray resp;

      if( ret = MakeTransaction( mReturnToNormal, l2max::CByteArray(), resp ) )
      {
         ret = !resp.size();
      }

      return ret;
   }

   bool CProto::StopDiagnosticSession()
   {
      TRACE_FUN( Routine, "CProto::StopDiagnosticSession" );

      bool ret( false );

      ResetError();

      l2max::CByteArray resp;

      if( ret = MakeTransaction( mReturnToNormal, l2max::CByteArray(), resp ) )
      {
         ret = !resp.size();
      }

      return ret;
   }

   bool CProto::ReadDataByLocalIdentifier()
   {
      TRACE_FUN( Routine, "CProto::ReadDataByLocalIdentifier" );

      bool ret( false );

      ResetError();

      l2max::CByteArray param;
      l2max::CBuffer paramStream( &param );

      paramStream << ( unsigned char )( 0x00 );

      l2max::CByteArray resp;

      if( ret = MakeTransaction( mTransmitData, param, resp ) )
      {
         l2max::CBuffer respStream( &resp );
         respStream.open( l2max::CAbstractIODevice::omReadOnly );

         if( ret = _abstractMode1Data0->Deserialize( respStream ) )
         {
            if( !_abstractMode4.IsNull() )
            {
               _abstractMode4->Synchronize();
            }
         }
         else
         {
            CAbstractProto::_errorCode = ecBinaryError;

            _errorCode = ecMode1Data0Deserialization;
         }
      }

      return ret;
   }

   bool CProto::WriteDataByLocalIdentifier()
   {
      TRACE_FUN( Routine, "CProto::WriteDataByLocalIdentifier" );

      bool ret( false );

      ResetError();

      _writeResultCode = wrcNotExecuted;

      l2max::CByteArray param;
      l2max::CBuffer paramStream( &param );

      paramStream.open( l2max::CAbstractIODevice::omWriteOnly );

      _abstractMode4->Serialize( paramStream );

      l2max::CByteArray resp;

      if( ret = MakeTransaction( mECUFunc, param, resp ) )
      {
         HAbstractMode4 abstractMode4Clone( _abstractMode4->clone() );

         l2max::CBuffer respStream( &resp );
         respStream.open( l2max::CAbstractIODevice::omReadOnly );

         if( ret = abstractMode4Clone->Deserialize( respStream ) )
         {
            if( *_abstractMode4 == *abstractMode4Clone )
            {
               _writeResultCode = wrcAccept;
            }
            else
            {
               _writeResultCode = wrcReject;
            }
         }
         else
         {
            CAbstractProto::_errorCode = ecBinaryError;

            _errorCode = ecMode4Deserialization;
         }
      }

      return ret;
   }

   std::string CProto::implementationErrorText()const
   {
      TErrorTextMap::const_iterator i( _errorTextMap.find( _errorCode ) );

      CHECK_OBJECT_STATE( i != _errorTextMap.end(), "Error text not found" );

      return i->second;
   }

   CAbstractProto::TErrorCode CProto::implementationErrorCode()const
   {
      return _errorCode;
   }

   void CProto::ResetError()
   {
      _errorCode = ecNoError;

      CAbstractProtoError::ResetError();
   }
   
   bool CProto::MakeTransaction( unsigned char aMode, const l2max::CByteArray& aParam, l2max::CByteArray& aResp )
   {
      bool ret( false );
      
      if( _strategy->type() == CAbstractProtocolCreationStrategy::tRegistry )
      {
         ret = CDataStreamLayer::MakeTransaction( aMode, aParam, aResp );
      }
      else if( _strategy->type() == CAbstractProtocolCreationStrategy::tFile )
      {
         if( aMode == mReturnToNormal )
         {
            ret = ( _errorCode == ecNoError );
         }
         else if( aMode == mTransmitData )
         {
            CHECK_OBJECT_STATE( _storage, "Storage is null" );

            ret = _storage->Read( aResp );

            if( !ret )
            {
               CAbstractProtoError::_errorCode = ecLogFilePacket;
            }
         }
         else
         {
            ret = CDataStreamLayer::MakeTransaction( aMode, aParam, aResp );
         }
      }
      
      if( ret )
      {
         if( _storage && aMode == mTransmitData )
         {
            _storage->Write( aResp );
         }
      }
      
      return ret;
   }

   Protocol::ALDL::EECUId CProto::ecuId()const
   {
      return _ecuId;
   }

   ::CProtocolDiagnosticDataStorage* CProto::storage()
   {
      return _storage;
   }
}
