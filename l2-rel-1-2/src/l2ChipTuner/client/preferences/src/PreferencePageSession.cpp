/*
 * PreferencePageSession.cpp
 *
 *  Created on: 04.01.2009
 *      Author: L2-Max
 */

#include "PreferencePageSession.h"

#include <AppDefs.h>
#include <AppContext.h>

#include <l2Variant.h>

#include <QFileDialog>
#include <QMessageBox>
#include <QDir>

CPreferencePageSession::CPreferencePageSession() : CPreferencePage( "Session" )
{
   setupUi( this );

   connect( _directoryBrowse, SIGNAL( clicked() ), SLOT( OnDirectoryBrowse() ) );
   connect( _readFrom, SIGNAL( stateChanged(int) ), SLOT( OnReadFrom(int) ) );
   connect( _writeTo, SIGNAL( stateChanged(int) ), SLOT( OnWriteTo(int) ) );
   connect( _auto, SIGNAL( stateChanged(int) ), SLOT( OnAuto(int) ) );

   l2max::CVariant readFrom;
   l2max::CVariant writeTo;
   l2max::CVariant readFromE;
   l2max::CVariant writeToE;
   l2max::CVariant curDir;

   CAppContext::GetInstance()->MainRegistry().Read( V_ECU_DIAG_SESS_DATAFILER, readFrom );
   CAppContext::GetInstance()->MainRegistry().Read( V_ECU_DIAG_SESS_DATAFILEW, writeTo );
   CAppContext::GetInstance()->MainRegistry().Read( V_ECU_DIAG_SESS_DATAFILER_E, readFromE );
   CAppContext::GetInstance()->MainRegistry().Read( V_ECU_DIAG_SESS_DATAFILEW_E, writeToE );
   CAppContext::GetInstance()->MainRegistry().Read( V_ECU_DIAG_SESS_DATADIR, curDir );

   _directory->setText( curDir.toString().c_str() );
   
   RefreshDirectory();

   _readFromFile->setCurrentIndex( _readFromFile->findText( readFrom.toString().c_str() ) );
   _writeToFile->setEditText( writeTo.toString().c_str() );

   if( readFromE.toInt() )
   {
      _readFrom->setCheckState( Qt::Checked );
   }

   if( writeToE.toInt() )
   {
      _writeTo->setCheckState( Qt::Checked );
   }
   
   l2max::CVariant reqAuto;
   CAppContext::GetInstance()->MainRegistry().Read( V_ECU_DIAG_SESS_REQAUTO, reqAuto );
   
   if( reqAuto.toInt() )
   {
      _auto->setCheckState( Qt::Checked );
   }
   
   l2max::CVariant reqTimeo;
   CAppContext::GetInstance()->MainRegistry().Read( V_ECU_DIAG_SESS_REQTIMEO, reqTimeo );

   _requestTimeout->setValue( reqTimeo.toInt() );
   
   
   l2max::CVariant reqQueue;
   CAppContext::GetInstance()->MainRegistry().Read( V_ECU_DIAG_SESS_REQQUEUE, reqQueue );
   
   _queueSize->setValue( reqQueue.toInt() );
}

CPreferencePageSession::~CPreferencePageSession()
{
}

void CPreferencePageSession::save()
{
   CAppContext::GetInstance()->MainRegistry().Write( V_ECU_DIAG_SESS_DATAFILER, _readFromFile->currentText().toStdString() );
   CAppContext::GetInstance()->MainRegistry().Write( V_ECU_DIAG_SESS_DATAFILEW, _writeToFile->currentText().toStdString() );
   CAppContext::GetInstance()->MainRegistry().Write( V_ECU_DIAG_SESS_DATAFILER_E, _readFrom->checkState() == Qt::Checked );
   CAppContext::GetInstance()->MainRegistry().Write( V_ECU_DIAG_SESS_DATAFILEW_E, _writeTo->checkState() == Qt::Checked );
   CAppContext::GetInstance()->MainRegistry().Write( V_ECU_DIAG_SESS_DATADIR, _directory->text().toStdString() );
   CAppContext::GetInstance()->MainRegistry().Write( V_ECU_DIAG_SESS_REQAUTO, _auto->checkState() == Qt::Checked );
   CAppContext::GetInstance()->MainRegistry().Write( V_ECU_DIAG_SESS_REQTIMEO, _requestTimeout->value() );
   CAppContext::GetInstance()->MainRegistry().Write( V_ECU_DIAG_SESS_REQQUEUE, _queueSize->value() );
}

void CPreferencePageSession::OnDirectoryBrowse()
{
   QFileDialog dlg( this, "Select directory", _directory->text() );
   dlg.setFileMode( QFileDialog::DirectoryOnly );

   if( dlg.exec() == QDialog::Accepted )
   {
      _directory->setText( dlg.directory().path().toStdString().c_str() );

      _readFrom->setCheckState( Qt::Unchecked );
      _writeTo->setCheckState( Qt::Unchecked );
      
      std::string curReadFile( _readFromFile->currentText().toStdString() );
      std::string curWriteFile( _writeToFile->currentText().toStdString() );
      
      RefreshDirectory();
      
      _readFromFile->setCurrentIndex( _readFromFile->findText( curReadFile.c_str() ) );
      _writeToFile->setEditText( curWriteFile.c_str() );
   }
}

void CPreferencePageSession::OnReadFrom( int aState )
{
   _readFromFile->setEnabled( aState != Qt::Checked );
   
   if( aState == Qt::Checked )
   {
      if( !_readFromFile->currentText().size() )
      {
         QMessageBox::warning( this, "Read From File", "File can not be empty" );

         _readFrom->setCheckState( Qt::Unchecked );
      }
   }
}

void CPreferencePageSession::OnWriteTo( int aState )
{
   _writeToFile->setEnabled( aState != Qt::Checked );
   
   if( aState == Qt::Checked )
   {
      if( !_writeToFile->currentText().size() )
      {
         QMessageBox::warning( this, "Write From File", "File can not be empty" );

         _writeTo->setCheckState( Qt::Unchecked );
      }
   }
}

void CPreferencePageSession::RefreshDirectory()
{
   _readFromFile->clear();
   _writeToFile->clear();
   
   QDir dir( _directory->text() );

   QStringList list( dir.entryList( QStringList() << "*.ini" << "*.rg", QDir::Files ) );

   for( QStringList::iterator i( list.begin() ); i != list.end(); ++i )
   {
      _readFromFile->addItem( i->toStdString().c_str() );
      _writeToFile->addItem( i->toStdString().c_str() );
   }
}

void CPreferencePageSession::OnAuto( int aState )
{
   _requestTimeout->setEnabled( aState != Qt::Checked );
}
