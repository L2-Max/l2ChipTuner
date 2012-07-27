/*
 * AbstractProtoError.h
 *
 *  Created on: 23.02.2009
 *      Author: mars
 */

#ifndef ABSTRACTPROTOERROR_H_
#define ABSTRACTPROTOERROR_H_

#include <string>
#include <map>

class CAbstractProtoError
{
public:
   enum EErrorCode
   {
      ecNoError,
      ecNoResponse,
      ecBinaryError,
      ecResponseUnExpected,
      ecLogFileConfiguration,
      ecLogFilePacket
   };
   
   typedef unsigned int TErrorCode;

private:
   typedef std::map< EErrorCode, std::string > TErrorTextMap;
   TErrorTextMap _errorTextMap;

protected:
   EErrorCode _errorCode;
   
public:
   CAbstractProtoError();
   virtual ~CAbstractProtoError();
   
   virtual std::string implementationErrorText()const = 0;
   virtual TErrorCode implementationErrorCode()const = 0;
   
   virtual void ResetError();
   
   EErrorCode errorCode()const;
   std::string errorText()const;
};

#endif /* ABSTRACTPROTOERROR_H_ */
