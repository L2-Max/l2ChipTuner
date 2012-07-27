/*
 * ECUDiagnosticUnitsMonitorItemDelegate.cpp
 *
 *  Created on: 06.02.2009
 *      Author: mars
 */

#include "ECUDiagnosticUnitsMonitorItemDelegate.h"
#include "ECUDiagnosticUnitsMonitorModelNodes.h"

#include <ECUUnitsTable.h>
#include <ECUDataUnit.h>

#include <l2ExceptionThrower.h>
#include <l2Variant.h>

#include <QDoubleSpinBox>

CECUDiagnosticUnitsMonitorItemDelegate::CECUDiagnosticUnitsMonitorItemDelegate( ECU::CUnitsTable& aTable ) : _dataUnitsTable( aTable ){}
CECUDiagnosticUnitsMonitorItemDelegate::~CECUDiagnosticUnitsMonitorItemDelegate(){}


QWidget* CECUDiagnosticUnitsMonitorItemDelegate::createEditor( QWidget* aParent,
                                                               const QStyleOptionViewItem& anOption,
                                                               const QModelIndex& anIndex )const
{
   QWidget* ret( QItemDelegate::createEditor( aParent, anOption, anIndex ) );
   
   if( ret )
   {
      CECUDiagnosticUnitsMonitorModelNode* node( reinterpret_cast< CECUDiagnosticUnitsMonitorModelNode* >( anIndex.internalPointer() ) );
      
      CHECK_OBJECT_STATE( node->type() == CECUDiagnosticUnitsMonitorModelNode::tUnit, "Delegate should be created for unit node" );
      
      CECUDiagnosticUnitsMonitorModelUnitNode* unitNode( static_cast< CECUDiagnosticUnitsMonitorModelUnitNode* >( node ) );
      
      l2max::CVariant dataSnapshot( _dataUnitsTable.data( unitNode->key() )->value() );
      
      if( dataSnapshot.precission() != l2max::CVariant::ptInfinitely )
      {
         QDoubleSpinBox* spinBox( dynamic_cast< QDoubleSpinBox* >( ret ) );
         
         CHECK_OBJECT_STATE( spinBox, "Delegate have to create double spin box for such data" );
         
         spinBox->setDecimals( dataSnapshot.precission() );
      }
   }
   
   return ret;
}
