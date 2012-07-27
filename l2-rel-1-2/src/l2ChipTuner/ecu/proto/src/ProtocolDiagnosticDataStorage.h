/*
 * ProtocolDiagnosticDataStorage
 *
 *  Created on: 02.09.2009
 *      Author: mars
 */

#ifndef PROTOCOLDIAGNOSTICDATASTORAGE_H_
#define PROTOCOLDIAGNOSTICDATASTORAGE_H_

#include <l2TextTreeRegistry.h>
#include <l2Mutex.h>

namespace l2max
{
   class CByteArray;
   class CRegistry;
}

struct IProtocolDiagnosticDataStorageEventSupplier;

class CProtocolDiagnosticDataStorage
{
   const l2max::CRegistry& _readRegistry;
   l2max::CTextTreeRegistry _writeRegistry;

   mutable l2max::Registry::TKeyList _keys;
   mutable l2max::Registry::TKeyListIterator _currentKey;
   mutable l2max::Registry::TKeyListIterator _lastReadKey;
   mutable l2max::CMutex _mutex;
   mutable unsigned int _readPacketTimeo;

   bool _isReadEnabled;
   bool _isWriteEnabled;
   unsigned int _writePacketNo;
   unsigned int _writePacketTimeo;
   
   IProtocolDiagnosticDataStorageEventSupplier* _supplier;

   static std::string buildKey( int aNum );

protected:
   const l2max::CRegistry& readRegistry()const;
   l2max::CTextTreeRegistry& writeRegistry();

   bool IsWriteEnabled()const;

public:
   CProtocolDiagnosticDataStorage( const l2max::CRegistry& aRegistry );
   virtual ~CProtocolDiagnosticDataStorage();
   
   bool Read( l2max::CByteArray& aPacket )const;
   bool Write( const l2max::CByteArray& aPacket );
   
   bool IsReadEnabled()const;
   
   void SetPacketNo( unsigned int aPacketNo );

   unsigned int packetCount()const;
   unsigned int packetNo()const;
   
   void SetSupplier( IProtocolDiagnosticDataStorageEventSupplier* aSupplier );
};

#endif /* PROTOCOLDIAGNOSTICDATASTORAGE_H_ */
