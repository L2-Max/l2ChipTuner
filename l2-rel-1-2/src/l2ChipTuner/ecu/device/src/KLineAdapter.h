/*
 * KLineAdapter.h
 *
 *  Created on: 08.10.2008
 *      Author: mars
 */

#ifndef KLINEADAPTER_H_
#define KLINEADAPTER_H_

#include "AbstractAdapter.h"

namespace l2max
{
   class CAbstractIODevice;
}

class CKLineAdapter : public CAbstractAdapter
{
   l2max::CAbstractIODevice* _device;

   static bool _debug;
   
   void CloseDevice();
   
public:
   CKLineAdapter();
   virtual ~CKLineAdapter();

   virtual bool Initialize();
   virtual void UnInitialize();

   virtual bool Write( const l2max::CByteArray& aData )const;
   virtual bool Read( l2max::CByteArray& aData )const;
};

#endif /* KLINEADAPTER_H_ */
