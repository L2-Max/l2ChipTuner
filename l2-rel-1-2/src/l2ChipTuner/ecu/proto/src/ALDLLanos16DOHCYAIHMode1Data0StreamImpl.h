/*
 * ALDLLanos16DOHCYAIHMode1Data0StreamImpl.h
 *
 *  Created on: 29.10.2008
 *      Author: mars
 */

#ifndef ALDLLANOS16DOHCYAIHMODE1DATA0STREAMIMPL_H_
#define ALDLLANOS16DOHCYAIHMODE1DATA0STREAMIMPL_H_

#include "ALDLAbstractDataStream.h"

namespace ALDL
{
   class CLanos16DOHCYAIHMode1Data0StreamImpl : public CAbstractMode1Data0
   {
   public:
      CLanos16DOHCYAIHMode1Data0StreamImpl();
      virtual ~CLanos16DOHCYAIHMode1Data0StreamImpl();

      virtual bool Deserialize( l2max::CAbstractIODevice& anIODevice );
   };
}

#endif /* ALDLLANOS16DOHCYAIHMODE1DATA0STREAMIMPL_H_ */
