/*
 * Adapter.h
 *
 *  Created on: 08.10.2008
 *      Author: mars
 */

#ifndef ABSTRACTADAPTER_H_
#define ABSTRACTADAPTER_H_

#include <string>
#include <map>

namespace l2max
{
   class CByteArray;
}

class CAbstractAdapter
{
public:
   enum EErrorCodes
   {
      ecNoError,
      ecCommDevOpenFail,
      ecCommDevSetupFail,
      ecWriteFail,
      ecReadFail,
      ecAdapterTrouble
   };

   enum EAdapterType
   {
      atKLine
   };

   enum EDeviceType
   {
      dtSerialLine
   };

protected:
   typedef std::map< EAdapterType, std::string > TAdapterTextTypes;
   static TAdapterTextTypes _adapterTextTypes;

   typedef std::map< EDeviceType, std::string > TDeviceTextTypes;
   static TDeviceTextTypes _deviceTextTypes;

   typedef std::map< EErrorCodes, std::string > TErrorTexts;
   mutable TErrorTexts _errorTexts;
   mutable EErrorCodes _errorCode;

public:
   CAbstractAdapter();
   virtual ~CAbstractAdapter();

   virtual bool Initialize() = 0;
   virtual void UnInitialize() = 0;

   virtual bool Write( const l2max::CByteArray& aData )const = 0;
   virtual bool Read( l2max::CByteArray& aData )const = 0;

   EErrorCodes errorCode();
   std::string errorText();

   static const std::string& TypeToText( EAdapterType anAdapterType );
   static const std::string& TypeToText( EDeviceType aDeviceType );

   void ResetError();
};

#endif /* ABSTRACTADAPTER_H_ */
