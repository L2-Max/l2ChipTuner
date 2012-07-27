/*
 * AdaptersHandles.h
 *
 *  Created on: 25.02.2009
 *      Author: mars
 */

#ifndef ADAPTERSHANDLES_H_
#define ADAPTERSHANDLES_H_

#include <l2RefHandle.hxx>

namespace Diagnostic
{
   class CDataAdapter;
   class CControlAdapter;
   class CErrorAdapter;

   typedef l2max::CRefHandle< CDataAdapter > HDataAdapter;
   typedef l2max::CRefHandle< CControlAdapter > HControlAdapter;
   typedef l2max::CRefHandle< CErrorAdapter > HErrorAdapter;
}
#endif /* ADAPTERSHANDLES_H_ */
