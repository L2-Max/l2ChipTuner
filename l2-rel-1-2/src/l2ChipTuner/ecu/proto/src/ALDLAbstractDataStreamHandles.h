/*
 * ALDLAbstractDataStreamHandles.h
 *
 *  Created on: 25.02.2009
 *      Author: mars
 */

#ifndef ALDLABSTRACTDATASTREAMHANDLES_H_
#define ALDLABSTRACTDATASTREAMHANDLES_H_

#include <l2RefHandle.hxx>

namespace ALDL
{
   class CAbstractMode1Data0;
   class CAbstractMode4;

   typedef l2max::CRefHandle< CAbstractMode1Data0 > HAbstractMode1Data0;
   typedef l2max::CRefHandle< CAbstractMode4 > HAbstractMode4;
}

#endif /* ALDLABSTRACTDATASTREAMHANDLES_H_ */
