/*
 * KWP2kProto.h
 *
 *  Created on: 04.11.2008
 *      Author: mars
 */

#ifndef KWP2KPROTO_H_
#define KWP2KPROTO_H_

#include "KWP2kApplicationLayer.h"

class CAbstractAdapter;

namespace KWP2k
{
   class CProto : public CApplicationLayer
   {
   public:
      CProto( CAbstractAdapter& anAdapter );
      virtual ~CProto();

      virtual Diagnostic::HDataAdapter diagnosticDataAdapter();
      virtual Diagnostic::HControlAdapter diagnosticControlAdapter();
      virtual Diagnostic::HErrorAdapter diagnosticErrorAdapter();

      virtual std::string implementationErrorText()const;
      virtual CAbstractProto::TErrorCode implementationErrorCode()const;
   };
}

#endif /* KWP2KPROTO_H_ */
