/*
 * Meter2DGraphModel.h
 *
 *  Created on: 07.03.2009
 *      Author: L2-Max
 */

#ifndef METER2DGRAPHMODEL_H_
#define METER2DGRAPHMODEL_H_

#include "AbstractMeterModel.h"

#include <DiagnosticTable.h>

struct IMeter2DGraphUpdateConsumer;
class CECUDeviceView;

class CMeter2DGraphModel : public CAbstractMeterModel
{
   int _timeFrame;
   int _timeResolution;
   
   float _valueFrame;
   int _valueResolution;
   
   float _timeOffset;
   float _timeOffsetIntegrator;
   
   unsigned int _lastUpdate;
   
   Diagnostic::EUnit _lastUnit;
   
public:
   typedef std::pair< float, unsigned int > TValueTimePair;
   typedef std::list< TValueTimePair > TValueTimeContainer;

   typedef std::list< std::string > TTimeTitleContainer;
   typedef std::list< std::string > TValueTitleContainer;

private:
   TTimeTitleContainer _timeTitleContainer;
   TValueTitleContainer _valueTitleContainer;
   TValueTimeContainer _valueTimeContainer;
   
   void UpdateTimeTitleContainer();
   void UpdateValueTitleContainer();

   IMeter2DGraphUpdateConsumer& updateConsumer()const;
   
public:
   CMeter2DGraphModel( const Diagnostic::CUnitKey& aKey,
                       const CECUDeviceView& aDeviceView,
                       IMeter2DGraphUpdateConsumer& aUpdateConsumer );
   
   virtual ~CMeter2DGraphModel();

   virtual void update();
   
   void SetTimeFrame( int aFrame );
   void SetTimeResolution( int aResolution );
   void SetValueFrame( float aFrame );
   void SetValueResolution( int aResolution );
   
   void UpdateTimeOffset( unsigned int anInterval );
   
   bool hasValues()const;
   
   int timeFrame()const;
   int timeResolution()const;
   int valueResolution()const;
   
   float valueFrame()const;
   float timeOffset()const;
   
   std::string timeAxisTitle()const;
   std::string valueAxisTitle()const;
   
   const TValueTimeContainer& valueTimeContainer()const;
   const TTimeTitleContainer& timeTitleContainer()const;
   const TValueTitleContainer& valueTitleContainer()const;
};

#endif /* METER2DGRAPHMODEL_H_ */
