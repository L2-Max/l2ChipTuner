/*
 * ALDLLanos15SOHCZXJNMode4StreamImpl.h
 *
 *  Created on: 02.02.2009
 *      Author: mars
 */

#ifndef ALDLLANOS15SOHCZXJNMODE4STREAMIMPL_H_
#define ALDLLANOS15SOHCZXJNMODE4STREAMIMPL_H_

#include "ALDLAbstractDataStream.h"
#include "DiagnosticTable.h"

namespace ALDL
{
   class CLanos15SOHCZXJNMode4StreamImpl : public CAbstractMode4
   {
   public:
      CLanos15SOHCZXJNMode4StreamImpl( Diagnostic::CDataAdapter& anAdapter );
      virtual ~CLanos15SOHCZXJNMode4StreamImpl();

      virtual void Serialize( l2max::CAbstractIODevice& anIODevice );
      virtual bool Deserialize( l2max::CAbstractIODevice& anIODevice );

      virtual HAbstractMode4 clone()const;
   };
}

#endif /* ALDLLANOS15SOHCZXJNMODE4STREAMIMPL_H_ */
