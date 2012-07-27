/*
 * IPreferenceOptionalPage.h
 *
 *  Created on: 25.03.2009
 *      Author: L2-Max
 */

#ifndef IPREFERENCEOPTIONALPAGE_H_
#define IPREFERENCEOPTIONALPAGE_H_

struct IPreferenceOptionalPage
{
   virtual void save() = 0;
   virtual void show() = 0;

   virtual ~IPreferenceOptionalPage(){}
};

#endif /* IPREFERENCEOPTIONALPAGE_H_ */
