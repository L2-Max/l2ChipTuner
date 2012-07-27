/*
 * Meter2DGraphModel.cpp
 *
 *  Created on: 07.03.2009
 *      Author: L2-Max
 */

#include "Meter2DGraphModel.h"
#include "IMeter2DGraphUpdateConsumer.h"

#include <AppContext.h>
#include <AppDefs.h>
#include <ECUDataUnit.h>

#include <l2Timer.h>
#include <l2Trace.h>
#include <l2ExceptionThrower.h>

CMeter2DGraphModel::CMeter2DGraphModel( const Diagnostic::CUnitKey& aKey,
                                        const CECUDeviceView& aDeviceView,
                                        IMeter2DGraphUpdateConsumer& aUpdateConsumer ) :
   CAbstractMeterModel( aKey, aDeviceView, aUpdateConsumer )
{
   TRACE_FUN( Routine, "CMeter2DGraphModel::CMeter2DGraphModel" );
   
   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   _lastUpdate = 0;
   _lastUnit = Diagnostic::uCounts;
   
   _timeOffset = .0;
   _timeOffsetIntegrator = 1.;
   
   _timeFrame = 30;
   _timeResolution = 10;
   _valueFrame = .0;
   _valueResolution = 5;
   
   SetTimeFrame( 30 );
   SetTimeResolution( 10 );
   SetValueFrame( .0 );
   SetValueResolution( 5 );
   
   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   l2max::CVariant timeFrame;
   l2max::CVariant timeResolution;
   l2max::CVariant valueResolution;

   CAppContext::GetInstance()->MainRegistry().Read( registryKey() + V_METER_GRAPH_TIME_FRAME, timeFrame );
   if( timeFrame.isValid() )
   {
      SetTimeFrame( timeFrame.toInt() );
   }

   CAppContext::GetInstance()->MainRegistry().Read( registryKey() + V_METER_GRAPH_TIME_RESOLUTION, timeResolution );
   if( timeResolution.isValid() )
   {
      SetTimeResolution( timeResolution.toInt() );
   }

   CAppContext::GetInstance()->MainRegistry().Read( registryKey() + V_METER_GRAPH_VALUE_RESOLUTION, valueResolution );
   if( valueResolution.isValid() )
   {
      SetValueResolution( valueResolution.toInt() );
   }
}

CMeter2DGraphModel::~CMeter2DGraphModel()
{
   CAppContext::GetInstance()->MainRegistry().Write( registryKey() + V_METER_GRAPH_TIME_FRAME, timeFrame() );
   CAppContext::GetInstance()->MainRegistry().Write( registryKey() + V_METER_GRAPH_TIME_RESOLUTION, timeResolution() );
   CAppContext::GetInstance()->MainRegistry().Write( registryKey() + V_METER_GRAPH_VALUE_RESOLUTION, valueResolution() );
}

void CMeter2DGraphModel::update()
{
   TRACE_FUN( Frequently, "CMeter2DGraphModel::update" );
   
   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   if( _valueTimeContainer.size() )
   {
      unsigned int curTime( l2max::CTimer::clock() );
      unsigned int interval( curTime - _lastUpdate );
      
      _lastUpdate = curTime;

      float newValueFrame( .0 );
      
      Diagnostic::HAbstractDataFormula oldFormula;
      Diagnostic::HAbstractDataFormula newFormula;
      
      if( _lastUnit != dataUnit()->measureUnit() )
      {
         oldFormula = dataUnit()->formula( _lastUnit );
         newFormula = dataUnit()->formula();
         
         _lastUnit = dataUnit()->measureUnit();

         updateConsumer().valueChanged();
      }
      
      for( TValueTimeContainer::iterator I_value( _valueTimeContainer.begin() ); I_value != _valueTimeContainer.end(); )
      {
         if( I_value->second / 1000. > timeFrame() + float( interval ) / 1000. )
         {
            I_value = _valueTimeContainer.erase( I_value );
         }
         else
         {
            if( !oldFormula.IsNull() || !newFormula.IsNull() )
            {
               l2max::CVariant newValue( I_value->first );
   
               if( !oldFormula.IsNull() )
               {
                  newValue = oldFormula->encode( newValue );
               }
   
               if( !newFormula.IsNull() )
               {
                  newValue = newFormula->decode( newValue );
               }
   
               I_value->first = newValue.toDouble();
            }
            
            I_value->second += interval;
            
            if( newValueFrame < I_value->first )
            {
               newValueFrame = I_value->first;
            }

            ++I_value;
         }
      }
      
      //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      if( !newValueFrame )
      {
         newValueFrame = 255.;
      }
      else
      {
         newValueFrame *= 1.2;
      }
      
      if( newValueFrame != valueFrame() )
      {
         SetValueFrame( newValueFrame );
      }
   }
   else
   {
      _lastUnit = dataUnit()->measureUnit();
   }
   
   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   if( dataUnit()->IsChanged() || _valueTimeContainer.size() < 2 )
   {
      _valueTimeContainer.push_front( TValueTimePair( dataUnit()->value().toDouble(), 0 ) );
      
      updateConsumer().valueChanged();
   }
   else
   {
      _valueTimeContainer.front().second = 0;
   }
   
   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   TValueTimeContainer::const_iterator I_value( _valueTimeContainer.begin() );

   if( I_value != _valueTimeContainer.end() && ++I_value != _valueTimeContainer.end() && I_value->second )
   {
      if( I_value->second < _timeOffset )
      {
         _timeOffset -= float( I_value->second );
   
         _timeOffsetIntegrator = _timeOffset / -float( I_value->second );
      }
      else
      {
         _timeOffset = .0;
         _timeOffsetIntegrator = 1.;
      }
   }
   
   CAbstractMeterModel::update();
}

void CMeter2DGraphModel::SetTimeFrame( int aFrame )
{
   _timeFrame = aFrame;
   
   UpdateTimeTitleContainer();
}

void CMeter2DGraphModel::SetTimeResolution( int aResolution )
{
   _timeResolution = aResolution;
   
   UpdateTimeTitleContainer();
}

void CMeter2DGraphModel::SetValueFrame( float aFrame )
{
   _valueFrame = aFrame;
   
   UpdateValueTitleContainer();
}

void CMeter2DGraphModel::SetValueResolution( int aResolution )
{
   _valueResolution = aResolution;
   
   UpdateValueTitleContainer();
}

int CMeter2DGraphModel::timeFrame()const
{
   return _timeFrame;
}

int CMeter2DGraphModel::timeResolution()const
{
   return _timeResolution;
}

float CMeter2DGraphModel::valueFrame()const
{
   return _valueFrame;
}

int CMeter2DGraphModel::valueResolution()const
{
   return _valueResolution;
}

const CMeter2DGraphModel::TValueTimeContainer& CMeter2DGraphModel::valueTimeContainer()const
{
   return _valueTimeContainer;
}

bool CMeter2DGraphModel::hasValues()const
{
   return _valueTimeContainer.size() > 1;
}

const CMeter2DGraphModel::TTimeTitleContainer& CMeter2DGraphModel::timeTitleContainer()const
{
   return _timeTitleContainer;
}
const CMeter2DGraphModel::TValueTitleContainer& CMeter2DGraphModel::valueTitleContainer()const
{
   return _valueTitleContainer;
}

void CMeter2DGraphModel::UpdateTimeTitleContainer()
{
   TRACE_FUN( Frequently, "CMeter2DGraphModel::UpdateTimeTitleContainer" );
   
   _timeTitleContainer.clear();

   float timeTitleSecs( .0 );

   for( int i( timeResolution() ); i > 0; --i )
   {
      timeTitleSecs += float( timeFrame() ) / timeResolution();

      l2max::CVariant minute( int( timeTitleSecs ) / 60 );
      minute.SetWidth( 2 );

      l2max::CVariant second( int( timeTitleSecs ) % 60 );
      second.SetWidth( 2 );

      std::string timeText( minute.toString() + ":" + second.toString() );

      _timeTitleContainer.push_back( timeText );
   }

   updateConsumer().timeFrameChanged();
}

void CMeter2DGraphModel::UpdateValueTitleContainer()
{
   TRACE_FUN( Frequently, "CMeter2DGraphModel::UpdateValueTitleContainer" );
   
   _valueTitleContainer.clear();

   float valueTitle( .0 );
   
   for( int i( valueResolution() ); i > 0; --i )
   {
      valueTitle += valueFrame() / valueResolution();

      l2max::CVariant value( valueTitle );

      value.SetType( dataUnit()->value().type() );
      value.SetWidth( dataUnit()->value().width() );
      value.SetPrecission( dataUnit()->value().precission(), dataUnit()->value().isPrecissionFixed() );

      _valueTitleContainer.push_front( value.toString() );
   }

   updateConsumer().valueFrameChanged();
}

float CMeter2DGraphModel::timeOffset()const
{
   return _timeOffset;
}

void CMeter2DGraphModel::UpdateTimeOffset( unsigned int anInterval )
{
   _timeOffset += anInterval * _timeOffsetIntegrator;
}

std::string CMeter2DGraphModel::timeAxisTitle()const
{
   return "time ( mm:ss )";
}

std::string CMeter2DGraphModel::valueAxisTitle()const
{
   std::string ret( dataUnit()->measureUnitName() );

   std::string symbol( dataUnit()->measureUnitSymbol() );

   if( symbol.size() )
   {
      ret += " (" + symbol + ")";
   }

   return ret;
}

IMeter2DGraphUpdateConsumer& CMeter2DGraphModel::updateConsumer()const
{
   return dynamic_cast< IMeter2DGraphUpdateConsumer& >( CAbstractMeterModel::updateConsumer() );
}
