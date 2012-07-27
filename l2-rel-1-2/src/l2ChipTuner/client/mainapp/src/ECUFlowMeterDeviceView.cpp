/*
 * ECUFlowMeterDeviceView.cpp
 *
 *  Created on: 10.12.2008
 *      Author: mars
 */

#include "ECUFlowMeterDeviceView.h"

#include <ui_FlowMeterDeviceViewPage.h>

#include <AppDefs.h>
#include <AppContext.h>

#include <ECUFlowMeterDevice.h>

#include <l2Variant.h>
#include <l2Trace.h>

class CFlowMeterDeviceViewPage : public QWidget,
                                 public Ui_FlowMeterDeviceViewPage
{
public:
   CFlowMeterDeviceViewPage( QWidget* aParent ) : QWidget( aParent )
   {
      setupUi( this );
   }
};

CECUFlowMeterDeviceView::CECUFlowMeterDeviceView( QWidget* aParent ) :
   QScrollArea( aParent ),
  _ecuFlowMeterDeviceNotifyHandler( this )
{
   _flowMeterDevice = 0;

   _flowMeterDeviceViewPage = new CFlowMeterDeviceViewPage( this );

   connect( _flowMeterDeviceViewPage->_reset, SIGNAL( clicked() ), SLOT( OnReset() ) );
   _flowMeterDeviceViewPage->_reset->setShortcut( Qt::Key_F8 );

   setWidget( _flowMeterDeviceViewPage );
   setWidgetResizable( true );
}

CECUFlowMeterDeviceView::~CECUFlowMeterDeviceView()
{
   if( _flowMeterDevice )
   {
      _flowMeterDevice->Stop();
   }

   delete _flowMeterDeviceViewPage;
}

void CECUFlowMeterDeviceView::InstallFlowMeterDevice( ECU::FlowMeter::CDevice* aMeter )
{
   if( _flowMeterDevice != aMeter )
   {
      _flowMeterDevice = aMeter;

      if( _flowMeterDevice )
      {
         _flowMeterDevice->InstallNotifyHandler( &_ecuFlowMeterDeviceNotifyHandler );
         _flowMeterDevice->Start();
      }
   }
}

void CECUFlowMeterDeviceView::OnUpdated()
{
   TRACE_FUN( Routine, "CECUFlowMeterDeviceView::OnUpdated" );

   double flowPerSecond( _flowMeterDevice->flowPerSecondInstant() );

   l2max::CVariant flowPerHour( flowPerSecond * 3600. );
   flowPerHour.SetPrecission( 2, true );

   _flowMeterDeviceViewPage->_literPerHour->setText( std::string( flowPerHour.toString() + " l/h" ).c_str() );

   ///////////////////////////////////////////////////////////////////////////
   double flowPerMeter( _flowMeterDevice->flowPerMeterInstant() );

   l2max::CVariant flowPer100Kilometer( flowPerMeter * 1000. * 100. );
   flowPer100Kilometer.SetPrecission( 1, true );

   std::string instantFlowString( "∞ l/100 km" );

   if( flowPerMeter >= .0 )
   {
      instantFlowString = flowPer100Kilometer.toString() + " l/100 km";
   }

   _flowMeterDeviceViewPage->_literPer100Kilometer->setText( QString::fromUtf8( instantFlowString.c_str() ) );

   ///////////////////////////////////////////////////////////////////////////
   double cummulativeFlowPerMeter( _flowMeterDevice->flowPerMeterCummulative() );

   l2max::CVariant flowPer100KilometerCummulative( cummulativeFlowPerMeter * 100000 );
   flowPer100KilometerCummulative.SetPrecission( 1, true );

   std::string cummulativeFlowString( "∞ l/100 km" );

   if( cummulativeFlowPerMeter > .0 )
   {
      cummulativeFlowString = flowPer100KilometerCummulative.toString() + " l/100 km";
   }

   _flowMeterDeviceViewPage->_literPer100KilometerCummulative->
      setText( QString::fromUtf8( cummulativeFlowString.c_str() ) );


   l2max::CVariant cummulativeFlow( _flowMeterDevice->cummulativeFlow() );
   cummulativeFlow.SetPrecission( 2, true );

   _flowMeterDeviceViewPage->_cummulativeFlow->setText( std::string( cummulativeFlow.toString() + " l" ).c_str() );


   l2max::CVariant cummulativeDistance( _flowMeterDevice->cummulativeDistance() / 1000 );
   cummulativeDistance.SetPrecission( 1, true );

   _flowMeterDeviceViewPage->_cummulativeDistance->setText( std::string( cummulativeDistance.toString() + " km" ).c_str() );

   l2max::CVariant averageSpeed( _flowMeterDevice->averageSpeed() );
   averageSpeed.SetPrecission( 1, true );

   _flowMeterDeviceViewPage->_averageSpeed->setText( std::string( averageSpeed.toString() + " km/h" ).c_str() );

   ///////////////////////////////////////////////////////////////////////////////
   _flowMeterDeviceViewPage->_tps->setValue( _flowMeterDevice->tps() );
   _flowMeterDeviceViewPage->_rpm->display( static_cast< int >( _flowMeterDevice->rpm() ) );
}

void CECUFlowMeterDeviceView::OnReset()
{
   if( _flowMeterDevice )
   {
      _flowMeterDevice->Reset();
   }
}
