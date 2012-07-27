/*
 * Lanos15SOHCZXJNMode1Data0StreamImpl.h
 *
 *  Created on: 12.10.2008
 *      Author: L2-Max
 */

#ifndef ALDLLANOS15SOHCZXJNMODE1DATA0STREAMIMPL_H_
#define ALDLLANOS15SOHCZXJNMODE1DATA0STREAMIMPL_H_

#include "ALDLAbstractDataStream.h"

namespace ALDL
{
   class CLanos15SOHCZXJNMode1Data0StreamImpl : public CAbstractMode1Data0
   {
   public:
      CLanos15SOHCZXJNMode1Data0StreamImpl();
      virtual ~CLanos15SOHCZXJNMode1Data0StreamImpl();

      virtual bool Deserialize( l2max::CAbstractIODevice& anIODevice );
   };

}

#endif /* ALDLLANOS15SOHCZXJNMODE1DATA0STREAMIMPL_H_ */
