/*
 * FileProtocolPacketSelectionDialog.cpp
 *
 *  Created on: 31.08.2009
 *      Author: L2-Max
 */

#include "FileProtocolPacketSelectionView.h"
#include "ProtocolDiagnosticDataStorageEventChannel.h"

#include <ProtocolDiagnosticDataStorage.h>

#include <AppDefs.h>
#include <AppContext.h>

#include <l2Trace.h>

CFileProtocolPacketSelectionView::CFileProtocolPacketSelectionView( QWidget* aParent, CProtocolDiagnosticDataStorage& aStorage ) :
   QWidget( aParent ),
   _storage( aStorage )
{
   TRACE_FUN( Routine, "CFileProtocolPacketSelectionView::CFileProtocolPacketSelectionView" );
   
   setupUi( this );
   
   setWindowFlags( Qt::Tool );

   _channel = new CProtocolDiagnosticDataStorageEventChannel( this );
   
   _storage.SetSupplier( _channel );
   
   _packetSelectionSlider->setMinimum( 0 );
   
   connect( _packetSelectionSlider, SIGNAL( sliderReleased() ), SLOT( OnPacketChangedBySlider() ) );
   connect( _packetSelectionSpinBox, SIGNAL( editingFinished() ), SLOT( OnPacketChangedBySpinBox() ) );
}

CFileProtocolPacketSelectionView::~CFileProtocolPacketSelectionView()
{
   TRACE_FUN( Routine, "CFileProtocolPacketSelectionView::~CFileProtocolPacketSelectionView" );
   
   _storage.SetSupplier( 0 );
   
   delete _channel;
}

void CFileProtocolPacketSelectionView::OnPacketRead()
{
   if( !_packetSelectionSlider->isSliderDown() && !_packetSelectionSpinBox->hasFocus() )
   {
      _packetCountLabel->setText( l2max::CVariant( _storage.packetCount() ).toString().c_str() );
      
      _packetSelectionSlider->setMaximum( _storage.packetCount() );
      _packetSelectionSlider->setTickInterval( _storage.packetCount() / 10 );
      
      _packetSelectionSlider->setValue( _storage.packetNo() );
      
      _packetSelectionSpinBox->setMaximum( _storage.packetCount() );
      _packetSelectionSpinBox->setValue( _storage.packetNo() );
   }
}

void CFileProtocolPacketSelectionView::OnPacketChangedBySlider()
{
   _storage.SetPacketNo( _packetSelectionSlider->value() );
}

void CFileProtocolPacketSelectionView::OnPacketChangedBySpinBox()
{
   _storage.SetPacketNo( _packetSelectionSpinBox->value() );

   _packetSelectionSpinBox->clearFocus();
}
