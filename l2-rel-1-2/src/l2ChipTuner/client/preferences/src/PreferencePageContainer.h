/*
 * PreferencePageContainer.h
 *
 *  Created on: 16.10.2008
 *      Author: mars
 */

#ifndef PREFERENCEPAGECONTAINER_H_
#define PREFERENCEPAGECONTAINER_H_

#include <vector>
#include <utility>
#include <string>

class CPreferencePage;

class QTreeWidgetItem;

class CPreferencePageContainer
{
protected:
   typedef std::vector< CPreferencePage* > TPreferencesPages;
   TPreferencesPages _preferencesPages;

   typedef std::vector< int > TPagesPath;

public:
   CPreferencePageContainer();
   virtual ~CPreferencePageContainer();

   CPreferencePageContainer* AddPage( CPreferencePage* aPage );
   CPreferencePageContainer* AddPage( const std::string& aName );

   void AppendChild( QTreeWidgetItem* aParent );

   CPreferencePage* page( TPagesPath aPath );
};

#endif /* PREFERENCEPAGECONTAINER_H_ */
