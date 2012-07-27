/*
 * PreferencePage.h
 *
 *  Created on: 16.10.2008
 *      Author: L2-Max
 */

#ifndef PREFERENCEPAGE_H_
#define PREFERENCEPAGE_H_

#include "PreferencePageContainer.h"

#include <QWidget>

class CPreferencePage : public QWidget,
                        public CPreferencePageContainer
{
   std::string _name;

public:
   CPreferencePage( const std::string& aName );
   virtual ~CPreferencePage();

   const std::string& name()const;

   virtual void save();
};

#endif /* PREFERENCEPAGE_H_ */
