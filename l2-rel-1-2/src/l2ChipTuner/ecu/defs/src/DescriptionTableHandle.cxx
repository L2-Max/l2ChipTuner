/*
 * DescriptionTableHandle.cpp
 *
 *  Created on: 24.03.2009
 *      Author: L2-Max
 */

#include "DescriptionTableHandle.hxx"
#include "DiagnosticTable.h"
#include "ProtocolTable.h"
#include "VehicleTable.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////
template class CDescriptionTableHandle< Diagnostic::TDataDescriptionMap >;
template class CDescriptionTableHandle< Diagnostic::TGroupDescriptionMap >;
template class CDescriptionTableHandle< Diagnostic::TUnitDescriptionMap >;
template class CDescriptionTableHandle< Diagnostic::TPrefixDescriptionMap >;
template class CDescriptionTableHandle< Diagnostic::TErrorDescriptionMap >;
template class CDescriptionTableHandle< Diagnostic::TControlDescriptionMap >;
template class CDescriptionTableHandle< Diagnostic::TGroup2UnitKeyMap >;
template class CDescriptionTableHandle< Diagnostic::TUnitKey2GroupMap >;

template class CDescriptionTableHandle< Protocol::TProtocolDescriptionMap >;
template class CDescriptionTableHandle< Protocol::ALDL::TECUIdDescriptionMap >;

template class CDescriptionTableHandle< Vehicle::TEngineDescriptionMap >;
template class CDescriptionTableHandle< Vehicle::TModelDescriptionMap >;

////////////////////////////////////////////////////////////////////////////////////////////////////////////
template< class TDescriptionTable >
   l2max::CMutex CDescriptionTableHandle< TDescriptionTable >::_mutex;

template< class TDescriptionTable >
   CDescriptionTableHandle< TDescriptionTable >::CDescriptionTableHandle( const CDescriptionTableHandle& aHandle ) :
      _descriptionTable( aHandle._descriptionTable )
   {
      CDescriptionTableHandle< TDescriptionTable >::_mutex.Lock();
   }

template< class TDescriptionTable >
   CDescriptionTableHandle< TDescriptionTable >::CDescriptionTableHandle( const TDescriptionTable& aTable ) : _descriptionTable( aTable )
   {
      CDescriptionTableHandle< TDescriptionTable >::_mutex.Lock();
   }

template< class TDescriptionTable >
   CDescriptionTableHandle< TDescriptionTable >::~CDescriptionTableHandle()
   {
      CDescriptionTableHandle< TDescriptionTable >::_mutex.UnLock();
   }

template< class TDescriptionTable >
   const TDescriptionTable* CDescriptionTableHandle< TDescriptionTable >::operator -> ()
   {
      return &_descriptionTable;
   }
