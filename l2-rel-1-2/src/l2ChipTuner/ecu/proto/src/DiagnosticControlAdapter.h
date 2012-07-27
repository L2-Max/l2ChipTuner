/*
 * DiagnosticControlAdapter.h
 *
 *  Created on: 02.02.2009
 *      Author: mars
 */

#ifndef DIAGNOSTICCONTROLADAPTER_H_
#define DIAGNOSTICCONTROLADAPTER_H_

#include "DiagnosticAbstractAdapter.h"

#include <l2Mutex.h>
#include <l2RefObject.h>

namespace l2max
{
   class CVariant;
}

namespace Diagnostic
{
   class CDataAdapter;

   class CControlAdapter : public CAbstractAdapter
   {
      TUnitKeys _unitKeys;

      void Synchronize( EDataType aType );

   protected:
      mutable l2max::CMutex _controlMutex;

      typedef std::map< EDataType, int > TRawDataMap;
      TRawDataMap _rawDataMap;
      
      CDataAdapter& _dataAdapter;

      void Clear();

   public:
      CControlAdapter( CDataAdapter& anAdapter );
      virtual ~CControlAdapter();

      virtual Diagnostic::TUnitKeys keys()const;
      virtual void Synchronize();

      bool operator == ( const CControlAdapter& anAdapter )const;
      bool operator != ( const CControlAdapter& anAdapter )const;

      void SetValue( const l2max::CVariant& aValue, EDataType aType, EUnit anUnit );

      l2max::CVariant value( Diagnostic::EDataType aType )const;
   };
}

#endif /* DIAGNOSTICCONTROLADAPTER_H_ */
