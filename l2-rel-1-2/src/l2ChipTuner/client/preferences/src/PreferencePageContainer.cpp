/*
 * PreferencePageContainer.cpp
 *
 *  Created on: 16.10.2008
 *      Author: mars
 */

#include "PreferencePageContainer.h"
#include "PreferencePage.h"

#include <QTreeWidgetItem>
#include <QLabel>
#include <QHBoxLayout>

namespace
{
   class CNullPreferencePage : public CPreferencePage
   {
      QLabel* _label;

   public:
      CNullPreferencePage( const std::string& aName ) : CPreferencePage( aName )
      {
         _label = new QLabel( "Select subentry for the list", this );
         _label->setAlignment( Qt::AlignCenter );

         QHBoxLayout* boxLayout( new QHBoxLayout( this ) );
         boxLayout->addWidget( _label );
      }
   };
}

CPreferencePageContainer::CPreferencePageContainer()
{
}

CPreferencePageContainer::~CPreferencePageContainer()
{
}

CPreferencePageContainer* CPreferencePageContainer::AddPage( CPreferencePage* aPage )
{
   _preferencesPages.push_back( aPage );

   return aPage;
}

CPreferencePageContainer* CPreferencePageContainer::AddPage( const std::string& aName )
{
   return AddPage( new CNullPreferencePage( aName ) );
}

void CPreferencePageContainer::AppendChild( QTreeWidgetItem* aParent )
{
   for( TPreferencesPages::iterator i( _preferencesPages.begin() ); i != _preferencesPages.end(); ++i )
   {
      QTreeWidgetItem* child( new QTreeWidgetItem() );

      child->setText( 0, ( *i )->name().c_str() );

      ( *i )->AppendChild( child );

      aParent->addChild( child );
   }
}

CPreferencePage* CPreferencePageContainer::page( TPagesPath aPath )
{
   CPreferencePage* selectedPage( _preferencesPages[ aPath.back() ] );

   aPath.pop_back();

   if( aPath.size() )
   {
      selectedPage = selectedPage->page( aPath );
   }

   return selectedPage;
}
