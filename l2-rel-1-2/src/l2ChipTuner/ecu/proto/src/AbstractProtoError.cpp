/*
 * AbstractProtoError.cpp
 *
 *  Created on: 23.02.2009
 *      Author: mars
 */

#include "AbstractProtoError.h"

#include <l2ExceptionThrower.h>

CAbstractProtoError::CAbstractProtoError()
{
   _errorTextMap[ ecNoError ]               = "No error";
   _errorTextMap[ ecNoResponse ]            = "No response";
   _errorTextMap[ ecBinaryError ]           = "Binary error. The protocol may be incompatible";
   _errorTextMap[ ecResponseUnExpected ]    = "Response unexpected. The protocol may be incopatible";
   _errorTextMap[ ecLogFileConfiguration ]  = "Read from file: configuration incorrect";
   _errorTextMap[ ecLogFilePacket ]         = "Read from file: no packets available";

   _errorCode = ecNoError;
}

CAbstractProtoError::~CAbstractProtoError(){}

CAbstractProtoError::EErrorCode CAbstractProtoError::errorCode()const
{
   return _errorCode;
}

std::string CAbstractProtoError::errorText()const
{
   TErrorTextMap::const_iterator i( _errorTextMap.find( errorCode() ) );
   
   CHECK_OBJECT_STATE( i != _errorTextMap.end(), "Error text not found" );
   
   std::string ret( i->second );
   
   if( implementationErrorCode() )
   {
      ret += " Impl: " + implementationErrorText();
   }
   
   return ret;
}

void CAbstractProtoError::ResetError()
{
   _errorCode = ecNoError;
}
