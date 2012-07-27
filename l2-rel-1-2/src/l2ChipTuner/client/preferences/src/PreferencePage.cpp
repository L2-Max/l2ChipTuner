/*
 * PreferencePage.cpp
 *
 *  Created on: 16.10.2008
 *      Author: L2-Max
 */

#include "PreferencePage.h"


CPreferencePage::CPreferencePage( const std::string& aName ) : _name( aName ){}
CPreferencePage::~CPreferencePage(){}

const std::string& CPreferencePage::name()const
{
   return _name;
}

void CPreferencePage::save()
{
   for( TPreferencesPages::iterator i( _preferencesPages.begin() ); i != _preferencesPages.end(); ++i )
   {
      ( *i )->save();
   }
}
