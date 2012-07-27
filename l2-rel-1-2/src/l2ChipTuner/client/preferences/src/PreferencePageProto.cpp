/*
 * PreferencePageProto.cpp
 *
 *  Created on: 26.10.2008
 *      Author: L2-Max
 */

#include "PreferencePageProto.h"
#include "PreferencePageProtoALDL.h"

#include <AppDefs.h>
#include <AppContext.h>
#include <ProtocolTable.h>

class CKWPProtocolPage : public QLabel,
                         public IPreferenceOptionalPage
{
public:
   CKWPProtocolPage( QWidget* aParent ) : QLabel( aParent )
   {
      setAlignment( Qt::AlignHCenter );

      setText( "The protocol implementation under development and not working now" );
   }

   virtual void save(){}
   virtual void show(){ QLabel::show(); }
};

CPreferencePageProto::CPreferencePageProto() : CPreferencePage( "Protocol" )
{
   _protocolConfigurationPage = 0;

   setupUi( this );

   CDescriptionTableHandle< Protocol::TProtocolDescriptionMap > protocolDescriptionMap( Protocol::protocolDescriptionMap() );

   for( Protocol::TProtocolDescriptionMap::const_iterator i( protocolDescriptionMap->begin() ); i != protocolDescriptionMap->end(); ++i )
   {
      _commProto->addItem( i->second._name, i->second._id );
   }

   connect( _commProto, SIGNAL( currentIndexChanged( int ) ), this, SLOT( OnCommunicationProtocolChanged( int ) ) );

   l2max::CVariant protoType;
   CAppContext::GetInstance()->MainRegistry().Read( V_ECU_PROTO_TYPE, protoType );

   if( _commProto->currentIndex() == _commProto->findData( protoType.toInt() ) )
   {
      OnCommunicationProtocolChanged( _commProto->currentIndex() );
   }
   else
   {
      _commProto->setCurrentIndex( _commProto->findData( protoType.toInt() ) );
   }
}

CPreferencePageProto::~CPreferencePageProto()
{
   delete _protocolConfigurationPage;
}

void CPreferencePageProto::save()
{
   l2max::CVariant val;

   val = _commProto->itemData( _commProto->currentIndex() ).toInt();
   CAppContext::GetInstance()->MainRegistry().Write( V_ECU_PROTO_TYPE, val );

   if( _protocolConfigurationPage )
   {
      _protocolConfigurationPage->save();
   }

   CPreferencePage::save();
}

void CPreferencePageProto::OnCommunicationProtocolChanged( int anIndex )
{
   if( _protocolConfigurationPage )
   {
      _protocolConfigurationPage->save();

      delete _protocolConfigurationPage;
      _protocolConfigurationPage = 0;
   }

   Protocol::EProtocolType protoType( static_cast< Protocol::EProtocolType >( _commProto->itemData( anIndex ).toInt() ) );

   if( protoType == Protocol::ptALDL )
   {
      _protocolConfigurationPage = new CPreferencePageProtoALDL( _commProtoOptions );
   }
   else
   {
      _protocolConfigurationPage = new CKWPProtocolPage( _commProtoOptions );
   }

   if( _protocolConfigurationPage )
   {
      _protocolConfigurationPage->show();
   }
}
