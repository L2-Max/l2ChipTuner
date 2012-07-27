/*
 * PreferencesDialog.cpp
 *
 *  Created on: 15.10.2008
 *      Author: L2-Max
 */

#include "PreferencesDialog.h"
#include "PreferencePage.h"

#include <AppDefs.h>
#include <AppContext.h>

#include <l2String.h>

#include <QHBoxLayout>

#define PATH_SEPARATOR ":"

CPreferencesDialog::CPreferencesDialog( QWidget* aParent ) :
   QDialog( aParent )
{
   setupUi( this );

   _currPreferencePage = 0;

   setWindowFlags( Qt::Tool );

   new QHBoxLayout( _preferencesAreaContext );

   connect( _preferencesTree, SIGNAL( itemSelectionChanged() ), this, SLOT( OnItemSelectionChanged() ) );

   l2max::CVariant windowX;
   l2max::CVariant windowY;
   l2max::CVariant windowWidth;
   l2max::CVariant windowHeight;

   CAppContext::GetInstance()->MainRegistry().Read( V_PREFERENCESWND_X, windowX );
   CAppContext::GetInstance()->MainRegistry().Read( V_PREFERENCESWND_Y, windowY );
   CAppContext::GetInstance()->MainRegistry().Read( V_PREFERENCESWND_WIDTH, windowWidth );
   CAppContext::GetInstance()->MainRegistry().Read( V_PREFERENCESWND_HEIGHT, windowHeight );

   if( windowX.toInt() != -1 && windowY.toInt() != -1 )
   {
      move( windowX.toInt(), windowY.toInt() );
   }

   if( windowWidth.toInt() != -1 && windowHeight.toInt() != -1 )
   {
      resize( windowWidth.toInt(), windowHeight.toInt() );
   }
}

CPreferencesDialog::~CPreferencesDialog()
{
   CAppContext::GetInstance()->MainRegistry().Write( V_PREFERENCESWND_X, x() );
   CAppContext::GetInstance()->MainRegistry().Write( V_PREFERENCESWND_Y, y() );
   CAppContext::GetInstance()->MainRegistry().Write( V_PREFERENCESWND_WIDTH, width() );
   CAppContext::GetInstance()->MainRegistry().Write( V_PREFERENCESWND_HEIGHT, height() );

   QTreeWidgetItem* child( _preferencesTree->currentItem() );
   QTreeWidgetItem* parent( 0 );

   l2max::TStringList path;

   while( child && ( parent = child->parent() ) )
   {
      path.push_front( l2max::CVariant( parent->indexOfChild( child ) ).toString() );

      child = parent;
   }

   if( child )
   {
      path.push_front( l2max::CVariant( _preferencesTree->invisibleRootItem()->indexOfChild( child ) ).toString() );

      CAppContext::GetInstance()->MainRegistry().Write( V_PREFERENCESWND_PREFTREE_SELPATH,
                                                        l2max::CString::List2String( path, PATH_SEPARATOR ).toStdString() );
   }
}

void CPreferencesDialog::accept()
{
   for( TPreferencesPages::iterator i( _preferencesPages.begin() ); i != _preferencesPages.end(); ++i )
   {
      ( *i )->save();
   }

   QDialog::accept();
}

int CPreferencesDialog::exec()
{
   for( TPreferencesPages::iterator i( _preferencesPages.begin() ); i != _preferencesPages.end(); ++i )
   {
      QTreeWidgetItem* root( new QTreeWidgetItem( ) );

      root->setText( 0, ( *i )->name().c_str() );

      ( *i )->AppendChild( root );

      _preferencesTree->addTopLevelItem( root );
   }

   _preferencesTree->expandAll();

   l2max::CVariant val;
   CAppContext::GetInstance()->MainRegistry().Read( V_PREFERENCESWND_PREFTREE_SELPATH, val );

   l2max::TStringList path( l2max::CString::String2List( val.toString(), PATH_SEPARATOR ) );

   QTreeWidgetItem* curItem( _preferencesTree->invisibleRootItem() );

   for( l2max::TStringList::iterator i( path.begin() ); i != path.end(); ++i )
   {
      curItem = curItem->child( l2max::CVariant( i->toStdString() ).toInt() );
   }

   _preferencesTree->setCurrentItem( curItem );

   return QDialog::exec();
}

void CPreferencesDialog::OnItemSelectionChanged()
{
   QList< QTreeWidgetItem* > itemList( _preferencesTree->selectedItems() );

   if( itemList.size() )
   {
      QTreeWidgetItem* selectedItem( itemList.front() );

      TPagesPath pagesPath( PagesReversedPath( selectedItem ) );

      CPreferencePage* selectedPage( page( pagesPath ) );

      _preferencesAreaContext->layout()->removeWidget( _currPreferencePage );
      _preferencesAreaContext->layout()->addWidget( selectedPage );

      _preferenceTitle->setText( selectedPage->name().c_str() );

      if( _currPreferencePage )
      {
         _currPreferencePage->hide();
      }

      _currPreferencePage = selectedPage;
      _currPreferencePage->show();
   }
}

CPreferencePageContainer::TPagesPath CPreferencesDialog::PagesReversedPath( QTreeWidgetItem* anItem )
{
   TPagesPath pagesPath;

   QTreeWidgetItem* parent( 0 );
   QTreeWidgetItem* child( anItem );

   while( parent = child->parent() )
   {
      pagesPath.push_back( parent->indexOfChild( child ) );

      child = parent;
   }

   pagesPath.push_back( _preferencesTree->invisibleRootItem()->indexOfChild( child ) );

   return pagesPath;
}
