/*
 * PreferencePageProtoALDL.cpp
 *
 *  Created on: 25.03.2009
 *      Author: L2-Max
 */

#include "PreferencePageProtoALDL.h"

#include <AppDefs.h>
#include <AppContext.h>
#include <ProtocolTable.h>
#include <VehicleTable.h>

CPreferencePageProtoALDL::CPreferencePageProtoALDL( QWidget* aParent ) : QWidget( aParent )
{
   setupUi( this );

   parentWidget()->setMinimumSize( size() );

   Protocol::ALDL::TModelSet modelSet( Protocol::ALDL::modelSet() );

   for( Protocol::ALDL::TModelSet::const_iterator i( modelSet.begin() ); i != modelSet.end(); ++i )
   {
      _model->addItem( Vehicle::modelDescriptionMap()->find( *i )->second._name,
                       Vehicle::modelDescriptionMap()->find( *i )->second._id );
   }

   connect( _model, SIGNAL( currentIndexChanged( int ) ), SLOT( OnModelChanged( int ) ) );
   connect( _engine, SIGNAL( currentIndexChanged( int ) ), SLOT( OnEngineChanged( int ) ) );

   ///////////////////////////////////////////////////////////////////////////////////////
   l2max::CVariant model;
   CAppContext::GetInstance()->MainRegistry().Read( V_ECU_PROTO_ALDL_MODEL, model );

   if( _model->currentIndex() == _model->findData( model.toInt() ) )
   {
      OnModelChanged( _model->currentIndex() );
   }
   else
   {
      _model->setCurrentIndex( _model->findData( model.toInt() ) );
   }

   ///////////////////////////////////////////////////////////////////////////////////////
   l2max::CVariant engine;
   CAppContext::GetInstance()->MainRegistry().Read( V_ECU_PROTO_ALDL_ENGINE, engine );

   if( _engine->currentIndex() == _engine->findData( engine.toInt() ) )
   {
      OnEngineChanged( _engine->currentIndex() );
   }
   else
   {
      _engine->setCurrentIndex( _engine->findData( engine.toInt() ) );
   }

   ///////////////////////////////////////////////////////////////////////////////////////
   l2max::CVariant ecuId;
   CAppContext::GetInstance()->MainRegistry().Read( V_ECU_PROTO_ALDL_ECUID, ecuId );

   if( _ecuId->currentIndex() != _ecuId->findData( ecuId.toInt() ) )
   {
      _ecuId->setCurrentIndex( _ecuId->findData( ecuId.toInt() ) );
   }
}

CPreferencePageProtoALDL::~CPreferencePageProtoALDL()
{
   parentWidget()->setMinimumSize( QSize() );
}

void CPreferencePageProtoALDL::save()
{
   CAppContext::GetInstance()->MainRegistry().Write( V_ECU_PROTO_ALDL_MODEL, _model->itemData( _model->currentIndex() ).toInt() );
   CAppContext::GetInstance()->MainRegistry().Write( V_ECU_PROTO_ALDL_ENGINE, _engine->itemData( _engine->currentIndex() ).toInt() );
   CAppContext::GetInstance()->MainRegistry().Write( V_ECU_PROTO_ALDL_ECUID, _ecuId->itemData( _ecuId->currentIndex() ).toInt() );
}

void CPreferencePageProtoALDL::show()
{
   QWidget::show();
}

void CPreferencePageProtoALDL::OnModelChanged( int anIndex )
{
   _engine->clear();

   Protocol::ALDL::TEngineSet engineSet( Protocol::ALDL::engineSet( static_cast< Vehicle::EModelType >( _model->itemData( anIndex ).toInt() ) ) );

   for( Protocol::ALDL::TEngineSet::const_iterator i( engineSet.begin() ); i != engineSet.end(); ++i )
   {
      _engine->addItem( Vehicle::engineDescriptionMap()->find( *i )->second._name,
                        Vehicle::engineDescriptionMap()->find( *i )->second._id );
   }
}

void CPreferencePageProtoALDL::OnEngineChanged( int anIndex )
{
   _ecuId->clear();

   Protocol::ALDL::TEcuIdSet ecuIdSet( Protocol::ALDL::ecuIdSet(
      static_cast< Vehicle::EModelType >( _model->itemData( _model->currentIndex() ).toInt() ),
      static_cast< Vehicle::EEngineType >( _engine->itemData( anIndex ).toInt() ) ) );

   for( Protocol::ALDL::TEcuIdSet::const_iterator i( ecuIdSet.begin() ); i != ecuIdSet.end(); ++i )
   {
      _ecuId->addItem( Protocol::ALDL::ecuIdDescriptionMap()->find( *i )->second._name,
                       Protocol::ALDL::ecuIdDescriptionMap()->find( *i )->second._id );
   }
}
