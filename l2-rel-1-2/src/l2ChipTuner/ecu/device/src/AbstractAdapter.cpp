/*
 * AbstractAdapter.cpp
 *
 *  Created on: 08.10.2008
 *      Author: mars
 */

#include "AbstractAdapter.h"

CAbstractAdapter::TAdapterTextTypes CAbstractAdapter::_adapterTextTypes;
CAbstractAdapter::TDeviceTextTypes CAbstractAdapter::_deviceTextTypes;


CAbstractAdapter::CAbstractAdapter()
{
   _errorTexts[ ecNoError ] = "No error";
   _errorTexts[ ecWriteFail ] = "Adapter write fail";
   _errorTexts[ ecReadFail ] = "Adapter read fail";
   _errorTexts[ ecAdapterTrouble ] = "Adapter trouble";

   _errorCode = ecNoError;
}

CAbstractAdapter::~CAbstractAdapter(){}

CAbstractAdapter::EErrorCodes CAbstractAdapter::errorCode()
{
   return _errorCode;
}

std::string CAbstractAdapter::errorText()
{
   return _errorTexts[ _errorCode ];
}

const std::string& CAbstractAdapter::TypeToText( EAdapterType anAdapterType )
{
   if( !CAbstractAdapter::_adapterTextTypes.size() )
   {
      CAbstractAdapter::_adapterTextTypes[ CAbstractAdapter::atKLine ] = "K-Line";
   }

   return CAbstractAdapter::_adapterTextTypes[ anAdapterType ];
}

const std::string& CAbstractAdapter::TypeToText( EDeviceType aDeviceType )
{
   if( !CAbstractAdapter::_deviceTextTypes.size() )
   {
      CAbstractAdapter::_deviceTextTypes[ CAbstractAdapter::dtSerialLine ] = "Serial Port";
   }

   return CAbstractAdapter::_deviceTextTypes[ aDeviceType ];
}

void CAbstractAdapter::ResetError()
{
   _errorCode = ecNoError;
}
