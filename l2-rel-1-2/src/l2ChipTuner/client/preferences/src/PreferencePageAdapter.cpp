/*
 * PreferencePageAdapter.cpp
 *
 *  Created on: 16.10.2008
 *      Author: mars
 */

#include "PreferencePageAdapter.h"
#include "IPreferenceOptionalPage.h"

#include <AppDefs.h>
#include <AppContext.h>
#include <AbstractAdapter.h>

#include <ui_PreferencePageAdapterSerialPort.h>

#include <l2SerialLine.h>

#include <QLineEdit>

class CSerialPortPage : public Ui_PreferencePageAdapterSerialPort,
                        public IPreferenceOptionalPage,
                        public QWidget
{
public:
   CSerialPortPage( QWidget* aParent ) : QWidget( aParent )
   {
      setupUi( this );

      parentWidget()->setMinimumSize( size() );

      _bitsPerSecond->setValidator( new QIntValidator( 110, 131072, aParent ) );
      _writeTimeout->setValidator( new QIntValidator( 10, 10000, aParent ) );
      _readTimeout->setValidator( new QIntValidator( 10, 10000, aParent ) );
      _bufferSize->setValidator( new QIntValidator( 32, 16384, aParent ) );

      l2max::CVariant val;

      CAppContext::GetInstance()->MainRegistry().Read( V_ECU_ADAPT_COMMDEVTYP_SL_AUTOSCAN, val );

      bool isAutoScan( val.toInt() );
      _autoPortScan->setCheckState( isAutoScan ? Qt::Checked : Qt::Unchecked );

      CAppContext::GetInstance()->MainRegistry().Read( V_ECU_ADAPT_COMMDEVTYP_SL_NUM, val );

      for( int i( 1 ); i < 257; ++i )
      {
         l2max::CSerialLine serialLine( i );

         if( !isAutoScan ||
               i == val.toInt() || serialLine.open( l2max::CAbstractIODevice::omReadWrite ) )
         {
            std::string name( "COM" + l2max::CVariant( i ).toString() );
            _availableComPorts->addItem( std::string( "COM" + l2max::CVariant( i ).toString() ).c_str(), i );
         }
      }

      _availableComPorts->setCurrentIndex( _availableComPorts->findData( val.toInt() ) );

      const char* speeds[] =
         {
          "110",
          "8192",
          "10400",
          "10472",
          "14400"
         };

      for( unsigned int i( 0 ); i < sizeof( speeds ) / sizeof( *speeds ); ++i )
      {
         _bitsPerSecond->addItem( speeds[ i ] );
      }

      CAppContext::GetInstance()->MainRegistry().Read( V_ECU_ADAPT_COMMDEVTYP_SL_BPS, val );
      _bitsPerSecond->lineEdit()->setText( val.toString().c_str() );

      CAppContext::GetInstance()->MainRegistry().Read( V_ECU_ADAPT_COMMDEVTYP_SL_RDTIMEO, val );
      _readTimeout->setText( val.toString().c_str() );

      CAppContext::GetInstance()->MainRegistry().Read( V_ECU_ADAPT_COMMDEVTYP_SL_WRTIMEO, val );
      _writeTimeout->setText( val.toString().c_str() );

      CAppContext::GetInstance()->MainRegistry().Read( V_ECU_ADAPT_COMMDEVTYP_SL_BUFSZ, val );
      _bufferSize->setText( val.toString().c_str() );
   }

   virtual ~CSerialPortPage()
   {
      parentWidget()->setMinimumSize( QSize() );
   }

   virtual void save()
   {
      l2max::CVariant val;

      val = _availableComPorts->itemData( _availableComPorts->currentIndex() ).toInt();
      CAppContext::GetInstance()->MainRegistry().Write( V_ECU_ADAPT_COMMDEVTYP_SL_NUM, val );

      val = l2max::CVariant( _bitsPerSecond->lineEdit()->text().toStdString().c_str() );
      val.SetType( l2max::CVariant::tInteger );
      CAppContext::GetInstance()->MainRegistry().Write( V_ECU_ADAPT_COMMDEVTYP_SL_BPS, val );

      val = l2max::CVariant( _autoPortScan->checkState() == Qt::Checked );
      CAppContext::GetInstance()->MainRegistry().Write( V_ECU_ADAPT_COMMDEVTYP_SL_AUTOSCAN, val );

      val = l2max::CVariant( _readTimeout->text().toStdString().c_str() );
      val.SetType( l2max::CVariant::tInteger );
      CAppContext::GetInstance()->MainRegistry().Write( V_ECU_ADAPT_COMMDEVTYP_SL_RDTIMEO, val );

      val = l2max::CVariant( _writeTimeout->text().toStdString().c_str() );
      val.SetType( l2max::CVariant::tInteger );
      CAppContext::GetInstance()->MainRegistry().Write( V_ECU_ADAPT_COMMDEVTYP_SL_WRTIMEO, val );

      val = l2max::CVariant( _bufferSize->text().toStdString().c_str() );
      val.SetType( l2max::CVariant::tInteger );
      CAppContext::GetInstance()->MainRegistry().Write( V_ECU_ADAPT_COMMDEVTYP_SL_BUFSZ, val );
   }

   virtual void show()
   {
      QWidget::show();
   }
};

CPreferencePageAdapter::CPreferencePageAdapter() : CPreferencePage( "Adapter" )
{
   _deviceConfigurationPage = 0;

   setupUi( this );

   _adapterType->addItem( CAbstractAdapter::TypeToText( CAbstractAdapter::atKLine ).c_str(),
                          CAbstractAdapter::atKLine );

   _communicationDevice->addItem( CAbstractAdapter::TypeToText( CAbstractAdapter::dtSerialLine ).c_str(),
                                  CAbstractAdapter::dtSerialLine );

   connect( _communicationDevice, SIGNAL( currentIndexChanged( int ) ),
            this, SLOT( OnCommunicationDeviceChanged( int ) ) );


   l2max::CVariant val;

   CAppContext::GetInstance()->MainRegistry().Read( V_ECU_ADAPT_ADAPTYPE, val );
   _adapterType->setCurrentIndex( _adapterType->findData( val.toInt() ) );


   CAppContext::GetInstance()->MainRegistry().Read( V_ECU_ADAPT_COMMDEVTYP, val );
   _communicationDevice->setCurrentIndex( _communicationDevice->findData( val.toInt() ) );


   if( _communicationDevice->currentIndex() == _communicationDevice->findData( val.toInt() ) )
   {
      OnCommunicationDeviceChanged( _communicationDevice->currentIndex() );
   }
   else
   {
      _communicationDevice->setCurrentIndex( _communicationDevice->findData( val.toInt() ) );
   }
}

CPreferencePageAdapter::~CPreferencePageAdapter()
{
   delete _deviceConfigurationPage;
}

void CPreferencePageAdapter::save()
{
   l2max::CVariant val;

   val = _adapterType->itemData( _adapterType->currentIndex() ).toInt();
   CAppContext::GetInstance()->MainRegistry().Write( V_ECU_ADAPT_ADAPTYPE, val );

   val = _communicationDevice->itemData( _communicationDevice->currentIndex() ).toInt();
   CAppContext::GetInstance()->MainRegistry().Write( V_ECU_ADAPT_COMMDEVTYP, val );

   if( _deviceConfigurationPage )
   {
      _deviceConfigurationPage->save();
   }

   CPreferencePage::save();
}

void CPreferencePageAdapter::OnCommunicationDeviceChanged( int anIndex )
{
   if( _deviceConfigurationPage )
   {
      _deviceConfigurationPage->save();

      delete _deviceConfigurationPage;
      _deviceConfigurationPage = 0;
   }

   CAbstractAdapter::EDeviceType deviceType( static_cast< CAbstractAdapter::EDeviceType >
      ( _communicationDevice->itemData( anIndex ).toInt() ) );

   if( deviceType == CAbstractAdapter::dtSerialLine )
   {
      _deviceConfigurationPage = new CSerialPortPage( _communicationDeviceOptions );
   }

   if( _deviceConfigurationPage )
   {
      _deviceConfigurationPage->show();
   }
}
