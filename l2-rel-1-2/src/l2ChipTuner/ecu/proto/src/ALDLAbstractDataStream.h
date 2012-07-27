/*
 * ALDLAbstractDataStream.h
 *
 *  Created on: 27.10.2008
 *      Author: mars
 */

#ifndef ALDLABSTRACTDATASTREAM_H_
#define ALDLABSTRACTDATASTREAM_H_

#include "DiagnosticDataAdapter.h"
#include "DiagnosticErrorAdapter.h"
#include "DiagnosticControlAdapter.h"
#include "ALDLAbstractDataStreamHandles.h"

#include <l2ByteArray.h>

namespace l2max
{
   class CAbstractIODevice;
}

namespace ALDL
{
   /*
    * Mode 1 Data 0. Data transfer stream
    * */
   class CAbstractMode1Data0 : public Diagnostic::CDataAdapter,
                               public Diagnostic::CErrorAdapter
   {
      CAbstractMode1Data0( const CAbstractMode1Data0& );
      CAbstractMode1Data0& operator = ( const CAbstractMode1Data0& );

   protected:
      void Adjust();

   public:
      CAbstractMode1Data0();
      virtual ~CAbstractMode1Data0();

      virtual bool Deserialize( l2max::CAbstractIODevice& anIODevice ) = 0;
   };

   /*
    * Mode 4. ECU Function
    * */
   class CAbstractMode4 : public Diagnostic::CControlAdapter
   {
   public:
      CAbstractMode4( Diagnostic::CDataAdapter& anAdapter );
      virtual ~CAbstractMode4();

      virtual void Serialize( l2max::CAbstractIODevice& anIODevice ) = 0;
      virtual bool Deserialize( l2max::CAbstractIODevice& anIODevice ) = 0;

      virtual HAbstractMode4 clone()const = 0;
   };
}

#endif /* ALDLABSTRACTDATASTREAM_H_ */
